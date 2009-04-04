/* This file is part of rockmarble
  *
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public License
  * along with this library; see the file COPYING.LIB.  If not, write to
  * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  * Boston, MA 02110-1301, USA.
  */

#include "mainwindow.h"

#include "artist.h"
#include "datafetcher.h"
#include "defines.h"
#include "eventmodel.h"
#include "eventsortfilterproxymodel.h"

#include <jsonconverterthread.h>

#include <QAbstractItemModel>
#include <MarbleMap.h>
#include <MarbleModel.h>
#include <QMessageBox>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QStatusBar>

using namespace Marble;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setupUi(this);

  marble->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
  marble->setShowGps(true);
  marble->setDownloadUrl( "http://download.kde.org/apps/marble/" );

  eventTable->setSortingEnabled(true);

  // data fetcher
  m_df = DataFetcher::instance();
  connect (m_df, SIGNAL(getArtistEventsReady(QString,bool,QString)), this, SLOT(slotArtistEventsReady(QString,bool,QString)));
  connect (m_df, SIGNAL(getTopArtistsReady(QString,bool,QString)), this, SLOT(slotTopArtistsReady(QString,bool,QString)));
  connect (m_df, SIGNAL(getEventsNearLocationReady(QString,bool,QString)), this, SLOT (slotEventsNearLocationReady(QString,bool,QString)));

  // gui signals
  connect(addArtistBtn, SIGNAL(clicked()), this, SLOT(slotAddArtist()));
  connect(importLastfmBtn, SIGNAL(clicked()), this, SLOT(slotImportLastfm()));
  connect(addCityBtn, SIGNAL(clicked()), this, SLOT(slotAddCity()));

  connect(artistList, SIGNAL(currentRowChanged(int)), this, SLOT(slotCurrentArtistRowChanged(int)));
  connect(citiesList, SIGNAL(currentRowChanged(int)), this, SLOT(slotCurrentCityRowChanged(int)));
  connect(filterEdit, SIGNAL(textChanged(QString)), this, SLOT(slotFilterTextChanged(QString)));
  connect(filterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotFilterIndexChanged()));
  connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
  connect(action_about, SIGNAL(triggered()), this, SLOT(slotAbout()));
}

MainWindow::~MainWindow()
{
  foreach (EventModel* model, m_artists)
    delete model;
  m_artists.clear();
}

void MainWindow::slotAbout()
{
  QMessageBox::about(this, tr("About rockmarble"), tr("rockmarble version %1\n\nSimple program for tracking tour dates of your favourite artists.\n\nFlavio Castelli <flavio@castelli.name>").arg(ROCKMARBLE_VERSION));
}

void MainWindow::slotAddArtist() {
  bool ok;
  QString artist = QInputDialog::getText(this, tr("Add a new artist"),
                                          tr("Artist name:"), QLineEdit::Normal,
                                          "", &ok);
  if (ok && !artist.isEmpty()) {
    addArtist(artist);
  }
}

void MainWindow::slotAddCity() {
  bool ok;
  QString city = QInputDialog::getText(this, tr("Find events near"),
                                          tr("City name:"), QLineEdit::Normal,
                                          "", &ok);
  if (ok && !city.isEmpty()) {
    addCity(city);
  }
}


void MainWindow::addArtist(const QString& artist)
{
  if (!artist.isEmpty()) {
    m_df->getArtistEvents(artist);
    m_statusBar->showMessage(tr("retrieving event dates for %1").arg(artist));
  }
}

void MainWindow::slotImportLastfm() {
  bool ok;
  QString user = QInputDialog::getText(this, tr("Import last.fm top artists"),
                              tr("Your last.fm user name:"), QLineEdit::Normal,
                              "", &ok);

  if (ok && !user.isEmpty()) {
    m_df->getTopArtists(user);
    m_statusBar->showMessage(tr("retrieving top artists for user %1").arg(user));
  }
}

void MainWindow::addCity(const QString& city)
{
  if (!city.isEmpty()) {
    m_df->getEventsNearLocation(city);
    m_statusBar->showMessage(tr("retrieving event near %1").arg(city));
  }
}

void MainWindow::slotFilterTextChanged(const QString& text)
{
  EventSortFilterProxyModel* proxyModel = static_cast<EventSortFilterProxyModel*>( eventTable->model());
  proxyModel->setFilterFixedString(text);
  proxyModel->setFilterKeyColumn(filterComboBox->currentIndex());
  proxyModel->setFilterCaseSensitivity ( Qt::CaseInsensitive );
}

