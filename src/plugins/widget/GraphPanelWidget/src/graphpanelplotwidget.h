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
// Graph panel plot widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "graphpanelwidgetglobal.h"

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_plot.h"
    #include "qwt_plot_curve.h"
    #include "qwt_scale_draw.h"
    #include "qwt_text.h"
#include "qwtend.h"

//==============================================================================

class QMenu;

//==============================================================================

class QwtPlotDirectPainter;

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

static const double DefMinAxis =    0.0;
static const double DefMaxAxis = 1000.0;

//==============================================================================

class GRAPHPANELWIDGET_EXPORT GraphPanelPlotGraph : public QwtPlotCurve
{
public:
    explicit GraphPanelPlotGraph(void *pParameterX = 0, void *pParameterY = 0);

    bool isValid() const;

    bool isSelected() const;
    void setSelected(const bool &pSelected);

    QString fileName() const;
    void setFileName(const QString &pFileName);

    void * parameterX() const;
    void setParameterX(void *pParameterX);

    void * parameterY() const;
    void setParameterY(void *pParameterY);

private:
    bool mSelected;

    QString mFileName;

    void *mParameterX;
    void *mParameterY;
};

//==============================================================================

typedef QList<GraphPanelPlotGraph *> GraphPanelPlotGraphs;

//==============================================================================

class GraphPanelPlotWidget;

//==============================================================================

class GraphPanelPlotOverlayWidget : public QWidget
{
public:
    explicit GraphPanelPlotOverlayWidget(GraphPanelPlotWidget *pParent);

    void setOriginPoint(const QPoint &pOriginPoint);
    void setPoint(const QPoint &pPoint);

    QRect zoomRegion() const;

protected:
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    enum Location {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    GraphPanelPlotWidget *mOwner;

    QPoint mOriginPoint;
    QPoint mPoint;

    QPoint optimisedPoint(const QPoint &pPoint) const;

    void drawCoordinates(QPainter *pPainter, const QPoint &pPoint,
                         const QColor &pBackgroundColor,
                         const QColor &pForegroundColor,
                         const Location &pLocation,
                         const bool &pCanMoveLocation = true);
};

//==============================================================================

class GraphPanelPlotScaleDraw : public QwtScaleDraw
{
public:
    void retranslateUi();

protected:
    virtual QwtText label(double pValue) const;
};

//==============================================================================

typedef QList<GraphPanelPlotWidget *> GraphPanelPlotWidgets;

//==============================================================================

class GRAPHPANELWIDGET_EXPORT GraphPanelPlotWidget : public QwtPlot,
                                                     public Core::CommonWidget
{
    Q_OBJECT

    friend class GraphPanelPlotOverlayWidget;

public:
    explicit GraphPanelPlotWidget(const GraphPanelPlotWidgets &pNeighbors,
                                  QAction *pSynchronizeXAxisAction,
                                  QAction *pSynchronizeYAxisAction,
                                  QWidget *pParent);
    ~GraphPanelPlotWidget();

    virtual void retranslateUi();

    GraphPanelPlotGraphs graphs() const;

    bool addGraph(GraphPanelPlotGraph *pGraph);
    bool removeGraph(GraphPanelPlotGraph *pGraph);

    QRectF dataRect() const;

    void optimiseAxisX(double &pMin, double &pMax);
    void optimiseAxisY(double &pMin, double &pMax);

    double minX() const;
    double maxX() const;
    double minY() const;
    double maxY() const;

    bool logarithmicXAxis() const;
    void setLogarithmicXAxis(const bool &pLogarithmicXAxis);

    bool logarithmicYAxis() const;
    void setLogarithmicYAxis(const bool &pLogarithmicYAxis);

    bool setAxes(double pMinX, double pMaxX, double pMinY, double pMaxY,
                 const bool &pSynchronizeAxes = true,
                 const bool &pCanReplot = true, const bool &pEmitSignal = true,
                 const bool &pForceXAxisSetting = false,
                 const bool &pForceYAxisSetting = false);

    bool drawGraphFrom(GraphPanelPlotGraph *pGraph,
                       const qulonglong &pFrom);

    GraphPanelPlotWidgets neighbors() const;

    void addNeighbor(GraphPanelPlotWidget *pPlot);
    void removeNeighbor(GraphPanelPlotWidget *pPlot);

    void alignWithNeighbors(const bool &pCanReplot,
                            const bool &pForceAlignment = false);
    void forceAlignWithNeighbors();

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseReleaseEvent(QMouseEvent *pEvent);
    virtual void paintEvent(QPaintEvent *pEvent);
    virtual void resizeEvent(QResizeEvent *pEvent);
    virtual void wheelEvent(QWheelEvent *pEvent);

private:
    enum Action {
        None,
        Pan,
        ShowCoordinates,
        Zoom,
        ZoomRegion
    };

    QwtPlotDirectPainter *mDirectPainter;

    GraphPanelPlotGraphs mGraphs;

    Action mAction;

    QPoint mOriginPoint;
    QPoint mPoint;

    GraphPanelPlotOverlayWidget *mOverlayWidget;

    bool mCanDirectPaint;
    bool mCanReplot;

    bool mCanZoomInX;
    bool mCanZoomOutX;
    bool mCanZoomInY;
    bool mCanZoomOutY;

    bool mNeedContextMenu;
    QMenu *mContextMenu;

    bool mCanUpdateActions;

    QAction *mCopyToClipboardAction;
    QAction *mSynchronizeXAxisAction;
    QAction *mSynchronizeYAxisAction;
    QAction *mLogarithmicXAxisAction;
    QAction *mLogarithmicYAxisAction;
    QAction *mCustomAxesAction;
    QAction *mZoomInAction;
    QAction *mZoomOutAction;
    QAction *mResetZoomAction;

    GraphPanelPlotScaleDraw *mAxisX;
    GraphPanelPlotScaleDraw *mAxisY;

    GraphPanelPlotWidgets mNeighbors;

    void handleMouseDoubleClickEvent(QMouseEvent *pEvent);

    void checkAxisValues(double &pMin, double &pMax);

    void updateActions();

    Action action() const;

    void resetAction();

    bool canZoomInX() const;
    bool canZoomOutX() const;
    bool canZoomInY() const;
    bool canZoomOutY() const;

    void optimiseAxis(const int &pAxisId, double &pMin, double &pMax) const;

    QRectF optimisedRect(const QRectF &pAxes);

    void setAxis(const int &pAxisId, double pMin, double pMax);

    bool resetAxes();

    bool scaleAxis(const double &pScalingFactor,
                   const bool &pCanZoomIn, const bool &pCanZoomOut,
                   const double pOriginPoint, double &pMin, double &pMax);
    void scaleAxes(const QPoint &pPoint,
                   const double &pScalingFactorX,
                   const double &pScalingFactorY);

    QPointF canvasPoint(const QPoint &pPoint,
                        const bool &pNeedOffset = true) const;

signals:
    void axesChanged(const double &pMinX, const double &pMaxX,
                     const double &pMinY, const double &pMaxY);

private slots:
    void cannotUpdateActions();

    void copyToClipboard();
    void customAxes();
    void logarithmicXAxis();
    void logarithmicYAxis();
    void zoomIn();
    void zoomOut();
    void resetZoom();
};

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
