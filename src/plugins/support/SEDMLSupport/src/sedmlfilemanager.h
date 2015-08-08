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
// SED-ML file manager
//==============================================================================

#ifndef SEDMLFILEMANAGER_H
#define SEDMLFILEMANAGER_H

//==============================================================================

#include "sedmlfile.h"
#include "sedmlsupportglobal.h"
#include "standardfilemanager.h"

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

class SEDMLSUPPORT_EXPORT SedmlFileManager : public StandardSupport::StandardFileManager
{
    Q_OBJECT

public:
    static SedmlFileManager * instance();

    bool isSedmlFile(const QString &pFileName) const;

    SedmlFile * sedmlFile(const QString &pFileName);

protected:
    virtual bool canLoadFileContents(const QString &pFileContents) const;

    virtual QObject * newFile(const QString &pFileName) const;
};

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================