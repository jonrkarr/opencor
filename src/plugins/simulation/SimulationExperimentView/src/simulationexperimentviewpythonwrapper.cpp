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
// Simulation Experiment view Python wrapper
//==============================================================================

#include "centralwidget.h"
#include "corecliutils.h"
#include "filemanager.h"
#include "pythonqtsupport.h"
#include "simulation.h"
#include "simulationexperimentviewplugin.h"
#include "simulationexperimentviewpythonwrapper.h"
#include "simulationexperimentviewwidget.h"

//==============================================================================

#include <array>

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

static PyObject * simulation(const QString &pFileName,
                             SimulationExperimentViewWidget *pSimulationExperimentViewWidget)
{
    // Return the simualtion, should there be one, for the given file using the
    // given Simulation Experiment view widget

    SimulationSupport::Simulation *simulation = pSimulationExperimentViewWidget->simulation(pFileName);

    if (simulation != nullptr) {
        if (simulation->runtime() == nullptr) {
            // The simulation is missing a runtime, so raise a Python exception

            PyErr_SetString(PyExc_ValueError, qPrintable(QObject::tr("unable to get the simulation's runtime")));

            return nullptr;
        }

        // Return our simulation wrapped into a Python object

        return PythonQt::priv()->wrapQObject(simulation);
    }

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
}

//==============================================================================

static PyObject * simulation(PyObject *pSelf,  PyObject *pArgs)
{
    Q_UNUSED(pSelf)
    Q_UNUSED(pArgs)

    // Return the current simulation

    SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();

    if (simulationExperimentViewWidget != nullptr) {
        return simulation(Core::centralWidget()->currentFileName(), simulationExperimentViewWidget);
    }

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
}

//==============================================================================

static PyObject * initializeSimulation(const QString &pFileName)
{
    // Initialise a simulation for the given file name

    SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();

    if (simulationExperimentViewWidget != nullptr) {
        simulationExperimentViewWidget->initialize(pFileName);

        return simulation(pFileName, simulationExperimentViewWidget);
    }

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
}

//==============================================================================

#define GUI_SUPPORT
    #include "opensimulation.cpp.inl"
#undef GUI_SUPPORT

//==============================================================================

#define GUI_SUPPORT
    #include "closesimulation.cpp.inl"
#undef GUI_SUPPORT

//==============================================================================

SimulationExperimentViewPythonWrapper::SimulationExperimentViewPythonWrapper(void *pModule,
                                                                             QObject *pParent) :
    QObject(pParent)
{
    // Add some Python wrappers

    static std::array<PyMethodDef, 4> PythonSimulationExperimentViewMethods = {{
                                                                                  { "simulation",  simulation, METH_VARARGS, "The current simulation." },
                                                                                  { "openSimulation", openSimulation, METH_VARARGS, "Open a simulation." },
                                                                                  { "closeSimulation", closeSimulation, METH_VARARGS, "Close a simulation." },
                                                                                  { nullptr, nullptr, 0, nullptr }
                                                                              }};

    PyModule_AddFunctions(static_cast<PyObject *>(pModule),
                          PythonSimulationExperimentViewMethods.data());
}

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================