void MainWindow::slotFilterIndexChanged()
{
  slotFilterTextChanged(filterEdit->text());
}

void MainWindow::slotCurrentArtistRowChanged(int row)
{
  QString artist = artistList->item(row)->text();
  eventsBox->setTitle(tr("%1 tour dates").arg(artist));

  EventModel* sourceModel = m_artists[artist];

  if (sourceModel->rowCount() != 0) {
    stackedWidget->setCurrentIndex(0);
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel();

    proxyModel->setSourceModel(sourceModel);
    eventTable->setModel(proxyModel);

    slotFilterTextChanged(filterEdit->text());

    connect(eventTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(slotCurrentEventChanged(QModelIndex,QModelIndex)));
  } else {
    stackedWidget->setCurrentIndex(1);
  }
  eventTable->update();
}

void MainWindow::slotCurrentCityRowChanged(int row)
{
//  QString city = citiesList->item(row)->text();
//  eventsBox->setTitle(tr("%Events near %1").arg(city));
//
//  LocationModel* sourceModel = m_locations[city];
//
//  if (sourceModel->rowCount() != 0) {
//    stackedWidget->setCurrentIndex(0);
//    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel();
//
//    proxyModel->setSourceModel(sourceModel);
//    eventTable->setModel(proxyModel);
//
//    slotFilterTextChanged(filterEdit->text());
//
//    connect(eventTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(slotCurrentEventChanged(QModelIndex,QModelIndex)));
//  } else {
//    stackedWidget->setCurrentIndex(1);
//  }
//  eventTable->update();
}


void MainWindow::slotCurrentEventChanged(const QModelIndex & current, const QModelIndex & previous ) {
  Q_UNUSED( previous)
  QSortFilterProxyModel* proxyModel = static_cast<QSortFilterProxyModel*>(eventTable->model());
  EventModel* model = static_cast<EventModel*>(proxyModel->sourceModel());

  qreal latitude, longitude;

  if (model->getCoordinates(proxyModel->mapToSource(current), &latitude, &longitude)) {
    marble->centerOn(longitude, latitude, true);
    marble->update();
  }
}

void MainWindow::slotArtistEventsReady(QString data, bool successfull, QString error) {
  if (successfull) {
    JSonConverterThread* thread = new JSonConverterThread(data, this);
    connect(thread, SIGNAL(conversionFinished(QVariant, bool, QString)), this, SLOT(slotArtistEventConverted(QVariant, bool, QString)));
    thread->start();
  } else {
    m_statusBar->showMessage(error);
  }
}

void MainWindow::slotArtistEventConverted(QVariant data, bool successfull, QString error) {
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

void MainWindow::slotTopArtistsReady(QString data, bool successfull, QString error) {
  if (successfull) {
    JSonConverterThread* thread = new JSonConverterThread (data, this);
    connect(thread, SIGNAL(conversionFinished(QVariant, bool, QString)), this, SLOT(slotTopArtistConverted(QVariant, bool, QString)));
    thread->start();
  } else {
    m_statusBar->showMessage(error);
  }
}

void MainWindow::slotTopArtistConverted(QVariant data, bool successfull, QString error) {
  if (successfull) {
    QVariantMap response = data.toMap();
    if (!response.contains("error")) {
      QVariantMap topartists = response["topartists"].toMap();
      QVariantList artists = topartists["artist"].toList();

      foreach(QVariant entry, artists) {
        QVariantMap artist = entry.toMap();
        QString artistName = artist["name"].toString();
        addArtist(artistName);
      }
    }
    else {
      m_statusBar->showMessage(response["message"].toString());
    }
  } else {
    m_statusBar->showMessage(error);
  }
}

void MainWindow::slotEventsNearLocationReady(QString data, bool successfull, QString error) {
  if (successfull) {
    JSonConverterThread* thread = new JSonConverterThread(data, this);
    connect(thread, SIGNAL(conversionFinished(QVariant, bool, QString)), this, SLOT(slotEventsNearLocationConverted(QVariant,bool,QString)));
    thread->start();
  } else {
    m_statusBar->showMessage(error);
  }
}

void MainWindow::slotEventsNearLocationConverted(QVariant data, bool successfull, QString error) {
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


