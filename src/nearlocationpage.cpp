#include "nearlocationpage.h"

#include <MBanner>
#include <MButton>
#include <MComboBox>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>

#include <QGeoPositionInfoSource>
#ifdef FAKE_GPS
#include <QNmeaPositionInfoSource>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#endif

#include <QtGui/QGraphicsLinearLayout>

#include "artistpage.h"

NearLocationPage::NearLocationPage(QGraphicsItem *parent)
  : MApplicationPage(parent)
{
    m_trackingLocation = false;
    // force initialization of the memory db
    DBManager::instance(DBManager::MEMORY);
    setTitle(tr("Events near you"));
}

NearLocationPage::~NearLocationPage()
{
}

void NearLocationPage::createContent()
{
  QGraphicsWidget *panel = centralWidget();
  QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
  panel->setLayout(layout);

  m_kmBox = new MComboBox();
  QStringList kms;
  for (unsigned int i = 0; i <= 300; i+= 10)
    kms << QString("%1").arg(i);
  m_kmBox->addItems(kms);
  m_kmBox->setTitle(tr("Look for events within (km)"));
  m_kmBox->setCurrentIndex(0);
  layout->addItem(m_kmBox);

  m_btnSearchEvents = new MButton();
  m_btnSearchEvents->setText(tr("Search events"));
  m_btnSearchEvents->setEnabled(false);
  layout->addItem(m_btnSearchEvents);
  connect (m_btnSearchEvents, SIGNAL(clicked()),
           this, SLOT(slotSearchEvents()));

  QGeoPositionInfoSource* src = QGeoPositionInfoSource::createDefaultSource(this);
  if (!src) {
#ifdef FAKE_GPS
    QNmeaPositionInfoSource *nmeaLocation = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::SimulationMode, this);
    QFile *logFile = new QFile(QApplication::applicationDirPath()
                              + QDir::separator() + "nmealog.txt", this);
    nmeaLocation->setDevice(logFile);
    src = nmeaLocation;
    qWarning() << "No gps source available, using log file";
#else
    showMessage(tr("Cannot find a location provider."), true);
    return;
#endif
  }

  m_trackingLocation = true;
  connect(src, SIGNAL(positionUpdated(QGeoPositionInfo)),
          this, SLOT(updatePosition(QGeoPositionInfo)));
  connect(src, SIGNAL(updateTimeout()),
          this, SLOT(updatePositionTimeout()));
  src->requestUpdate(5000); // Start request for actual position
  showMessage(tr("Tracking location..."), false);
}

void NearLocationPage::updatePosition(const QGeoPositionInfo& pos)
{
  m_position = pos;
  showMessage(tr("Location found."), false);
  m_btnSearchEvents->setEnabled(true);
}

void NearLocationPage::updatePositionTimeout()
{
  showMessage(tr("Cannot find current location."), true);
}

void NearLocationPage::showMessage(const QString &message, bool error)
{
  MBanner* banner = new MBanner();
  banner->setObjectName("SystemBanner");
  if (error) {
    banner->setIconID("icon-m-common-error");
  }

  banner->setTitle(message);
  banner->appear(MSceneWindow::DestroyWhenDone);
}

void NearLocationPage::slotSearchEvents()
{
  MApplicationPage* page = new ArtistPage(m_position.coordinate().latitude(),
                                          m_position.coordinate().longitude(),
                                          m_kmBox->currentIndex()*10);
  page->appear(MSceneWindow::DestroyWhenDismissed);
}

