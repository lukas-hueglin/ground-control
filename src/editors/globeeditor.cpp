#include "editors/globeeditor.h"

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
#include "SpatialReference.h"
#include "PolylineBuilder.h"
#include "SimpleLineSymbol.h"
#include "SimpleMarkerSymbol.h"
#include "Basemap.h"
#include "ArcGISSceneLayer.h"
#include "LayerListModel.h"
#include "RasterLayer.h"
#include "GeoPackageRaster.h"
#include "GeoPackage.h"
#include "Error.h"

#include <QUrl>
#include <QMainWindow>
#include <QCheckBox>
#include <QComboBox>

using namespace Esri::ArcGISRuntime;

GlobeEditor::GlobeEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // create a new scene
    Scene* m_scene = new Scene(BasemapStyle::ArcGISDarkGray, this);

    // create a new elevation source
    ArcGISTiledElevationSource* elevationSource = new ArcGISTiledElevationSource(
        QUrl("https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer"), this);

    // add the elevation source to scene
    m_scene->baseSurface()->elevationSources()->append(elevationSource);

    // add a scene layer for buildings
    ArcGISSceneLayer* buildingLayer = new ArcGISSceneLayer(QUrl("https://www.arcgis.com/home/item.html?id=a714a2ca145446b79d97aaa7b895ff95"), this);
    m_scene->operationalLayers()->append(buildingLayer);

    // create new SceneGraphicsView
    m_sceneView = new SceneGraphicsView(this);
    m_sceneView->setArcGISScene(m_scene);

    // set atmoshpere effect to realistic
    m_sceneView->setAtmosphereEffect(AtmosphereEffect(0));

    // set viewport and add to container
    m_viewport = m_sceneView;
    m_container->addWidget(m_viewport);

    // set QLists
    m_gpsPath = new QList<Point>;

    // check if log file is already loaded
    if (m_dataFrame->isAlreadySetup()){
        setupAircraft();
        setupDrawer();
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
        setupDrawer();
        properlySetup = true;
    });
}

void GlobeEditor::setupDrawer() {
    Editor::setupDrawer();

    // create QVBoxLayout
    QVBoxLayout *drawerLayout = new QVBoxLayout(m_drawer);
    m_drawer->setLayout(drawerLayout);

    // create QCheckBoxes
    QCheckBox *cameraBox = new QCheckBox("Aircraft Camera", m_drawer);
    cameraBox->setProperty("cssClass", "drawerItem");
    cameraBox->setChecked(true);
    drawerLayout->addWidget(cameraBox);
    connect(cameraBox, &QCheckBox::clicked, [this](bool state){
        OrbitGeoElementCameraController *camera = state ? m_aircraftCamera : m_groundCamera;
        m_sceneView->setCameraController(camera);
    });

    QCheckBox *airPathBox = new QCheckBox("Air Path", m_drawer);
    airPathBox->setProperty("cssClass", "drawerItem");
    airPathBox->setChecked(true);
    drawerLayout->addWidget(airPathBox);
    connect(airPathBox, &QCheckBox::clicked, [this](bool state){
        if (state) {
            m_airPathOverlay->graphics()->append(m_flownAirPathGraphic);
            m_airPathOverlay->graphics()->append(m_remainingAirPathGraphic);
        } else {
            m_airPathOverlay->graphics()->clear();
        }
    });

    QCheckBox *groundPathBox = new QCheckBox("Ground Path", m_drawer);
    groundPathBox->setProperty("cssClass", "drawerItem");
    groundPathBox->setChecked(true);
    drawerLayout->addWidget(groundPathBox);
    connect(groundPathBox, &QCheckBox::clicked, [this](bool state){
        if (state) {
            m_groundPathOverlay->graphics()->append(m_flownGroundPathGraphic);
            m_groundPathOverlay->graphics()->append(m_remainingGroundPathGraphic);
        } else {
            m_groundPathOverlay->graphics()->clear();
        }
    });

    QCheckBox *beaconBox = new QCheckBox("Beacon", m_drawer);
    beaconBox->setProperty("cssClass", "drawerItem");
    beaconBox->setChecked(true);
    drawerLayout->addWidget(beaconBox);
    connect(beaconBox, &QCheckBox::clicked, [this, cameraBox](bool state){
        if (state) {
            m_beaconOverlay->graphics()->append(m_beaconGraphic);
            m_beaconOverlay->graphics()->append(m_groundGraphic);
            cameraBox->setEnabled(true);
        } else {
            m_beaconOverlay->graphics()->clear();

            m_sceneView->setCameraController(m_aircraftCamera);
            cameraBox->setChecked(true);
            cameraBox->setEnabled(false);
        }
    });

    QComboBox *layerBox = new QComboBox(m_drawer);
    layerBox->setProperty("cssClass", "drawerItem");

    QStringList list = {"Dark", "Navigation", "Streets", "Imagery", "Hillshade"};
    layerBox->addItems(list);

    drawerLayout->addWidget(layerBox);
    connect(layerBox, &QComboBox::currentIndexChanged, this, &GlobeEditor::changeLayer);

    // add strech item last
    drawerLayout->addStretch(1);
}

