#include "eventdetailspage.h"

#include <MAction>
#include <MComboBox>
#include <MLayout>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <MSlider>
#include <MTheme>
#include <MWidget>
#include <MWidgetAction>
#include <QtGui/QGraphicsLinearLayout>
#include <QtCore/QUrl>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QNetworkProxy>

// QtMobility
#include <QGeoServiceProvider>
#include <QGeoMappingManager>
#include <QGeoRoutingManager>
#include <QGeoMapPixmapObject>
#include <QProcessEnvironment>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>

#include "event.h"
#include "location.h"
#include "mapwidget.h"

using namespace QtMobility;

EventDetailsPage::EventDetailsPage(const DBManager::Storage& storage,
                                   const int& event_id, QGraphicsItem *parent)
  : MApplicationPage(parent),
    m_serviceProvider(0),
    m_dbStorage(storage),
    m_mapWidget(0),
    m_session(0)
{
  m_eventID = event_id;
  // Set Internet Access Point
  QNetworkConfigurationManager manager;
  const bool canStartIAP = (manager.capabilities()
                            & QNetworkConfigurationManager::CanStartAndStopInterfaces);
  // Is there default access point, use it
  QNetworkConfiguration cfg = manager.defaultConfiguration();
  if (!cfg.isValid() || (!canStartIAP && cfg.state() != QNetworkConfiguration::Active)) {
    //TODO handle
    return;
  }

  m_session = new QNetworkSession(cfg, this);
  connect(m_session, SIGNAL(opened()), this, SLOT(networkSessionOpened()));
  //  connect(m_session,
  //          SIGNAL(error(QNetworkSession::SessionError)),
  //          this,
  //          SLOT(error(QNetworkSession::SessionError)));
}

void EventDetailsPage::networkSessionOpened()
{
  QString urlEnv = QProcessEnvironment::systemEnvironment().value("http_proxy");
  if (!urlEnv.isEmpty()) {
    QUrl url = QUrl(urlEnv, QUrl::TolerantMode);
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(url.host());
    proxy.setPort(url.port(8080));
    QNetworkProxy::setApplicationProxy(proxy);
  } else
    QNetworkProxyFactory::setUseSystemConfiguration(true);
  setProvider("nokia");

  createMap();
}

void EventDetailsPage::setProvider(QString providerId)
{
  if (m_serviceProvider)
    delete m_serviceProvider;
  m_serviceProvider = new QGeoServiceProvider(providerId);
  if (m_serviceProvider->error() != QGeoServiceProvider::NoError) {
    //TODO: handle
    return;
  }

  m_mapManager = m_serviceProvider->mappingManager();
  m_routingManager = m_serviceProvider->routingManager();
  m_searchManager = m_serviceProvider->searchManager();
}

void EventDetailsPage::createContent()
{
  MLayout *layout = new MLayout;
  QGraphicsWidget *panel = centralWidget();
  panel->setLayout(layout);

  Event* event = DBManager::instance(m_dbStorage)->eventFromID(m_eventID);
  m_starred = event->starred();
  Location* location = event->location();

  // Build a vertical layout that will hold all the informations.
  m_infoLayout = new QGraphicsLinearLayout(Qt::Vertical);
  //  infoLayout->addItem(new MLabel("<strong>Headliner</strong>"));
  //  infoLayout->addItem(new MLabel(event.headliner()));

  m_infoLayout->addItem(new MLabel("<strong>" + tr("When") + "</strong>"));
  m_infoLayout->addItem(new MLabel(event->dateTime().toString(Qt::TextDate)));

  m_infoLayout->addItem(new MLabel("<strong>" + tr("Artists") + "</strong>"));
  QString artistsString ("<ul>");
  foreach(QString artist, event->artists()){
    artistsString += QString("<li>%1</li>").arg(artist);
  }
  artistsString += "</ul>";
  m_infoLayout->addItem(new MLabel(artistsString));

  m_infoLayout->addItem(new MLabel("<strong>" + tr("Where") + "</strong>"));
  QString locationString ("<ul>");
  locationString += QString("<li><em>" + tr("Name") + ": </em>%1</li>")\
      .arg(location->name());
  locationString += QString("<li><em>" + tr("Street") + ": </em>%1</li>")\
      .arg(location->street());
  locationString += QString("<li><em>" + tr("City") + ": </em>%1</li>")\
      .arg(location->city());
  locationString += QString("<li><em>" + tr("Country") + ": </em>%1</li>")\
      .arg(location->country());
  locationString += "</ul>";
  m_infoLayout->addItem(new MLabel(locationString));

  // Landscape orientation
  MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
  landscapePolicy->addItem(m_infoLayout);
  layout->setLandscapePolicy(landscapePolicy);

  // Portrait orientation
  MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
  portraitPolicy->addItem(m_infoLayout);
  layout->setPortraitPolicy(portraitPolicy);

  // Toolbar Actions
  m_actionStar = new MAction(this);
  m_actionStar->setLocation(MAction::ToolBarLocation);
  addAction(m_actionStar);
  connect(m_actionStar, SIGNAL(triggered()), this, SLOT(slotChangeStar()));
  updateStarredAction();

  // trigger creation of the map widget
  if (m_session)
    m_session->open();
  else {
    m_infoLayout->addItem(new MLabel(tr("No network connection, cannot fetch map.")));
  }

  delete event;
}

