#include "artistpage.h"

/* This file is part of rockmarble
  *
  * Copyright (C) 2011 Flavio Castelli <flavio@castelli.name>
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

#include "artistpage.h"

#include "artistitemcreator.h"
#include "artistmodel.h"
#include "dbmanager.h"
#include "countrypage.h"
#include "eventpage.h"
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

ArtistPage::ArtistPage(const QString& country, QGraphicsItem *parent)
    : MApplicationPage(parent), m_country(country)
{
  setTitle(tr("Artists playing in %1").arg(m_country));

  m_lastfm = new Lastfm(this);
  m_filterVisible = false;
  m_showArtistsWithoutEvents = true;
  m_pageMode = ARTISTS_BY_COUNTRY;
}

ArtistPage::ArtistPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{
  setTitle(tr("Artists"));

  m_lastfm = new Lastfm(this);
  m_filterVisible = false;
  m_showArtistsWithoutEvents = true;

  m_pageMode = ALL_ARTISTS;
}

ArtistPage::~ArtistPage()
{
}

void ArtistPage::createContent()
{
  MApplicationPage::createContent();

  QGraphicsWidget *panel = centralWidget();
  MLayout* layout = new MLayout(panel);
  m_policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
  layout->setAnimation(NULL);
  panel->setLayout(layout);
  layout->setLandscapePolicy(m_policy);
  layout->setPortraitPolicy(m_policy);

  if (m_pageMode == ALL_ARTISTS) {
    // Menu Actions
    MAction* actionImportLastfm = new MAction(panel);
    actionImportLastfm->setText(tr("Import from Last.fm"));
    actionImportLastfm->setLocation(MAction::ApplicationMenuLocation);
    addAction(actionImportLastfm);
    connect(actionImportLastfm, SIGNAL(triggered()), this, SLOT(slotImportLastfm()));

    MAction* actionAddArtist = new MAction(panel);
    actionAddArtist->setText(tr("Add artist"));
    actionAddArtist->setLocation(MAction::ApplicationMenuLocation);
    addAction(actionAddArtist);
    connect(actionAddArtist, SIGNAL(triggered()), this, SLOT(slotAddArtist()));

    // Toolbar Actions
    MAction* actionFilter = new MAction("icon-m-toolbar-filter", "", this);
    actionFilter->setLocation(MAction::ToolBarLocation);
    addAction(actionFilter);
    connect(actionFilter, SIGNAL(triggered()), this, SLOT(slotShowFilter()));
  }

  MAction* actionSearch = new MAction("icon-m-toolbar-search", "", this);
  actionSearch->setLocation(MAction::ToolBarLocation);
  addAction(actionSearch);
  connect(actionSearch, SIGNAL(triggered()), this, SLOT(slotShowSearch()));

  // setup model
  m_artistsModel = new ArtistModel(artistsModelQuery());

  // filtering text box
  QGraphicsLinearLayout *containerLayout = new QGraphicsLinearLayout(Qt::Horizontal);

  MLabel* filterLabel = new MLabel(tr("Filter artist:"));
  containerLayout->addItem(filterLabel);

  m_filter = new MTextEdit(MTextEditModel::SingleLine, QString());
  containerLayout->addItem(m_filter);
  m_filter->setObjectName("CommonSingleInputField");
  connect(m_filter, SIGNAL(textChanged()), this, SLOT(slotFilterChanged()));

  m_filterWidget = new MWidget();
  m_filterWidget->setLayout(containerLayout);

  // No artist found label
  m_noArtistLabel = new MLabel(tr("No artist available."));
  m_noArtistLabel->setAlignment(Qt::AlignCenter);
  if (m_artistsModel->rowCount() == 0)
    m_policy->addItem(m_noArtistLabel);

  // MList with fast view
  MList* artistsList = new MList();
  artistsList->setSelectionMode(MList::SingleSelection);

  // Content item creator and item model for the list
  artistsList->setCellCreator(new ArtistItemCreator(m_pageMode, m_country));
  artistsList->setItemModel(m_artistsModel);
  m_policy->addItem(artistsList);

  connect(artistsList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotArtistClicked(QModelIndex)));
  connect(DBManager::instance(), SIGNAL(artistAdded(int,bool)),
           this, SLOT(slotArtistAdded(int,bool)));
}

QSqlQuery ArtistPage::artistsModelQuery()
{
  QSqlQuery query;
  QString q;
  if (m_pageMode == ALL_ARTISTS) {
    q = "SELECT distinct artists.id FROM artists ";
    if (!m_showArtistsWithoutEvents) {
      q += " JOIN artists_events on artists.id = artists_events.artist_id ";
    }
    q += " WHERE artists.favourite = 1 ";
    if (m_filterVisible && !m_filter->text().isEmpty()) {
      q += " AND artists.name LIKE ? ";
    }
    q += " ORDER BY artists.name ASC";
    query.prepare(q);
    if (m_filterVisible && !m_filter->text().isEmpty()) {
      QString name('%');
      name += m_filter->text() + '%';
      query.addBindValue(name);
    }
  } else if (m_pageMode == ARTISTS_BY_COUNTRY) {
    q = "SELECT distinct artists.id FROM artists ";
    q += " JOIN artists_events on artists.id = artists_events.artist_id ";
    q += " JOIN events on events.id = artists_events.event_id ";
    q += " JOIN locations on locations.id = events.location_id ";
    q += " WHERE artists.favourite = 1 ";
    q += " AND locations.country = ? ";
    if (m_filterVisible && !m_filter->text().isEmpty()) {
      q += " AND artists.name LIKE ? ";
    }
    q += " ORDER BY artists.name ASC";
    query.prepare(q);
    query.addBindValue(m_country);
    if (m_filterVisible && !m_filter->text().isEmpty()) {
      QString name('%');
      name += m_filter->text() + '%';
      query.addBindValue(name);
    }
  }

  query.exec();
  return query;
}

void ArtistPage::slotArtistAdded(const int artistID, bool favourite)
{
  if (favourite) {
    m_policy->removeItem(m_noArtistLabel);

    QString artist = DBManager::instance()->artistNameFromID(artistID);
    m_lastfm->getArtistImage(artist);
    if (!m_manuallyAddedArtists.contains(artist,Qt::CaseInsensitive))
      m_lastfm->getEventsForArtist(artist);
  }
}

void ArtistPage::slotRefreshArtistsModel()
{
  m_artistsModel->setQuery(artistsModelQuery());
}

void ArtistPage::slotFilterChanged()
{
  m_artistsModel->setQuery(artistsModelQuery());
}

void ArtistPage::slotShowSearch()
{
  qreal startValue, endValue;
  if (m_filterVisible) {
    // let's hide it
    startValue = 1.0;
    endValue = 0.0;
  } else {
    // let's show it
    m_filterWidget->setOpacity(0.0); // ensure it's not visible
    m_policy->insertItem(0, m_filterWidget, Qt::AlignCenter);
    startValue = 0.0;
    endValue = 1.0;
  }

  QPropertyAnimation *fadeInAnimation = new QPropertyAnimation;
  fadeInAnimation->setTargetObject(m_filterWidget);
  fadeInAnimation->setPropertyName("opacity");
  fadeInAnimation->setStartValue(startValue);
  fadeInAnimation->setEndValue(endValue);
  fadeInAnimation->setDuration(1000.0);
  fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
  connect(fadeInAnimation, SIGNAL(finished()), this,
          SLOT(slotFilterAnimationFinished()));

  m_filterVisible = !m_filterVisible;
}

void ArtistPage::slotFilterAnimationFinished()
{
  if (!m_filterVisible) {
    m_policy->removeItem(m_filterWidget);
  }
}

void ArtistPage::slotShowFilter()
{
  MWidget *centralWidget = new MWidget;
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);

  MLabel *label = new MLabel(tr("Show artists without tour dates."),
                             centralWidget);
  label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  MButton *artistsWithoutEventsSwitch = new MButton();
  artistsWithoutEventsSwitch->setViewType(MButton::switchType);
  artistsWithoutEventsSwitch->setCheckable(true);
  artistsWithoutEventsSwitch->setChecked(m_showArtistsWithoutEvents);

  centralWidget->setLayout(layout);

  layout->addItem(label);
  layout->addItem(artistsWithoutEventsSwitch);

  MDialog* dialog = new MDialog(tr("Import from Last.fm"),
                                M::OkButton | M::CancelButton);
  dialog->setCentralWidget(centralWidget);
  dialog->appear(MSceneWindow::DestroyWhenDone);
  if ((dialog->exec() == MDialog::Accepted) &&
      (m_showArtistsWithoutEvents != artistsWithoutEventsSwitch->isChecked())) {
    m_showArtistsWithoutEvents = !m_showArtistsWithoutEvents;
    slotRefreshArtistsModel();
  }
}

void ArtistPage::showMessage(const QString &message, bool error)
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

void ArtistPage::slotAddArtist() {

  MWidget *centralWidget = new MWidget;
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);

  MLabel *label = new MLabel(tr("Fetch tour dates of"), centralWidget);
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

  MDialog* dialog = new MDialog(tr("Add artist"),
                                M::OkButton | M::CancelButton);
  dialog->setCentralWidget(centralWidget);
  dialog->appear(MSceneWindow::DestroyWhenDone);
  if (dialog->exec() == MDialog::Accepted)
  {
    QString artist = textEdit->text();
    if (!artist.isEmpty()) {
      m_manuallyAddedArtists << artist;
      m_lastfm->getEventsForArtist(artist);
      showMessage(tr("Retrieving event dates for %1").arg(artist));
    }

  }
}

void ArtistPage::slotImportLastfm() {
  MWidget *centralWidget = new MWidget;
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);

  MLabel *label = new MLabel(tr("Last.fm username"), centralWidget);
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

  MDialog* dialog = new MDialog(tr("Import from Last.fm"),
                                M::OkButton | M::CancelButton);
  dialog->setCentralWidget(centralWidget);
  dialog->appear(MSceneWindow::DestroyWhenDone);
  if (dialog->exec() == MDialog::Accepted)
  {
    QString user = textEdit->text();
    if (!user.isEmpty())
      m_lastfm->getTopArtists(user);
      showMessage(tr("Retrieving top artists for user %1").arg(user));
  }
}

void ArtistPage::slotArtistClicked(const QModelIndex& index)
{
  int artistID = index.data(Qt::DisplayRole).toInt();
  MApplicationPage* page;
  if (m_pageMode == ALL_ARTISTS)
    page = new CountryPage(artistID);
  else
    page = new EventPage(artistID, m_country);
  page->appear(MSceneWindow::DestroyWhenDismissed);
}
