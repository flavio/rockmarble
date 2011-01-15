/* This file is part of rockmarble
  *
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2  of  the  License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be  useful, but
  * WITHOUT  ANY  WARRANTY;  without  even  the   implied   warranty  of
  * MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  GNU
  * General Public License for more details.
  *
  * You should have received a copy of the GNU  General  Public  License
  * along with  this  program;  if  not,  write  to  the  Free  Software
  * Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
  */

#include "mainpage.h"

#include "defines.h"
#include "artistitemcreator.h"
#include "countrypage.h"
#include "event.h"
#include "eventmodel.h"
#include "eventsortfilterproxymodel.h"
#include "lastfm.h"

#include <MAction>
#include <MBanner>
#include <MDialog>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MList>
#include <MSceneManager>
#include <MTextEdit>

#include <QtCore/QAbstractItemModel>
#include <QtGui/QGraphicsLinearLayout>
#include <QtSql/QSqlQueryModel>


//#include <marble/MarbleMap.h>
//#include <marble/MarbleModel.h>
//using namespace Marble;

MainPage::MainPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{
  setTitle("Rockmarble");
//  marble->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
//  marble->setShowGps(true);
//
//  eventTable->setSortingEnabled(true);

  // filter
//  filterComboBox->addItem(tr("Country"), EventModel::CountryColumn);
//  filterComboBox->addItem(tr("City"), EventModel::CityColumn);
//  filterComboBox->addItem(tr("Location"), EventModel::LocationColumn);
//  filterComboBox->addItem(tr("Headliner"), EventModel::HeadlinerColumn);
//  filterComboBox->addItem(tr("Artists"), EventModel::ArtistsColumn);

  // last filtering rules
//  m_lastArtistFilterRule = EventModel::CountryColumn;
//  m_lastCityFilterRule = EventModel::HeadlinerColumn;

  // gui signals
//  connect(addArtistBtn, SIGNAL(clicked()), this, SLOT(slotAddArtist()));
//  connect(importLastfmBtn, SIGNAL(clicked()), this, SLOT(slotImportLastfm()));
//  connect(addCityBtn, SIGNAL(clicked()), this, SLOT(slotAddCity()));
//
//  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)));
//  connect(artistList, SIGNAL(currentRowChanged(int)), this, SLOT(slotCurrentArtistRowChanged(int)));
//  connect(citiesList, SIGNAL(currentRowChanged(int)), this, SLOT(slotCurrentCityRowChanged(int)));
//  connect(filterEdit, SIGNAL(textChanged(QString)), this, SLOT(slotFilterTextChanged(QString)));
//  connect(filterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotFilterIndexChanged()));
//  connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

  m_lastfm = new Lastfm(this);
}

MainPage::~MainPage()
{
  foreach (Event* event, m_artists.values())
    delete event;
  m_artists.clear();

  foreach (Event* event, m_cities.values())
    delete event;
  m_cities.clear();
}

void MainPage::createContent()
{
  MApplicationPage::createContent();

  QGraphicsWidget *panel = centralWidget();
  MLayout *layout = new MLayout(panel);
  layout->setAnimation(NULL);
  panel->setLayout(layout);
  MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

  // Actions
  MAction* actionImportLastfm = new MAction(panel);
  actionImportLastfm->setText("Import from Last.fm");
  actionImportLastfm->setLocation(MAction::ApplicationMenuLocation);
  addAction(actionImportLastfm);
  connect(actionImportLastfm, SIGNAL(triggered()), this, SLOT(slotImportLastfm()));

  MAction* actionAddArtist = new MAction(panel);
  actionAddArtist->setText("Add artist");
  actionAddArtist->setLocation(MAction::ApplicationMenuLocation);
  addAction(actionAddArtist);
  connect(actionAddArtist, SIGNAL(triggered()), this, SLOT(slotAddArtist()));

  MAction* actionAbout = new MAction(panel);
  actionAbout->setText("About");
  actionAbout->setLocation(MAction::ApplicationMenuLocation);
  addAction(actionAbout);
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));

  // MList with fast view
  MList* artistList = new MList();
  artistList->setSelectionMode(MList::SingleSelection);

  // Content item creator and item model for the list
  ArtistItemCreator *cellCreator = new ArtistItemCreator();
  artistList->setCellCreator(cellCreator);
  QSqlQueryModel* artistsModel = new QSqlQueryModel();
  artistsModel->setQuery("SELECT name FROM artists WHERE favourite = 1 "
                         "ORDER BY name ASC");
  artistList->setItemModel(artistsModel);
  policy->addItem(artistList);

  connect (artistList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotArtistClicked(QModelIndex)));
}

