#include "mainwindow.h"

#include "datafetcher.h"

#include <MarbleMap.h>
#include <MarbleModel.h>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

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
}

MainWindow::~MainWindow()
{
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

void MainWindow::slotArtistEventsReady(QVariant data, bool successfull, QString error) {
  if (successfull) {
    qDebug() << data;
  } else {
    qDebug() << error;
  }
}

