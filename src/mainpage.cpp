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
#include "dbmanager.h"
#include "countrypage.h"
#include "event.h"
#include "eventmodel.h"
#include "eventsortfilterproxymodel.h"
#include "lastfm.h"

#include <MAction>
#include <MBanner>
#include <MButton>
#include <MDialog>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MList>
#include <MSceneManager>
#include <MTextEdit>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QSortFilterProxyModel>
#include <QtSql/QSqlQuery>
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

  m_lastfm = new Lastfm(this);
  m_filterVisible = false;
  m_showArtistsWithoutEvents = true;
}

MainPage::~MainPage()
{
}

void MainPage::createContent()
{
  MApplicationPage::createContent();

  QGraphicsWidget *panel = centralWidget();
  MLayout *layout = new MLayout(panel);
  layout->setAnimation(NULL);
  panel->setLayout(layout);
  MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
  layout->setLandscapePolicy(policy);
  layout->setPortraitPolicy(policy);

  // Menu Actions
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

  // Toolbar Actions
  MAction* actionFilter = new MAction("icon-m-toolbar-filter", "", this);
  actionFilter->setLocation(MAction::ToolBarLocation);
  addAction(actionFilter);
  connect(actionFilter, SIGNAL(triggered()), this, SLOT(slotShowFilter()));

  MAction* actionSearch = new MAction("icon-m-toolbar-search", "", this);
  actionSearch->setLocation(MAction::ToolBarLocation);
  addAction(actionSearch);
  connect(actionSearch, SIGNAL(triggered()), this, SLOT(slotShowSearch()));

  // filtering text box
  m_filter = new MTextEdit(MTextEditModel::SingleLine, QString(), this);
  m_filter->setOpacity(0.0);
  m_filter->setObjectName("CommonSingleInputField");
  m_filter->setToolTip("Foobar");
  policy->addItem(m_filter);
  connect(m_filter, SIGNAL(textChanged()), this, SLOT(slotFilterChanged()));

  // MList with fast view
  MList* artistList = new MList();
  artistList->setSelectionMode(MList::SingleSelection);

  // Content item creator and item model for the list
  ArtistItemCreator *cellCreator = new ArtistItemCreator();
  artistList->setCellCreator(cellCreator);
  m_artistsModel = new QSqlQueryModel();
  m_artistsModel->setQuery(artistsModelQuery());
  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setSourceModel(m_artistsModel);
  artistList->setItemModel(m_proxyModel);
  policy->addItem(artistList);

  connect (artistList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotArtistClicked(QModelIndex)));

  connect(DBManager::instance(), SIGNAL(artistAdded(const QString&, bool)),
          this, SLOT(slotArtistAdded(const QString&, bool)));

}

QString MainPage::artistsModelQuery() const
{
  QString q = "SELECT distinct artists.name FROM artists ";
  if (!m_showArtistsWithoutEvents) {
    q += " JOIN artists_events on artists.id = artists_events.artist_id ";
  }
  q += " WHERE artists.favourite = 1 ";
  q += " ORDER BY artists.name ASC";
  return q;
}

void MainPage::slotArtistAdded(const QString& artist, bool favourite)
{
  if (favourite) {
    m_lastfm->getArtistImage(artist);
    if (!m_manuallyAddedArtists.contains(artist,Qt::CaseInsensitive))
      m_lastfm->getEventsForArtist(artist);
    refreshArtistsModel();
  }
}

void MainPage::refreshArtistsModel()
{
  m_artistsModel->setQuery(artistsModelQuery());
}

void MainPage::slotFilterChanged()
{
  m_proxyModel->setFilterRegExp(m_filter->text());
}

void MainPage::slotShowSearch()
{
  qreal startValue, endValue;
  if (m_filterVisible) {
    // let's hide it
    startValue = 1.0;
    endValue = 0.0;
  } else {
    // let's show it
    startValue = 0.0;
    endValue = 1.0;
  }
  m_filterVisible = !m_filterVisible;

  QPropertyAnimation *fadeInAnimation = new QPropertyAnimation;
  fadeInAnimation->setTargetObject(m_filter);
  fadeInAnimation->setPropertyName("opacity");
  fadeInAnimation->setStartValue(startValue);
  fadeInAnimation->setEndValue(endValue);
  fadeInAnimation->setDuration(1000.0);
  fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainPage::slotShowFilter()
{
  MWidget *centralWidget = new MWidget;
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);

  MLabel *label = new MLabel("Show artists without tour dates.",
                             centralWidget);
  label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  MButton *artistsWithoutEventsSwitch = new MButton();
  artistsWithoutEventsSwitch->setViewType(MButton::switchType);
  artistsWithoutEventsSwitch->setCheckable(true);
  artistsWithoutEventsSwitch->setChecked(m_showArtistsWithoutEvents);

  centralWidget->setLayout(layout);

  layout->addItem(label);
  layout->addItem(artistsWithoutEventsSwitch);

  MDialog* dialog = new MDialog("Import from Last.fm",
                                M::OkButton | M::CancelButton);
  dialog->setCentralWidget(centralWidget);
  dialog->appear(MSceneWindow::DestroyWhenDone);
  if ((dialog->exec() == MDialog::Accepted) &&
      (m_showArtistsWithoutEvents != artistsWithoutEventsSwitch->isChecked())) {
    m_showArtistsWithoutEvents = !m_showArtistsWithoutEvents;
    refreshArtistsModel();
  }
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
    if (!artist.isEmpty()) {
      m_manuallyAddedArtists << artist;
      m_lastfm->getEventsForArtist(artist);
      showMessage(QString("retrieving event dates for %1").arg(artist));
    }

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