void EventDetailsPage::createMap()
{
  setTitle(tr("Event details"));
  Event* event = DBManager::instance(m_dbStorage)->eventFromID(m_eventID);
  m_starred = event->starred();
  Location* location = event->location();
  QGeoCoordinate eventCoordinates (location->latitude(), location->longitude());
  delete event;

  m_mapWidget = new MapWidget(m_mapManager);
  m_mapWidget->setCenter(eventCoordinates);
  QGeoMapPixmapObject *marker = new QGeoMapPixmapObject(eventCoordinates,
                                      QPoint(0,0),
                                      *MTheme::pixmap("icon-m-common-location"));
  m_mapWidget->addMapObject(marker);
  m_mapWidget->setZoomLevel(m_mapWidget->maximumZoomLevel());

  MWidget* mapWidget =new MWidget;
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);

  m_slider = new MSlider();
  m_slider->setOrientation(Qt::Vertical);
  m_slider->setMinLabelIconID("icon-m-camera-zoom-out");
  m_slider->setMinLabelVisible(true);
  m_slider->setMinimum(m_mapWidget->minimumZoomLevel());
//  m_slider->setMinLabel(tr("Min"));
  m_slider->setMaximum(m_mapWidget->maximumZoomLevel());
  m_slider->setMaxLabelIconID("icon-m-camera-zoom-in");
  m_slider->setMaxLabelVisible(true);
//  m_slider->setMaxLabel(tr("Max"));
  m_slider->setValue(m_mapWidget->zoomLevel());
  connect(m_slider, SIGNAL(valueChanged(int)),
          this, SLOT(sliderValueChanged(int)));

  layout->addItem(m_slider);
  layout->addItem(m_mapWidget);
  mapWidget->setLayout(layout);
  m_infoLayout->addItem(mapWidget);

  // Menu Actions
  MWidgetAction *mapTypeAction = new MWidgetAction(centralWidget());
  mapTypeAction->setLocation(MAction::ApplicationMenuLocation);

  QStringList mapTypeNames;

  QList<QGraphicsGeoMap::MapType> supportedMapTypes = m_mapWidget->supportedMapTypes();

  if (supportedMapTypes.contains(QGraphicsGeoMap::StreetMap)) {
    m_mapControlTypes.append(QGraphicsGeoMap::StreetMap);
    mapTypeNames.append(tr("Street"));
  }
  if (supportedMapTypes.contains(QGraphicsGeoMap::SatelliteMapDay)) {
    m_mapControlTypes.append(QGraphicsGeoMap::SatelliteMapDay);
    mapTypeNames.append(tr("Satellite"));
  }

  if (supportedMapTypes.contains(QGraphicsGeoMap::SatelliteMapNight)) {
    m_mapControlTypes.append(QGraphicsGeoMap::SatelliteMapNight);
    mapTypeNames.append(tr("Satellite - Night"));
  }

  if (supportedMapTypes.contains(QGraphicsGeoMap::TerrainMap)) {
    m_mapControlTypes.append(QGraphicsGeoMap::TerrainMap);
    mapTypeNames.append(tr("Terrain"));
  }

  MComboBox* m_comboBox = new MComboBox;
  m_comboBox->addItems(mapTypeNames);

  m_comboBox->setIconVisible(false);
  m_comboBox->setTitle(tr("Map Type"));
  m_comboBox->setCurrentIndex(0);
  connect(m_comboBox, SIGNAL(activated(int)),
          this, SLOT(slotMapTypeChanged(int)));
  mapTypeAction->setWidget(m_comboBox);
  addAction(mapTypeAction);
}

void EventDetailsPage::slotMapTypeChanged(int selection)
{
  QGraphicsGeoMap::MapType mapType = m_mapControlTypes.at(selection);
  m_mapWidget->setMapType(mapType);
}

void EventDetailsPage::sliderValueChanged(int zoomLevel)
{
  m_mapWidget->setZoomLevel(zoomLevel);
}

void EventDetailsPage::slotChangeStar()
{
  m_starred = !m_starred;
  DBManager::instance(m_dbStorage)->setEventStarred(m_eventID, m_starred);
  updateStarredAction();
}

void EventDetailsPage::updateStarredAction()
{
  if (m_starred) {
    m_actionStar->setIconID("icon-m-toolbar-favorite-mark");
  } else {
    m_actionStar->setIconID("icon-m-toolbar-favorite-unmark");
  }
}
