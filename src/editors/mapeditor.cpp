#include "editors/mapeditor.h"

#ifdef PCH_BUILD
#include "pch.hpp"
#endif // PCH_BUILD

#include "Graphic.h"
#include "PictureMarkerSymbol.h"
#include "Map.h"
#include "MapGraphicsView.h"
#include "SimpleRenderer.h"
#include "MapTypes.h"
#include "GraphicsOverlayListModel.h"
#include "GraphicListModel.h"
#include "RendererSceneProperties.h"
#include "AttributeListModel.h"
#include "GraphicsOverlay.h"
#include "SpatialReference.h"
#include "Point.h"
#include "SpatialReference.h"
#include "PolylineBuilder.h"
#include "SimpleLineSymbol.h"
#include "SimpleMarkerSymbol.h"
#include "Basemap.h"
#include "FeatureLayer.h"
#include "LayerListModel.h"
#include "ShapefileFeatureTable.h"
#include "SymbolTypes.h"
#include "SimpleFillSymbol.h"
#include "SimpleRenderer.h"

#include <QUrl>
#include <QMainWindow>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>

#include "helpers/drawer.h"

using namespace Esri::ArcGISRuntime;

MapEditor::MapEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // set QLists
    m_gpsPath = new QList<Point>;

    // connect setupViewport and setupDrawer methods
    connect(m_dataFrame, &DataFrame::onFileChange, this, &MapEditor::setupViewport);
    connect(m_dataFrame, &DataFrame::onFileChange, this, &MapEditor::setupDrawer);

    // check if the dataframe is already ready
    if (m_dataFrame->isAlreadySetup()) {
        // call setupViewport and setupDrawer
        setupViewport();
        setupDrawer();
    }
}

void MapEditor::setupViewport() {
    // call parent function to delete the old viewport
    Editor::setupViewport();

    // create a new scene
    m_map = new Map(BasemapStyle::ArcGISDarkGray, this);

    // add drone layers
    addShapeFileLayer(QString("C://Users//lukas//Documents//QtProjects//ground-control//arcgis//layers//UASZones//nature//nature.shp"), QColor(255, 170, 0), SimpleFillSymbolStyle::Solid);
    addShapeFileLayer(QString("C://Users//lukas//Documents//QtProjects//ground-control//arcgis//layers//UASZones//sensitive//sensitive.shp"), QColor(0, 174, 255), SimpleFillSymbolStyle::Solid);
    addShapeFileLayer(QString("C://Users//lukas//Documents//QtProjects//ground-control//arcgis//layers//UASZones//air_traffic_rst01//air_traffic_rst01.shp"), QColor(110, 0, 60), SimpleFillSymbolStyle::Solid);
    addShapeFileLayer(QString("C://Users//lukas//Documents//QtProjects//ground-control//arcgis//layers//UASZones//air_traffic_rst02//air_traffic_rst02.shp"), QColor(255, 0, 234), SimpleFillSymbolStyle::Solid);
    addShapeFileLayer(QString("C://Users//lukas//Documents//QtProjects//ground-control//arcgis//layers//UASZones//air_traffic_rst03//air_traffic_rst03.shp"), QColor(110, 110, 110), SimpleFillSymbolStyle::BackwardDiagonal);

    // create new SceneGraphicsView
    m_mapView = new MapGraphicsView(this);
    m_mapView->setMap(m_map);

    // set viewport to map View
    m_viewport = m_mapView;
    m_layout->addWidget(m_viewport, 0, 1, 1, 1);

    // call setupAircraft
    setupAircraft();
}

void MapEditor::setupDrawer() {
    // call parent function
    Editor::setupDrawer();

    // get parent
    QGroupBox* drawer = m_drawer->getSettings();

    // create QVBoxLayout
    QVBoxLayout *drawerLayout = new QVBoxLayout(drawer);

    QCheckBox *pathBox = new QCheckBox("Path", drawer);
    pathBox->setProperty("cssClass", "drawerItem");
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

    QCheckBox *routeBox = new QCheckBox("Route", drawer);
    routeBox->setProperty("cssClass", "drawerItem");
    routeBox->setChecked(true);
    drawerLayout->addWidget(routeBox);
    connect(routeBox, &QCheckBox::clicked, [this](bool state){
        if (state) {
            m_routeOverlay->graphics()->append(m_routeGraphic);
        } else {
            m_routeOverlay->graphics()->clear();
        }
    });

    QCheckBox *uasBox = new QCheckBox("Show UAS Map", drawer);
    uasBox->setProperty("cssClass", "drawerItem");
    uasBox->setChecked(true);
    drawerLayout->addWidget(uasBox);
    connect(uasBox, &QCheckBox::clicked, [this](bool state){
        for (int i = 0; i < m_map->operationalLayers()->size(); ++i) {
            m_map->operationalLayers()->at(i)->setVisible(state);
        }
    });

    QComboBox *basemapBox = new QComboBox(m_drawer);
    basemapBox->setProperty("cssClass", "drawerItem");

    QStringList list = {"Dark", "Navigation", "Streets", "Imagery", "Hillshade"};
    basemapBox->addItems(list);

    drawerLayout->addWidget(basemapBox);
    connect(basemapBox, &QComboBox::currentIndexChanged, this, &MapEditor::changeBasemap);

    // add strech item last
    drawerLayout->addStretch(1);

    // add drawerLayout to drawer
    m_drawer->setupContent(drawerLayout);
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

void MapEditor::addShapeFileLayer(QString path, QColor color, SimpleFillSymbolStyle style){
    // create feature table and feature layer
    ShapefileFeatureTable* featureTable = new ShapefileFeatureTable(path, this);
    FeatureLayer* featureLayer = new FeatureLayer(featureTable, this);

    // create the symbols for feature layer
    SimpleLineSymbol* sls = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, color, 1.0f, this);

    // change alpha of color
    color.setAlpha(80);
    SimpleFillSymbol* sfs = new SimpleFillSymbol(style,color , this);

    // set the outline
    sfs->setOutline(sls);

    // create the renderer for the feature layer
    SimpleRenderer* natureSimpleRenderer = new SimpleRenderer(sfs, this);
    featureLayer->setRenderer(natureSimpleRenderer);

    // add feature layer to map
    m_map->operationalLayers()->append(featureLayer);
}

void MapEditor::changeBasemap(const int index) {
    QStringList list = {"Dark", "Navigation", "Streets", "Imagery", "Hillshade"};

    switch(index){
        case 0:
        m_mapView->map()->setBasemap(new Basemap(BasemapStyle::ArcGISDarkGray, this));
            break;
        case 1:
            m_mapView->map()->setBasemap(new Basemap(BasemapStyle::ArcGISNavigationNight, this));
            break;
        case 2:
            m_mapView->map()->setBasemap(new Basemap(BasemapStyle::ArcGISStreetsNight, this));
            break;
        case 3:
            m_mapView->map()->setBasemap(new Basemap(BasemapStyle::ArcGISImagery, this));
            break;
        case 4:
            m_mapView->map()->setBasemap(new Basemap(BasemapStyle::ArcGISHillshadeDark, this));
            break;
    }
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
