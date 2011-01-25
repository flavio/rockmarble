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
#include "defines.h"
#include "mainpage.h"
#include "pageitemcreator.h"
#include "pagesmodel.h"

#include <MAction>
#include <MDialog>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MList>
#include <MSceneManager>

#include <QtGui/QGraphicsLinearLayout>

MainPage::MainPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{
  setTitle("Rockmarble");

  m_pages.insert(ByArtist, "By artist");
  m_pages.insert(ByLocation, "By location");
  m_pages.insert(ByCurrentLocation, "By current location");
}

MainPage::~MainPage()
{
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
  actionAbout->setText("About");
  actionAbout->setLocation(MAction::ApplicationMenuLocation);
  addAction(actionAbout);
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));

  // MList with fast view
  MList* pagesList = new MList();
  pagesList->setSelectionMode(MList::SingleSelection);

  // Content item creator and item model for the list
  PageItemCreator *cellCreator = new PageItemCreator();
  cellCreator->pages = m_pages;
  pagesList->setCellCreator(cellCreator);

  pagesList->setItemModel(new PagesModel(m_pages));
  //m_pages.values()
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

void MainPage::slotItemClicked(QModelIndex index)
{
  MApplicationPage* page;
  PageType pageType = (PageType) index.data(Qt::DisplayRole).toInt();

  switch (pageType) {
    case ByArtist:
      page = new ArtistPage();
      break;
    default:
      //"This shouldn't happen";
      return;
      break;
  }

  page->appear(MSceneWindow::DestroyWhenDismissed);
}
