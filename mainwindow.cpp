#include "mainwindow.h"

#include "artist.h"
#include "datafetcher.h"
#include "eventmodel.h"

#include <MarbleMap.h>
#include <MarbleModel.h>
#include <QAbstractItemModel>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QStatusBar>

#include <QDebug>

using namespace Marble;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setupUi(this);

  marble->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
  marble->setShowGps(true);
  marble->setDownloadUrl( "http://download.kde.org/apps/marble/" );

  connect( addArtist, SIGNAL(clicked()), this, SLOT(slotAddArtist()));

  m_df = DataFetcher::instance();
  connect (m_df, SIGNAL(getArtistEventsReady(QVariant,bool,QString)), this, SLOT(slotArtistEventsReady(QVariant,bool,QString)));

  connect(artistList, SIGNAL(currentRowChanged(int)), this, SLOT(slotCurrentArtistRowChanged(int)));
}

MainWindow::~MainWindow()
{
  foreach (EventModel* model, m_artists)
    delete model;
  m_artists.clear();
}

void MainWindow::slotAddArtist() {
  bool ok;
  QString artist = QInputDialog::getText(this, tr("rockmarble - add a new artist"),
                                          tr("Artist name:"), QLineEdit::Normal,
                                          "", &ok);
 if (ok && !artist.isEmpty()) {
    m_df->getArtistEvents(artist);
    m_statusBar->showMessage(tr("retrieving event dates for %1").arg(artist));
  }
}

void MainWindow::slotCurrentArtistRowChanged(int row)
{
  QString artist = artistList->item(row)->text();
  eventsBox->setTitle(tr("%1 tour dates").arg(artist));
  eventTable->setModel(m_artists[artist]);
  connect(eventTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(slotCurrentEventChanged(QModelIndex,QModelIndex)));
  eventTable->update();
}

void MainWindow::slotCurrentEventChanged(const QModelIndex & current, const QModelIndex & previous ) {
  Q_UNUSED( previous)
  EventModel* model = static_cast<EventModel*>(eventTable->model());

  qreal latitude, longitude;

  if (model->getCoordinates(current, &latitude, &longitude)) {
    marble->centerOn(longitude, latitude, true);
    marble->update();
  }
}

void MainWindow::slotArtistEventsReady(QVariant data, bool successfull, QString error) {
  if (successfull) {
    Artist* artist = new Artist (data);

    QMap<QString,EventModel*>::iterator match = m_artists.find(artist->name());
    if (match != m_artists.end()) {
      EventModel* model = match.value();
      delete model;
      m_artists[artist->name()] = new EventModel(artist);
    } else {
      m_artists.insert(artist->name(), new EventModel(artist));
      new QListWidgetItem(artist->name(), artistList);
    }
    m_statusBar->showMessage(tr("Event dates for %1 successfully retrieved").arg(artist->name()), 1200);
  } else {
    m_statusBar->showMessage(error);
  }
}

