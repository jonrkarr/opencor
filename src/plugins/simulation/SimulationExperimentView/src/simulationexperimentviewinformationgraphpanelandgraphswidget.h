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
// Simulation Experiment view information graph panel and graphs widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfileruntime.h"
#include "commonwidget.h"
#include "corecliutils.h"
#include "graphpanelplotwidget.h"
#include "propertyeditorwidget.h"

//==============================================================================

#include <QStackedWidget>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace GraphPanelWidget {
    class GraphPanelWidget;
}   // namespace GraphPanelWidget

//==============================================================================

namespace SimulationSupport {
    class Simulation;
}   // namespace SimulationSupport

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

class SimulationExperimentViewSimulationWidget;
class SimulationExperimentViewWidget;

//==============================================================================

class SimulationExperimentViewInformationGraphPanelAndGraphsWidget : public QStackedWidget,
                                                                     public Core::CommonWidget
{
    Q_OBJECT

public:
    enum Mode {
        GraphPanel,
        Graphs
    };

    explicit SimulationExperimentViewInformationGraphPanelAndGraphsWidget(SimulationExperimentViewWidget *pViewWidget,
                                                                          SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                                          QWidget *pParent);

    virtual void retranslateUi();

    void initialize(SimulationSupport::Simulation *pSimulation);
    void finalize();

    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    void updateGui();

    void finishEditing();

    Core::Properties graphPanelProperties(GraphPanelWidget::GraphPanelWidget *pGraphPanel) const;
    Core::Properties graphProperties(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                     const QString &pFileName = QString()) const;

    SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode mode() const;
    void setMode(const Mode &pMode);

    void setGraphPanelColumnWidth(const int &pIndex, const int &pColumnWidth);
    void setGraphsColumnWidth(const int &pIndex, const int &pColumnWidth);

    void setGraphPanelSectionExpanded(const int &pSection, const bool &pExpanded);

    void reinitialize(GraphPanelWidget::GraphPanelWidget *pGraphPanel);

private:
    Mode mMode;

    SimulationExperimentViewWidget *mViewWidget;
    SimulationExperimentViewSimulationWidget *mSimulationWidget;

    QMap<Core::PropertyEditorWidget *, GraphPanelWidget::GraphPanelWidget *> mGraphPanels;
    QMap<GraphPanelWidget::GraphPanelWidget *, Core::PropertyEditorWidget *> mGraphPanelPropertyEditors;
    QMap<GraphPanelWidget::GraphPanelWidget *, Core::PropertyEditorWidget *> mGraphsPropertyEditors;
    Core::PropertyEditorWidget *mGraphPanelPropertyEditor;
    Core::PropertyEditorWidget *mGraphsPropertyEditor;

    QMap<Core::Property *, GraphPanelWidget::GraphPanelPlotGraph *> mGraphs;
    QMap<GraphPanelWidget::GraphPanelPlotGraph *, Core::Property *> mGraphProperties;

    QMenu *mGraphPanelContextMenu;

    QAction *mSelectGraphPanelColorAction;

    QMenu *mGraphContextMenu;
    QMenu *mGraphParametersContextMenu;

    QAction *mAddGraphAction;
    QAction *mRemoveCurrentGraphAction;
    QAction *mRemoveAllGraphsAction;
    QAction *mSelectAllGraphsAction;
    QAction *mUnselectAllGraphsAction;
    QAction *mSelectGraphColorAction;

    QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *> mParameterActions;

    QMap<QString, QString> mRenamedModelListValues;

    bool mCanEmitGraphsUpdatedSignal;

    void retranslateGraphPanelPropertyEditor(Core::PropertyEditorWidget *pGraphPanelPropertyEditor);

    void populateGraphPanelPropertyEditor();

    void populateGraphParametersContextMenu(CellMLSupport::CellmlFileRuntime *pRuntime);

    bool checkParameter(CellMLSupport::CellmlFileRuntime *pRuntime,
                        GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                        Core::Property *pParameterProperty,
                        const bool &pParameterX) const;

    QString modelListValue(const QString &pFileName) const;

    void updateGraphInfo(Core::Property *pProperty);
    void updateGraphsInfo(Core::Property *pSectionProperty = 0);
    void updateAllGraphsInfo();

    void selectAllGraphs(const bool &pSelect);

    bool rootProperty(Core::Property *pProperty) const;

signals:
    void graphPanelGraphsModeChanged(const OpenCOR::SimulationExperimentView::SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode &pMode);

    void graphPanelHeaderSectionResized(const int &pIndex, const int &pOldSize,
                                        const int &pNewSize);
    void graphsHeaderSectionResized(const int &pIndex, const int &pOldSize,
                                    const int &pNewSize);

    void graphPanelSectionExpanded(const int &pSection, const bool &pExpanded);

    void graphUpdated(OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *pGraph);
    void graphsUpdated(const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

public slots:
    void initialize(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                    const bool &pActive = true);
    void finalize(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel);

    void addGraph(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                  OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                  const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphProperties &pGraphProperties);
    void removeGraphs(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                      const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

private slots:
    void selectGraphPanelColor();

    void addGraph();
    void removeCurrentGraph();
    void removeAllGraphs();
    void selectAllGraphs();
    void unselectAllGraphs();
    void selectGraphColor();

    void showGraphPanelContextMenu(const QPoint &pPosition) const;
    void showGraphsContextMenu(const QPoint &pPosition) const;

    void graphPanelSectionExpanded(const QModelIndex &pIndex);
    void graphPanelSectionCollapsed(const QModelIndex &pIndex);

    void graphPanelPropertyChanged(Core::Property *pProperty);
    void graphsPropertyChanged(Core::Property *pProperty);

    void updateParameterValue();
};

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================