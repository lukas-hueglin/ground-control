#include "globeeditor.h"

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD


#include "ArcGISTiledElevationSource.h"
#include "Graphic.h"
#include "ModelSceneSymbol.h"
#include "OrbitGeoElementCameraController.h"
#include "Scene.h"
#include "SceneGraphicsView.h"
#include "SimpleRenderer.h"
#include "MapTypes.h"
#include "TaskWatcher.h"
#include "GraphicsOverlayListModel.h"
#include "GraphicListModel.h"
#include "Surface.h"
#include "ElevationSourceListModel.h"
#include "LayerSceneProperties.h"
#include "RendererSceneProperties.h"
#include "AttributeListModel.h"
#include "SceneViewTypes.h"
#include "GraphicsOverlay.h"
#include "SpatialReference.h"
#include "Point.h"
#include "Camera.h"
#include "SpatialReference.h"

#include <QUrl>
#include <QMainWindow>

using namespace Esri::ArcGISRuntime;

GlobeEditor::GlobeEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // create a new scene
    Scene* m_scene = new Scene(BasemapStyle::ArcGISImageryStandard, this);

    // create a new elevation source
    ArcGISTiledElevationSource* elevationSource = new ArcGISTiledElevationSource(
        QUrl("https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer"), this);

    // add the elevation source to scene
    m_scene->baseSurface()->elevationSources()->append(elevationSource);

    // create new SceneGraphicsView
    m_sceneView = new SceneGraphicsView(this);
    m_sceneView->setArcGISScene(m_scene);

    // set atmoshpere effect to realistic
    m_sceneView->setAtmosphereEffect(AtmosphereEffect(2));

    // set viewport and add to container
    m_viewport = m_sceneView;
    m_container->addWidget(m_viewport);

    // check if log file is already loaded
    if (m_dataFrame->isAlreadySetup()){
        setupAircraft();
        //setupDrawer();
        // set properlySetup
        properlySetup = true;
    }
    else {
        // set properlySetup
        properlySetup = false;
    }

    // connect reload of log file
    connect(m_dataFrame, &DataFrame::onFileChanged, [this](){
        setupAircraft();
        //setupDrawer();
        properlySetup = true;
    });
}

void GlobeEditor::setupAircraft() {
    // create a aircraft model.
    QUrl aircraftURL = QUrl("C:/Users/lukas/OneDrive/Desktop/GliderAircraft/GliderAircraft.obj");
    ModelSceneSymbol* aircraftModel = new ModelSceneSymbol(aircraftURL, 10.0, this);

    // create graphic
    m_graphic = new Graphic(Point(8.5431, 47.3668, 10, SpatialReference::wgs84()), aircraftModel, this);

    // create graphics overlay and add graphic
    m_overlay = new GraphicsOverlay(this);
    m_overlay->setSceneProperties(LayerSceneProperties(SurfacePlacement::Relative));
    m_overlay->graphics()->append(m_graphic);

    // add overlay to scene view
    m_sceneView->graphicsOverlays()->append(m_overlay);

    // create a renderer to control aircrafts rotation
    SimpleRenderer* renderer3D = new SimpleRenderer(this);

    //Set the renderer pitch/heading expressions, so the plane can be oriented via properties.
    RendererSceneProperties properties = renderer3D->sceneProperties();
    properties.setRollExpression("[ROLL]");
    properties.setPitchExpression("[PITCH]");
    properties.setHeadingExpression("[YAW]");
    renderer3D->setSceneProperties(properties);

    m_overlay->setRenderer(renderer3D);

    // insert rotation attributes
    m_graphic->attributes()->insertAttribute("ROLL", 0);
    m_graphic->attributes()->insertAttribute("PITCH", 0);
    m_graphic->attributes()->insertAttribute("YAW", 0);

    // create camera
    m_camera = new OrbitGeoElementCameraController(m_graphic, 10, this);

    // restrict the camera to stay between 1 meter and 10 kilometers from the plane
    m_camera->setMinCameraDistance(1);
    m_camera->setMaxCameraDistance(10000);

    // position the plane a third from the bottom of the screen
    m_camera->setTargetVerticalScreenFactor(0.33f);

    // don't pitch the camera when the plane pitches
    m_camera->setAutoPitchEnabled(false);

    // apply our new orbiting camera to the scene view.
    m_sceneView->setCameraController(m_camera);

    // update geometry
    updateGeometry();

    // connect feedback
    connect(m_dataFrame, &DataFrame::onTimeChanged, this, &GlobeEditor::updateGeometry);
}

void GlobeEditor::updateGeometry() {
    float gpsN = m_dataFrame->getValue("gps_n");
    float gpsE = m_dataFrame->getValue("gps_e");
    float height = m_dataFrame->getValue("height");

    float roll = m_dataFrame->getValue("roll");
    float pitch = m_dataFrame->getValue("pitch");
    float yaw = m_dataFrame->getValue("yaw");

    setPosition(gpsN, gpsE, height);
    setRotation(roll, pitch, yaw);
}

void GlobeEditor::setPosition(float p_gpsN, float p_gpsE, float p_height) {
    m_graphic->setGeometry(Point(p_gpsE, p_gpsN, p_height, SpatialReference::wgs84()));
}

void GlobeEditor::setRotation(float p_roll, float p_pitch, float p_yaw) {
    m_graphic->attributes()->replaceAttribute("ROLL", 180*p_roll/M_PI);
    m_graphic->attributes()->replaceAttribute("PITCH", -180*p_pitch/M_PI);
    m_graphic->attributes()->replaceAttribute("YAW", 180*(p_yaw/M_PI+1));
}