void GlobeEditor::setupAircraft() {
    // create a aircraft model.
    QUrl aircraftURL = QUrl("C:/Users/lukas/Documents/QtProjects/ground-control/style/models/GliderAircraft.obj");
    ModelSceneSymbol* aircraftModel = new ModelSceneSymbol(aircraftURL, 10.0, this);

    // create graphic
    m_aircraftGraphic = new Graphic(Point(8.5431, 47.3668, 10, SpatialReference::wgs84()), aircraftModel, this);

    // create graphics overlay and add graphic
    m_aircraftOverlay = new GraphicsOverlay(this);
    m_aircraftOverlay->setSceneProperties(LayerSceneProperties(SurfacePlacement::Relative));
    m_aircraftOverlay->graphics()->append(m_aircraftGraphic);

    // create Paths
    PolylineBuilder* flownPathBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);
    PolylineBuilder* remainingPathBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);

    // create line symboly
    m_solidAirLine = new SimpleLineSymbol(SimpleLineSymbolStyle(5), QColor(Qt::white), 3, this);
    m_dashedAirLine = new SimpleLineSymbol(SimpleLineSymbolStyle(0), QColor(Qt::white), 3, this);
    m_solidGroundLine = new SimpleLineSymbol(SimpleLineSymbolStyle(5), QColor(Qt::yellow), 1, this);
    m_dashedGroundLine = new SimpleLineSymbol(SimpleLineSymbolStyle(0), QColor(Qt::yellow), 1, this);

    // create graphics to display the lines with their symbology
    m_flownAirPathGraphic = new Graphic(flownPathBuilder->toGeometry(), m_solidAirLine, this);
    m_remainingAirPathGraphic = new Graphic(remainingPathBuilder->toGeometry(), m_dashedAirLine, this);
    m_flownGroundPathGraphic = new Graphic(flownPathBuilder->toGeometry(), m_solidGroundLine, this);
    m_remainingGroundPathGraphic = new Graphic(remainingPathBuilder->toGeometry(), m_dashedGroundLine, this);

    // create graphics overlays
    m_airPathOverlay = new GraphicsOverlay(this);
    m_airPathOverlay->setSceneProperties(LayerSceneProperties(SurfacePlacement::Relative));
    m_groundPathOverlay = new GraphicsOverlay(this);
    m_groundPathOverlay->setSceneProperties(LayerSceneProperties(SurfacePlacement::DrapedBillboarded));

    m_airPathOverlay->graphics()->append(m_flownAirPathGraphic);
    m_airPathOverlay->graphics()->append(m_remainingAirPathGraphic);
    m_groundPathOverlay->graphics()->append(m_flownGroundPathGraphic);
    m_groundPathOverlay->graphics()->append(m_remainingGroundPathGraphic);

    // horizontal beacon and ground point
    PolylineBuilder* beaconBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);

    // create line symboly
    m_beaconLine = new SimpleLineSymbol(SimpleLineSymbolStyle(2), QColor(Qt::red), 1, this);
    m_groundMarker = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle(2), QColor(Qt::red), 10, this);

    // create graphics to display the lines with their symbology
    m_beaconGraphic = new Graphic(beaconBuilder->toGeometry(), m_beaconLine, this);
    m_groundGraphic = new Graphic(flownPathBuilder->toGeometry(), m_groundMarker, this);

    // create graphics overlays
    m_beaconOverlay = new GraphicsOverlay(this);
    m_beaconOverlay->setSceneProperties(LayerSceneProperties(SurfacePlacement::Relative));

    m_beaconOverlay->graphics()->append(m_beaconGraphic);
    m_beaconOverlay->graphics()->append(m_groundGraphic);

    // add overlays to scene view
    m_sceneView->graphicsOverlays()->append(m_aircraftOverlay);
    m_sceneView->graphicsOverlays()->append(m_airPathOverlay);
    m_sceneView->graphicsOverlays()->append(m_groundPathOverlay);
    m_sceneView->graphicsOverlays()->append(m_beaconOverlay);

    // create a renderer to control aircrafts rotation
    SimpleRenderer* renderer3D = new SimpleRenderer(this);

    //Set the renderer pitch/heading expressions, so the plane can be oriented via properties.
    RendererSceneProperties properties = renderer3D->sceneProperties();
    properties.setRollExpression("[ROLL]");
    properties.setPitchExpression("[PITCH]");
    properties.setHeadingExpression("[YAW]");
    renderer3D->setSceneProperties(properties);

    m_aircraftOverlay->setRenderer(renderer3D);

    // insert rotation attributes
    m_aircraftGraphic->attributes()->insertAttribute("ROLL", 0);
    m_aircraftGraphic->attributes()->insertAttribute("PITCH", 0);
    m_aircraftGraphic->attributes()->insertAttribute("YAW", 0);

    // create camera
    m_aircraftCamera = new OrbitGeoElementCameraController(m_aircraftGraphic, 10, this);
    m_groundCamera = new OrbitGeoElementCameraController(m_groundGraphic, 10, this);

    // restrict the camera to stay between 1 meter and 100 / 1 kilometers from the plane
    m_aircraftCamera->setMinCameraDistance(1);
    m_aircraftCamera->setMaxCameraDistance(100000);
    m_groundCamera->setMinCameraDistance(1);
    m_groundCamera->setMaxCameraDistance(1000);

    // position the plane a third from the bottom of the screen
    m_aircraftCamera->setTargetVerticalScreenFactor(0.4f);
    m_groundCamera->setTargetVerticalScreenFactor(0.4f);

    // don't pitch the camera when the plane pitches
    m_aircraftCamera->setAutoPitchEnabled(false);

    // apply our new orbiting camera to the scene view.
    m_sceneView->setCameraController(m_aircraftCamera);

    // fill m_gpsPath with values
    for (unsigned int t=0; t < m_dataFrame->getSize(); ++t) {
        float gpsN = m_dataFrame->getValue("gps_n", t);
        float gpsE = m_dataFrame->getValue("gps_e", t);
        float height = m_dataFrame->getValue("height", t);

        m_gpsPath->append(Point(gpsE, gpsN, height, SpatialReference::wgs84()));
    }

    // initiate by updating
    update();

    // connect feedback
    connect(m_dataFrame, &DataFrame::onTimeChanged, this, &GlobeEditor::update);
}

