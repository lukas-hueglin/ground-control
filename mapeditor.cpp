#include "mapeditor.h"

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD


#include "ArcGISTiledElevationSource.h"
#include "Graphic.h"
#include "PictureMarkerSymbol.h"
#include "OrbitGeoElementCameraController.h"
#include "Map.h"
#include "MapGraphicsView.h"
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
#include "RouteTask.h"

#include <QUrl>
#include <QMainWindow>
#include <QCheckBox>

using namespace Esri::ArcGISRuntime;

MapEditor::MapEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // create a new scene
    Map* map = new Map(BasemapStyle::ArcGISDarkGray, this);

    // create new SceneGraphicsView
    m_mapView = new MapGraphicsView(this);
    m_mapView->setMap(map);

    // set viewport and add to container
    m_viewport = m_mapView;
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

void MapEditor::setupDrawer() {
    Editor::setupDrawer();

    // create QVBoxLayout
    QVBoxLayout *drawerLayout = new QVBoxLayout(m_drawer);
    m_drawer->setLayout(drawerLayout);

    QCheckBox *pathBox = new QCheckBox("Path", m_drawer);
    pathBox->setChecked(true);
    drawerLayout->addWidget(pathBox);
    connect(pathBox, &QCheckBox::clicked, [this](bool state){
        if (state) {
            m_pathOverlay->graphics()->append(m_flownPathGraphic);
            m_pathOverlay->graphics()->append(m_remainingPathGraphic);
        } else {
            m_pathOverlay->graphics()->clear();
        }
    });

    QCheckBox *routeBox = new QCheckBox("Route", m_drawer);
    routeBox->setChecked(true);
    drawerLayout->addWidget(routeBox);
    connect(routeBox, &QCheckBox::clicked, [this](bool state){
        if (state) {
            m_routeOverlay->graphics()->append(m_routeGraphic);
        } else {
            m_routeOverlay->graphics()->clear();
        }
    });
}

void MapEditor::setupAircraft() {
    // create a aircraft model
    QUrl aircraftURL = QUrl("qrc:/aircraftAssets/GliderAircraft.png");
    PictureMarkerSymbol* aircraftPic = new PictureMarkerSymbol(aircraftURL, this);
    aircraftPic->setWidth(20);
    aircraftPic->setHeight(20);

    // create graphic
    m_aircraftGraphic = new Graphic(Point(8.5431, 47.3668, SpatialReference::wgs84()), this);

    // create graphics overlay and add graphic
    m_aircraftOverlay = new GraphicsOverlay(this);
    //m_aircraftOverlay->setSceneProperties(LayerSceneProperties(SurfacePlacement::Relative));
    m_aircraftOverlay->graphics()->append(m_aircraftGraphic);

    // create Paths
    PolylineBuilder* flownPathBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);
    PolylineBuilder* remainingPathBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);

    // create line symboly
    m_solidLine = new SimpleLineSymbol(SimpleLineSymbolStyle(5), QColor("white"), 3, this);
    m_dashedLine = new SimpleLineSymbol(SimpleLineSymbolStyle(0), QColor("white"), 3, this);

    // create graphics to display the lines with their symbology
    m_flownPathGraphic = new Graphic(flownPathBuilder->toGeometry(), m_solidLine, this);
    m_remainingPathGraphic = new Graphic(remainingPathBuilder->toGeometry(), m_dashedLine, this);

    // create graphics overlays
    m_pathOverlay = new GraphicsOverlay(this);
    //m_pathOverlay->setSceneProperties(LayerSceneProperties(SurfacePlacement::Relative));

    m_pathOverlay->graphics()->append(m_flownPathGraphic);
    m_pathOverlay->graphics()->append(m_remainingPathGraphic);

    // create self
    m_selfMarker = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle(2), QColor("pink"), 10, this);

    // create graphics
    m_selfGraphic = new Graphic(Point(8.5431, 47.3668, SpatialReference::wgs84()), m_selfMarker, this);

    // create graphics overlays
    m_selfOverlay = new GraphicsOverlay(this);
    //m_selfOverlay->setSceneProperties(LayerSceneProperties(SurfacePlacement::Relative));

    m_selfOverlay->graphics()->append(m_selfGraphic);

    // add overlays to scene view
    m_mapView->graphicsOverlays()->append(m_aircraftOverlay);
    m_mapView->graphicsOverlays()->append(m_pathOverlay);
    //m_mapView->graphicsOverlays()->append(m_routeOverlay);
    m_mapView->graphicsOverlays()->append(m_selfOverlay);

    // create a renderer to control aircrafts rotation
    SimpleRenderer* renderer2D = new SimpleRenderer(this);
    renderer2D->setSymbol(aircraftPic);

    //Set the renderer pitch/heading expressions, so the plane can be oriented via properties.
    RendererSceneProperties properties = renderer2D->sceneProperties();
    properties.setHeadingExpression("[YAW]");
    renderer2D->setSceneProperties(properties);

    m_aircraftOverlay->setRenderer(renderer2D);

    // insert rotation attributes
    m_aircraftGraphic->attributes()->insertAttribute("YAW", 0);

    // fill m_gpsPath with values
    for (unsigned int t=0; t < m_dataFrame->getSize(); ++t) {
        float gpsN = m_dataFrame->getValue("gps_n", t);
        float gpsE = m_dataFrame->getValue("gps_e", t);

        m_gpsPath->append(Point(gpsE, gpsN, SpatialReference::wgs84()));
    }

    // initiate by updating
    update();

    // connect feedback
    connect(m_dataFrame, &DataFrame::onTimeChanged, this, &MapEditor::update);
}

void MapEditor::update() {
    // update Aircraft
    float gpsN = m_dataFrame->getValue("gps_n");
    float gpsE = m_dataFrame->getValue("gps_e");

    float yaw = m_dataFrame->getValue("yaw");

    setPosition(gpsN, gpsE);
    setRotation(yaw);

    // update paths
    PolylineBuilder* flownPathBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);
    PolylineBuilder* remainingPathBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);

    // append points
    const unsigned int i = m_dataFrame->getTime();
    const unsigned int n = m_gpsPath->size();

    flownPathBuilder->addPoints(m_gpsPath->sliced(0, i+1));
    remainingPathBuilder->addPoints(m_gpsPath->sliced(i, n-i));

    // change geometry of graphics
    m_flownPathGraphic->setGeometry(flownPathBuilder->toGeometry());
    m_remainingPathGraphic->setGeometry(remainingPathBuilder->toGeometry());
}

void MapEditor::setPosition(float p_gpsN, float p_gpsE) {
    m_aircraftGraphic->setGeometry(Point(p_gpsE, p_gpsN, SpatialReference::wgs84()));
}

void MapEditor::setRotation(float p_yaw) {
    m_aircraftGraphic->attributes()->replaceAttribute("YAW", 180*(p_yaw/M_PI+1));
}
