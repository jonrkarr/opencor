/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// CLI utilities
//==============================================================================

#include "cliapplication.h"
#include "cliutils.h"
#include "settings.h"

//==============================================================================

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QResource>
#include <QSettings>
#include <QTemporaryFile>

//==============================================================================

namespace OpenCOR {

//==============================================================================

#include "corecliutils.cpp.inl"

//==============================================================================

void initQtMessagePattern()
{
    // We don't want to see debug/warning messages when not in debug mode

#ifndef QT_DEBUG
    qSetMessagePattern("%{if-debug}%{endif}"
                       "%{if-warning}%{endif}"
                       "%{if-critical}%{message}%{endif}"
                       "%{if-fatal}%{message}%{endif}");
#endif
}

//==============================================================================

void initPluginsPath(const QString &pAppFileName)
{
    // Initialise the plugins path

    QFileInfo appFileInfo = pAppFileName;
    QString appDir;

#ifdef Q_OS_WIN
    if (appFileInfo.completeSuffix().isEmpty()) {
        // If pAppFileName has no suffix, then it means that we tried to run
        // OpenCOR using something like "[OpenCOR]/OpenCOR", in which case
        // QFileInfo() will be lost when trying to retrieve the canonical path
        // for OpenCOR. Now, when we use something like "[OpenCOR]/OpenCOR",
        // it's as if we were to use something like "[OpenCOR]/OpenCOR.com", so
        // update appFileInfo accordingly

        appFileInfo = pAppFileName+".com";
    }
#endif

    appDir = nativeCanonicalDirName(appFileInfo.canonicalPath());

    QString pluginsDir = QString();

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    pluginsDir = appDir+QDir::separator()+QString("..")+QDir::separator()+"plugins";

    if (!QDir(pluginsDir).exists()) {
        // The plugins directory doesn't exist, which should only happen if we
        // are trying to run OpenCOR from within Qt Creator, in which case
        // OpenCOR's file name will be [OpenCOR]/build/OpenCOR.exe rather than
        // [OpenCOR]/build/bin/OpenCOR.exe as it should normally be if we were
        // to mimic the case where OpenCOR has been deployed. Then, because the
        // plugins are in [OpenCOR]/build/plugins/OpenCOR, we must skip the
        // "../" bit. So, yes, it's not neat, but is there another solution?...

        pluginsDir = appDir+QDir::separator()+"plugins";
    }
#elif defined(Q_OS_MAC)
    pluginsDir = appDir+QDir::separator()+QString("..")+QDir::separator()+"PlugIns";
#else
    #error Unsupported platform
#endif

    QCoreApplication::setLibraryPaths(QStringList() << nativeCanonicalDirName(pluginsDir));
}

//==============================================================================

void initApplication(QCoreApplication *pApp, QString *pAppDate)
{
    // Remove our global settings, in case OpenCOR previously crashed or
    // something (and therefore didn't remove all of them before quitting)

    OpenCOR::removeGlobalSettings();

    // Ignore SSL-related warnings
    // Note #1: this is to address an issue with QSslSocket not being able to
    //          resolve some methods...
    // Note #2: see issue #516 for more information...

    qputenv("QT_LOGGING_RULES", "qt.network.ssl.warning=false");

    // Set the name of the application

    pApp->setApplicationName(QFileInfo(pApp->applicationFilePath()).baseName());

    // Retrieve and set the version of the application

    QString versionData;

    readTextFromFile(":app_versiondate", versionData);

    QStringList versionDataList = versionData.split(eolString());

    pApp->setApplicationVersion(versionDataList.first());

    if (pAppDate)
        *pAppDate = versionDataList.last();
}

//==============================================================================

bool cliApplication(QCoreApplication *pApp, int *pRes)
{
    // Create and run our CLI application object

    CliApplication *cliApp = new CliApplication(pApp);

    bool res = cliApp->run(pRes);

    delete cliApp;

    return res;
}

//==============================================================================

void removeGlobalSettings()
{
    // Remove the global settings shared between OpenCOR and its different
    // plugins

    QSettings(SettingsOrganization, SettingsApplication).remove(SettingsGlobal);
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
