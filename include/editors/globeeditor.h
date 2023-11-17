#ifndef GLOBEEDITOR_H
#define GLOBEEDITOR_H

#include "editor.h"

namespace Esri::ArcGISRuntime {
    class Scene;
    class SceneGraphicsView;
    class OrbitGeoElementCameraController;
    class Graphic;
    class GraphicsOverlay;
    class SimpleLineSymbol;
    class SimpleMarkerSymbol;
    class Point;
} // namespace Esri::ArcGISRuntime

class GlobeEditor : public Editor
{
    Q_OBJECT
public:
    GlobeEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

public:
    void setPosition(float p_gpsN, float p_gpsE, float p_height);
    void setRotation(float p_roll, float p_pitch, float p_yaw);

protected slots:
    void setupViewport() override;
    void setupDrawer() override;

public slots:
    void update();

private:
    void setupAircraft();
    void changeLayer(const int index);

private:
    Esri::ArcGISRuntime::Scene *m_scene;
    Esri::ArcGISRuntime::SceneGraphicsView *m_sceneView;

    Esri::ArcGISRuntime::OrbitGeoElementCameraController *m_aircraftCamera;
    Esri::ArcGISRuntime::OrbitGeoElementCameraController *m_groundCamera;

    Esri::ArcGISRuntime::Graphic *m_aircraftGraphic;

    Esri::ArcGISRuntime::Graphic *m_flownAirPathGraphic;
    Esri::ArcGISRuntime::Graphic *m_remainingAirPathGraphic;
    Esri::ArcGISRuntime::Graphic *m_flownGroundPathGraphic;
    Esri::ArcGISRuntime::Graphic *m_remainingGroundPathGraphic;

    Esri::ArcGISRuntime::Graphic *m_groundGraphic;
    Esri::ArcGISRuntime::Graphic *m_beaconGraphic;

    Esri::ArcGISRuntime::GraphicsOverlay *m_aircraftOverlay;
    Esri::ArcGISRuntime::GraphicsOverlay *m_airPathOverlay;
    Esri::ArcGISRuntime::GraphicsOverlay *m_groundPathOverlay;
    Esri::ArcGISRuntime::GraphicsOverlay *m_beaconOverlay;

    Esri::ArcGISRuntime::SimpleLineSymbol *m_solidAirLine;
    Esri::ArcGISRuntime::SimpleLineSymbol *m_dashedAirLine;
    Esri::ArcGISRuntime::SimpleLineSymbol *m_solidGroundLine;
    Esri::ArcGISRuntime::SimpleLineSymbol *m_dashedGroundLine;
    Esri::ArcGISRuntime::SimpleLineSymbol *m_beaconLine;

    Esri::ArcGISRuntime::SimpleMarkerSymbol *m_groundMarker;

    QList<Esri::ArcGISRuntime::Point> *m_gpsPath;
};

#endif // GLOBEEDITOR_H
