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

#include "artistpage.h"
#include "countrypage.h"
#include "eventpage.h"
#include "defines.h"
#include "mainpage.h"
#include "pageitemcreator.h"
#include "pagesmodel.h"
#include "nearlocationmainpage.h"
#include "nearlocationsearchpage.h"

#include <MAction>
#include <MDialog>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MList>
#include <MSceneManager>

#include <QtCore/QFile>
#include <QtGui/QGraphicsLinearLayout>

MainPage::MainPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{
  setTitle("Rockmarble");

  Page artistPage (ByArtist, tr("Artist"),
           QString("icon-m-content-artist"),
           tr("Search the events by artist and then by country."));
  m_pages << artistPage;

  Page locationPage (ByLocation, tr("Location"),
           QString("icon-m-clock-city-create"),
           tr("Search the events by country and then by artist."));
  m_pages << locationPage;

  Page currenLocationtPage (ByCurrentLocation, tr("Your current location"),
           QString("icon-m-common-location"),
           tr("Show all the events taking place near you."));
  m_pages << currenLocationtPage;

  Page starredPage (ByStar, tr("Starred"),
           QString("icon-m-common-favorite-mark"),
           tr("Show all the events you starred."));
  m_pages << starredPage;
}

void MainPage::slotAboutToQuit()
{
  DBManager::removeDatabase(DBManager::MEMORY);
}

void MainPage::createContent()
{
  QGraphicsWidget *panel = centralWidget();
  MLayout *layout = new MLayout(panel);
  layout->setAnimation(NULL);
  panel->setLayout(layout);
  MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
  layout->setLandscapePolicy(policy);
  layout->setPortraitPolicy(policy);

  // Menu Actions
  MAction* actionAbout = new MAction(panel);
  actionAbout->setText(tr("About"));
  actionAbout->setLocation(MAction::ApplicationMenuLocation);
  addAction(actionAbout);
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));

  // Label
  MLabel* label = new MLabel(tr("Find events by:"));

  // MList with fast view
  MList* pagesList = new MList();
  pagesList->setSelectionMode(MList::SingleSelection);

  // Content item creator and item model for the list
  PageItemCreator *cellCreator = new PageItemCreator();
  cellCreator->pages = m_pages;
  pagesList->setCellCreator(cellCreator);

  pagesList->setItemModel(new PagesModel(m_pages));
  policy->addItem(label);
  policy->addItem(pagesList);

  connect (pagesList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotItemClicked(QModelIndex)));
}

void MainPage::slotAbout()
{
  MWidget *centralWidget = new MWidget;
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);

  MLabel *title = new MLabel(tr("rockmarble version %1").arg(ROCKMARBLE_VERSION),
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

  MDialog* dialog = new MDialog(tr("About rockmarble"), M::OkButton);
  dialog->setCentralWidget(centralWidget);
  dialog->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::slotItemClicked(QModelIndex index)
{
  MApplicationPage* page;
  PageType pageType = (PageType) index.data(Qt::DisplayRole).toInt();

  switch (pageType) {
    case ByArtist:
      page = new ArtistPage(DBManager::DISK);
      break;
    case ByLocation:
      page = new CountryPage(DBManager::DISK);
      break;
  case ByStar:
    page = new EventPage(DBManager::DISK);
    break;
  case ByCurrentLocation: {
    if (!QFile::exists(DBManager::databaseFile(DBManager::MEMORY)) ||
        DBManager::instance(DBManager::MEMORY)->isDatabaseEmpty()) {
      // this is not the first time the 'current location' is selected
      page = new NearLocationSearchPage();
    } else {
      // this is the first time the 'current location' is selected
      page = new NearLocationMainPage();
    }
    break;
  } default:
    //"This shouldn't happen";
    return;
  }

  page->appear(MSceneWindow::DestroyWhenDismissed);
}
