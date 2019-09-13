    /*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Run Python script plugin
//==============================================================================

#include "pythonqtsupportplugin.h"
#include "pythonrunscriptplugin.h"

//==============================================================================

#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QTimer>

//==============================================================================

#include "pythonbegin.h"
    #include "pythonqtsupport.h"
#include "pythonend.h"

//==============================================================================

namespace OpenCOR {
namespace PythonRunScript {

//==============================================================================

PLUGININFO_FUNC PythonRunScriptPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the run Python script  plugin."));
    descriptions.insert("fr", QString::fromUtf8("the run Python script  plugin."));

    return new PluginInfo(PluginInfo::Category::Miscellaneous, true, true,
                          QStringList() << "Core" << "DataStore"
                                        << "SimulationSupport"
                                        << "PythonQtSupport",
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

bool PythonRunScriptPlugin::executeCommand(const QString &pCommand,
                                           const QStringList &pArguments,
                                           int &pRes)
{
    // Run the given CLI command

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return true;
    } else if (pCommand.isEmpty() || !pCommand.compare("python")) {
        if (pArguments.count() == 0) {
            // Run an interactive Python shell

            return runShell(pArguments, pRes);
        } else {
            // Run the Python script in the specified file

            return runScript(pArguments, pRes);
        }
    } else {
        // Not a CLI command that we support

        runHelpCommand();

        return false;
    }
}

//==============================================================================
// Plugin specific
//==============================================================================

// Should we combine "script" and "shell" ??
// And rename the plugin to say "RunPython" ??

void PythonRunScriptPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the run Python script plugin:" << std::endl;
    std::cout << " * Display the commands supported by the plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Run an interactive Python shell in OpenCOR's environment:" << std::endl;
    std::cout << "      python" << std::endl;
    std::cout << " * Run a Python script in OpenCOR's environment:" << std::endl;
    std::cout << "      python <python_file> [parameters...]" << std::endl;
}

//==============================================================================

bool PythonRunScriptPlugin::runScript(const QStringList &pArguments, int &pRes)
{
    // Make sure that we have sufficient arguments

    if (pArguments.count() < 1) {
        runHelpCommand();

        return false;
    }

    const QString filename = pArguments[0];

    if (!QFile::exists(filename)) {
        std::cerr << "File '" << filename.toStdString() << "' does not exist" << std::endl;

        return false;
    }

    // Set `sys.argv` to the list of arguments

    PyObject *arguments = PyList_New(0);

    for (const auto &argument : pArguments) {
        PyList_Append(arguments, PyUnicode_FromString(argument.toUtf8().constData()));
    }

    auto sysModule = PythonQtSupport::importModule("sys");

    PyModule_AddObject(sysModule.object(), "argv", arguments);  // Steals the reference

    // Run the file

    PythonQtSupport::evalFile(filename);

    // Pass back Python's `sys.exit()` code

    pRes = PythonQtSupport::PythonQtSupportPlugin::instance()->systemExitCode();

    return true;
}

//==============================================================================

bool PythonRunScriptPlugin::runShell(const QStringList &pArguments, int &pRes)
{
    // The following has been adapted from `Programs/python.c` in the Python sources.

    const int argc = pArguments.size() + 1;
    wchar_t **argv = reinterpret_cast<wchar_t **>(PyMem_RawMalloc(sizeof(wchar_t*) * (argc+1)));

    /* We need a second copy, as Python might modify the first one. */
    wchar_t **argv_copy = reinterpret_cast<wchar_t **>(PyMem_RawMalloc(sizeof(wchar_t*) * (argc+1)));

    if (!argv || !argv_copy) {
        fprintf(stderr, "out of memory\n");
        pRes = 1;
        return false;
    }

    /* PEP 754 requires that FP exceptions run in "no stop" mode by default,
     * and until C vendors implement C99's ways to control FP exceptions,
     * Python requires non-stop mode.  Alas, some platforms enable FP
     * exceptions by default.  Here we disable them.
     */
#ifdef __FreeBSD__
    fedisableexcept(FE_OVERFLOW);
#endif

    char *oldloc = _PyMem_RawStrdup(setlocale(LC_ALL, nullptr));
    if (!oldloc) {
        fprintf(stderr, "out of memory\n");
        pRes = 1;
        return false;
    }

    setlocale(LC_ALL, "");
    argv_copy[0] = argv[0] = Py_DecodeLocale("python", nullptr);
    for (int i = 1; i < argc; i++) {
        argv[i] = Py_DecodeLocale(pArguments[i-1].toUtf8().constData(), nullptr);
        if (!argv[i]) {
            PyMem_RawFree(oldloc);
            fprintf(stderr, "Fatal Python error: "
                            "unable to decode the command line argument #%i\n",
                            i + 1);
            pRes = 1;
            return false;
        }
        argv_copy[i] = argv[i];
    }
    argv_copy[argc] = argv[argc] = nullptr;

    setlocale(LC_ALL, oldloc);
    PyMem_RawFree(oldloc);

    // N.B. PyMain() calls Py_Initialize() and Py_Finalize()

    pRes = Py_Main(argc, argv);

    for (int i = 0; i < argc; i++) {
        PyMem_RawFree(argv_copy[i]);
    }
    PyMem_RawFree(argv);
    PyMem_RawFree(argv_copy);

    return true;
}

//==============================================================================

}   // namespace PythonRunScript
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
