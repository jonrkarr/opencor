#ifndef QSCINTILLAPLUGIN_H
#define QSCINTILLAPLUGIN_H

#include "plugininfo.h"

namespace OpenCOR {
namespace QScintilla {

PLUGININFO_FUNC QScintillaPluginInfo();

class QScintillaPlugin : public QObject
{
    Q_OBJECT
};

} }

#endif
