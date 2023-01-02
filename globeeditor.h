#ifndef GLOBEEDITOR_H
#define GLOBEEDITOR_H

#include "editor.h"

namespace Esri::ArcGISRuntime {
    class Scene;
    class SceneGraphicsView;
    class OrbitGeoElementCameraController;
    class GraphicsOverlay;
    class Graphic;
} // namespace Esri::ArcGISRuntime

class GlobeEditor : public Editor
{
    Q_OBJECT
public:
    GlobeEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

public:
    void setPosition(float p_gpsN, float p_gpsE, float p_height);
    void setRotation(float p_roll, float p_pitch, float p_yaw);

public slots:
    void updateGeometry();

private:
    void setupDrawer();
    void setupAircraft();

private:
    Esri::ArcGISRuntime::Scene *m_scene;
    Esri::ArcGISRuntime::SceneGraphicsView *m_sceneView;
    Esri::ArcGISRuntime::OrbitGeoElementCameraController *m_camera;
    Esri::ArcGISRuntime::GraphicsOverlay *m_overlay;
    Esri::ArcGISRuntime::Graphic *m_graphic;

    bool properlySetup;
};

#endif // GLOBEEDITOR_H
