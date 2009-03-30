#include "mainwindow.h"

#include "artist.h"
#include "datafetcher.h"
#include "event.h"
#include "location.h"

#include <MarbleMap.h>
#include <MarbleModel.h>
#include <QAbstractItemModel>
#include <QListWidgetItem>
#include <QtGui/QTableWidgetItem>

#include <QDebug>

using namespace Marble;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setupUi(this);

  marble->setMapThemeId("earth/plain/plain.dgml");
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
  foreach (Artist* artist, m_artists)
    delete artist;
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
  setupEventTable(artist);
}

void MainWindow::setupEventTable(const QString& artistName)
{
  qDebug() << "setupEventTable for" << artistName;
  // country
  // city
  // location
  // date
  Artist* artist = m_artists[artistName];
  eventTable->clearContents();
  int row = 0;
  foreach(Event* event, artist->events()) {
    QTableWidgetItem* country = new QTableWidgetItem(event->location()->country());
    QTableWidgetItem* city = new QTableWidgetItem(event->location()->city());
    QTableWidgetItem* location = new QTableWidgetItem(QString("%1 - %2").arg(event->location()->name()).arg(event->location()->street()));
    QTableWidgetItem* date = new QTableWidgetItem(event->date().toString());

    eventTable->insertRow(row);

    eventTable->setItem(row, 0, country);
    eventTable->setItem(row, 1, city);
    eventTable->setItem(row, 2, location);
    eventTable->setItem(row, 3, date);
    eventTable->update();
    row++;
  }
}

void MainWindow::slotCurrentEventChanged(int currentRow, int currentColumn, int previousRow, int previousColumn) {

}

void MainWindow::slotArtistEventsReady(QVariant data, bool successfull, QString error) {
  if (successfull) {
    Artist* artist = new Artist (data);

    QMap<QString,Artist*>::iterator match = m_artists.find(artist->name());
    if (match != m_artists.end()) {
      Artist* a = match.value();
      delete a;
      m_artists[artist->name()] = artist;
    } else {
      m_artists.insert(artist->name(), artist);
      new QListWidgetItem(artist->name(), artistList);
    }

  } else {
    qDebug() << error;
  }
}

