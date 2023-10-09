#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "editor.h"

namespace Esri::ArcGISRuntime {
    class MapGraphicsView;
    class Graphic;
    class GraphicsOverlay;
    class SimpleLineSymbol;
    class SimpleMarkerSymbol;
    class Point;
    class RouteTask;
    class RouteTracker;
} // namespace Esri::ArcGISRuntime

class MapEditor : public Editor
{
    Q_OBJECT
public:
    MapEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

public:
    void setPosition(float p_gpsN, float p_gpsE);
    void setRotation(float p_yaw);

public slots:
    void update();

private:
    void setupDrawer();
    void setupAircraft();

private:
    Esri::ArcGISRuntime::MapGraphicsView *m_mapView;

    Esri::ArcGISRuntime::Graphic *m_aircraftGraphic;
    Esri::ArcGISRuntime::Graphic *m_flownPathGraphic;
    Esri::ArcGISRuntime::Graphic *m_remainingPathGraphic;
    Esri::ArcGISRuntime::Graphic *m_routeGraphic;
    Esri::ArcGISRuntime::Graphic *m_selfGraphic;

    Esri::ArcGISRuntime::GraphicsOverlay *m_aircraftOverlay;
    Esri::ArcGISRuntime::GraphicsOverlay *m_pathOverlay;
    Esri::ArcGISRuntime::GraphicsOverlay *m_routeOverlay;
    Esri::ArcGISRuntime::GraphicsOverlay *m_selfOverlay;

    Esri::ArcGISRuntime::SimpleLineSymbol *m_solidLine;
    Esri::ArcGISRuntime::SimpleLineSymbol *m_dashedLine;
    Esri::ArcGISRuntime::SimpleLineSymbol *m_routeLine;

    Esri::ArcGISRuntime::SimpleMarkerSymbol *m_selfMarker;

    QList<Esri::ArcGISRuntime::Point> *m_gpsPath;

    Esri::ArcGISRuntime::RouteTask *m_routeTask;
    Esri::ArcGISRuntime::RouteTracker *m_routeTracker;

    bool properlySetup;
};

#endif // MAPEDITOR_H
