#include "mainwindow.h"

#include "artist.h"
#include "datafetcher.h"
#include "eventmodel.h"

#include <MarbleMap.h>
#include <MarbleModel.h>
#include <QAbstractItemModel>
#include <QListWidgetItem>
//#include <QtGui/QItemSelectionModel>

#include <QDebug>

using namespace Marble;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setupUi(this);

  marble->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
  marble->setShowGps(true);
  marble->setDownloadUrl( "http://download.kde.org/apps/marble/" );

  listView->setModel(marble->model()->placeMarkModel());

  connect( lineEdit, SIGNAL(returnPressed()), this, SLOT(search()));
  connect (action_load_kde_developer, SIGNAL(triggered()), this, SLOT(loadKdeDevelopers()));
  connect( artistEdit, SIGNAL(returnPressed()), this, SLOT(searchEvents()));

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

void MainWindow::search()
{
  QString text = lineEdit->text();
  MarbleModel* model = marble->model();
  QAbstractItemModel* itemModel = model->placeMarkModel();
  listView ->setModel(itemModel);
  QModelIndexList matches = itemModel-> match(itemModel->index(0,0), Qt::DisplayRole, QVariant(text));
  qDebug() << "MATCHES for" << text << ":" << matches.size();
  if (!matches.empty()) {
    QModelIndex match = matches[0];
    if (match.isValid()) {
      listView->setCurrentIndex(match);
      marble->map()->centerOn(match);
      marble->update();
    }
  }
}

void MainWindow::searchEvents() {
  QString artist = artistEdit->text();
  m_df->getArtistEvents(artist);
}

void MainWindow::loadKdeDevelopers()
{
//  QString filename = "/home/flavio/marble/kde-devel-locations.kml";
  QString filename = "/home/flavio/marble/foo.kml";
  marble->addPlaceMarkFile(filename);
  marble->update();
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
    marble->zoomView(500);
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
  } else {
    qDebug() << error;
  }
}