void MainPage::showMessage(const QString &message, bool error)
{
  MBanner* banner = new MBanner();
  banner->setObjectName("SystemBanner");
  if (error) {
    banner->setIconID("icon-m-common-error");
    //banner->setObjectName("InformationBanner");
  }

  banner->setTitle(message);
  banner->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::slotAbout()
{
  MWidget *centralWidget = new MWidget;
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);

  MLabel *title = new MLabel(QString("rockmarble version %1").arg(ROCKMARBLE_VERSION),
                             centralWidget);
  MLabel *description = new MLabel("Flavio Castelli <flavio@castelli.name>",
                             centralWidget);

  title->setObjectName("CommonTitleInverted");
  MLabel *spacer = new MLabel();
  spacer->setObjectName("CommonSpacer");

  centralWidget->setLayout(layout);

  layout->addItem(title);
  layout->addItem(spacer);
  layout->addItem(description);

  MDialog* dialog = new MDialog("About rockmarble", M::OkButton);
  dialog->setCentralWidget(centralWidget);
  dialog->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::slotAddArtist() {

  MWidget *centralWidget = new MWidget;
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);

  MLabel *label = new MLabel("Fetch tour dates of", centralWidget);
  label->setObjectName("CommonTitleInverted");
  MTextEdit *textEdit = new MTextEdit(MTextEditModel::SingleLine,
                                      QString(), centralWidget);
  textEdit->setObjectName("CommonSingleInputField");
  MLabel *spacer = new MLabel();
  spacer->setObjectName("CommonSpacer");

  centralWidget->setLayout(layout);

  layout->addItem(label);
  layout->addItem(textEdit);
  layout->addItem(spacer);

  MDialog* dialog = new MDialog("Add artist",
                                M::OkButton | M::CancelButton);
  dialog->setCentralWidget(centralWidget);
  dialog->appear(MSceneWindow::DestroyWhenDone);
  if (dialog->exec() == MDialog::Accepted)
  {
    QString artist = textEdit->text();
    if (!artist.isEmpty())
      addArtist(artist);
  }
}

//void MainPage::slotAddCity() {
//  bool ok;
//  QString city = QInputDialog::getText(this, tr("Find events near"),
//                                          tr("City name:"), QLineEdit::Normal,
//                                          "", &ok);
//  if (ok && !city.isEmpty()) {
//    addCity(city);
//  }
//}

void MainPage::addArtist(const QString& artist)
{
  if (!artist.isEmpty()) {
//    m_df->getArtistEvents(artist);
    //showMessage(QString("retrieving event dates for %1").arg(artist));
  }
}

void MainPage::slotImportLastfm() {
  MWidget *centralWidget = new MWidget;
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);

  MLabel *label = new MLabel("Last.fm username", centralWidget);
  label->setObjectName("CommonTitleInverted");
  MTextEdit *textEdit = new MTextEdit(MTextEditModel::SingleLine,
                                          QString(),
                                          centralWidget);
  textEdit->setObjectName("CommonSingleInputField");
  MLabel *spacer = new MLabel();
  spacer->setObjectName("CommonSpacer");

  centralWidget->setLayout(layout);

  layout->addItem(label);
  layout->addItem(textEdit);
  layout->addItem(spacer);

  MDialog* dialog = new MDialog("Import from Last.fm",
                                M::OkButton | M::CancelButton);
  dialog->setCentralWidget(centralWidget);
  dialog->appear(MSceneWindow::DestroyWhenDone);
  if (dialog->exec() == MDialog::Accepted)
  {
    QString user = textEdit->text();
    if (!user.isEmpty())
      m_lastfm->getTopArtists(user);
      showMessage(QString("Retrieving top artists for user %1").arg(user));
  }
}

void MainPage::addCity(const QString& city)
{
  if (!city.isEmpty()) {
//    m_df->getEventsNearLocation(city);
//    m_statusBar->showMessage(tr("retrieving event near %1").arg(city));
  }
}

void MainPage::slotArtistClicked(const QModelIndex& index)
{
  QString artist= index.data(Qt::DisplayRole).toString();
  CountryPage *countryPage = new CountryPage(artist);
  countryPage->appear(MSceneWindow::DestroyWhenDismissed);
}