void GlobeEditor::changeLayer(const int index) {
    QStringList list = {"Dark", "Navigation", "Streets", "Imagery", "Hillshade"};

    switch(index){
    case 0:
        m_sceneView->arcGISScene()->setBasemap(new Basemap(BasemapStyle::ArcGISDarkGray, this));
        m_sceneView->setAtmosphereEffect(AtmosphereEffect(0));
        break;
    case 1:
        m_sceneView->arcGISScene()->setBasemap(new Basemap(BasemapStyle::ArcGISNavigationNight, this));
        m_sceneView->setAtmosphereEffect(AtmosphereEffect(0));
        break;
    case 2:
        m_sceneView->arcGISScene()->setBasemap(new Basemap(BasemapStyle::ArcGISStreetsNight, this));
        m_sceneView->setAtmosphereEffect(AtmosphereEffect(0));
        break;
    case 3:
        m_sceneView->arcGISScene()->setBasemap(new Basemap(BasemapStyle::ArcGISImagery, this));
        m_sceneView->setAtmosphereEffect(AtmosphereEffect(2));
        break;
    case 4:
        m_sceneView->arcGISScene()->setBasemap(new Basemap(BasemapStyle::ArcGISHillshadeDark, this));
        m_sceneView->setAtmosphereEffect(AtmosphereEffect(0));
        break;
    }
}

void GlobeEditor::update() {
    // update Aircraft
    float gpsN = m_dataFrame->getValue("gps_n");
    float gpsE = m_dataFrame->getValue("gps_e");
    float height = m_dataFrame->getValue("height");

    float roll = m_dataFrame->getValue("roll");
    float pitch = m_dataFrame->getValue("pitch");
    float yaw = m_dataFrame->getValue("yaw");

    setPosition(gpsN, gpsE, height);
    setRotation(roll, pitch, yaw);

    // update paths
    PolylineBuilder* flownPathBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);
    PolylineBuilder* remainingPathBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);

    // append points
    const unsigned int i = m_dataFrame->getTime();
    const unsigned int n = m_gpsPath->size();

    flownPathBuilder->addPoints(m_gpsPath->sliced(0, i+1));
    remainingPathBuilder->addPoints(m_gpsPath->sliced(i, n-i));

    // change geometry of graphics
    m_flownAirPathGraphic->setGeometry(flownPathBuilder->toGeometry());
    m_remainingAirPathGraphic->setGeometry(remainingPathBuilder->toGeometry());
    m_flownGroundPathGraphic->setGeometry(flownPathBuilder->toGeometry());
    m_remainingGroundPathGraphic->setGeometry(remainingPathBuilder->toGeometry());

    // update horizontal beacon
    PolylineBuilder* beaconBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);

    // add points
    beaconBuilder->addPoint(Point(gpsE, gpsN, height));
    beaconBuilder->addPoint(Point(gpsE, gpsN, 0));

    // set geometry
    m_beaconGraphic->setGeometry(beaconBuilder->toGeometry());
    m_groundGraphic->setGeometry(Point(gpsE, gpsN, 0));
}

void GlobeEditor::setPosition(float p_gpsN, float p_gpsE, float p_height) {
    m_aircraftGraphic->setGeometry(Point(p_gpsE, p_gpsN, p_height, SpatialReference::wgs84()));
}

void GlobeEditor::setRotation(float p_roll, float p_pitch, float p_yaw) {
    m_aircraftGraphic->attributes()->replaceAttribute("ROLL", 180*p_roll/M_PI);
    m_aircraftGraphic->attributes()->replaceAttribute("PITCH", -180*p_pitch/M_PI);
    m_aircraftGraphic->attributes()->replaceAttribute("YAW", 180*(p_yaw/M_PI+1));
}
