#include "mainwindow.h"

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

void MainWindow::loadKdeDevelopers()
{
//  QString filename = "/home/flavio/marble/kde-devel-locations.kml";
  QString filename = "/home/flavio/marble/foo.kml";
  marble->addPlaceMarkFile(filename);
  marble->update();
}