//void MainPage::slotFilterTextChanged(const QString& text)
//{
//  EventSortFilterProxyModel* proxyModel = static_cast<EventSortFilterProxyModel*>( eventTable->model());
//  proxyModel->setFilterFixedString(text);
//  proxyModel->setFilterKeyColumn(filterComboBox->itemData( filterComboBox->currentIndex()).toInt());
//  proxyModel->setFilterCaseSensitivity ( Qt::CaseInsensitive );
//
//  QAbstractItemModel* sourceModel = proxyModel->sourceModel();
//
//  totalLabel->setText(tr("%1/%2 events").arg(proxyModel->rowCount()).arg(sourceModel->rowCount()));
//}
//
//void MainPage::slotFilterIndexChanged()
//{
//  slotFilterTextChanged(filterEdit->text());
//}
//
//void MainPage::slotCurrentTabChanged(int index)
//{
//  qDebug() << artistList->currentRow();
//  qDebug() << citiesList->currentRow();
//
//  if (index == 0) {
//    // artist tab
//    if (citiesList->currentRow() >= 0) {
//      m_lastCityFilterRule = filterComboBox->itemData(filterComboBox->currentIndex()).toInt();
//      m_lastCityFilterText = filterEdit->text();
//      m_lastCityTableItem = eventTable->currentIndex();
//    }
//
//    if (artistList->currentRow() >= 0) {
//      slotCurrentArtistRowChanged(artistList->currentRow());
//      eventTable->setCurrentIndex(m_lastArtistTableItem);
//      slotCurrentEventChanged(m_lastArtistTableItem, QModelIndex());
//    } else {
//      stackedWidget->setCurrentIndex(1);
//      eventsBox->setTitle(tr("Tour dates"));
//    }
//  } else if (index == 1) {
//    // cities tab
//
//    if (artistList->currentRow() >= 0) {
//      m_lastArtistFilterRule = filterComboBox->itemData(filterComboBox->currentIndex()).toInt();
//      m_lastArtistFilterText = filterEdit->text();
//      m_lastArtistTableItem = eventTable->currentIndex();
//    }
//
//    if (citiesList->currentRow() >= 0) {
//      slotCurrentCityRowChanged(citiesList->currentRow());
//      eventTable->setCurrentIndex(m_lastCityTableItem);
//      slotCurrentEventChanged(m_lastCityTableItem, QModelIndex());
//    } else {
//      stackedWidget->setCurrentIndex(1);
//      eventsBox->setTitle(tr("Tour dates"));
//    }
//  }
//}
//
//
//void MainPage::slotCurrentArtistRowChanged(int row)
//{
//  QString artist = artistList->item(row)->text();
//  eventsBox->setTitle(tr("%1 tour dates").arg(artist));
//
//  EventList events = m_artists.values(artist);
//  EventModel* sourceModel = new EventModel(events, this);
//
//  if (sourceModel->rowCount() != 0) {
//    stackedWidget->setCurrentIndex(0);
//    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
//
//    proxyModel->setSourceModel(sourceModel);
//    eventTable->setModel(proxyModel);
//    eventTable->hideColumn(EventModel::HeadlinerColumn);
//    filterComboBox->removeItem( filterComboBox->findData(EventModel::HeadlinerColumn));
//
//    filterEdit->setText(m_lastArtistFilterText);
//    filterComboBox->setCurrentIndex( filterComboBox->findData(m_lastArtistFilterRule));
//    slotFilterTextChanged(filterEdit->text());
//
//    connect(eventTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(slotCurrentEventChanged(QModelIndex,QModelIndex)));
//
//    totalLabel->setText(tr("%1 events").arg(proxyModel->rowCount()));
//  } else {
//    stackedWidget->setCurrentIndex(1);
//  }
//  eventTable->update();
//}
//
//void MainPage::slotCurrentCityRowChanged(int row)
//{
//  QString city = citiesList->item(row)->text();
//  eventsBox->setTitle(tr("Events near %1").arg(city));
//
//  EventList events = m_cities.values(city);
//  EventModel* sourceModel = new EventModel(events, this);
//
//  if (sourceModel->rowCount() != 0) {
//    stackedWidget->setCurrentIndex(0);
//    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
//
//    proxyModel->setSourceModel(sourceModel);
//    eventTable->setModel(proxyModel);
//    eventTable->showColumn(EventModel::HeadlinerColumn);
//    if (filterComboBox->findData(EventModel::HeadlinerColumn) == -1)
//      filterComboBox->addItem(tr("Headliner"), EventModel::HeadlinerColumn);
//
//    filterEdit->setText(m_lastCityFilterText);
//    filterComboBox->setCurrentIndex( filterComboBox->findData(m_lastCityFilterRule));
//    slotFilterTextChanged(filterEdit->text());
//
//    connect(eventTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(slotCurrentEventChanged(QModelIndex,QModelIndex)));
//
//    totalLabel->setText(tr("%1 events").arg(proxyModel->rowCount()));
//  } else {
//    stackedWidget->setCurrentIndex(1);
//  }
//  eventTable->update();
//}
//
//
//void MainPage::slotCurrentEventChanged(const QModelIndex & current, const QModelIndex & previous ) {
//  Q_UNUSED( previous)
//  QSortFilterProxyModel* proxyModel = static_cast<QSortFilterProxyModel*>(eventTable->model());
//  EventModel* model = static_cast<EventModel*>(proxyModel->sourceModel());
//
//  qreal latitude, longitude;
//
//  if (model->getCoordinates(proxyModel->mapToSource(current), &latitude, &longitude)) {
//    marble->centerOn(longitude, latitude, true);
//    marble->update();
//  }
//}
