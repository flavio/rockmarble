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

#include "nearlocationmainpage.h"
#include "nearlocationsearchpage.h"

#include "artistpage.h"
#include "pagesmodel.h"
#include "pageitemcreator.h"

#include <MList>
#include <MSceneManager>

#include <QtGui/QGraphicsLinearLayout>

NearLocationMainPage::NearLocationMainPage(QGraphicsItem *parent)
  : MApplicationPage(parent)
{
  setTitle("Events near you");

  Page newSearchPage (NewSearch, tr("New search"), QString(),
                      tr("Performs a new search."));
  Page prevSearchPage (PreviousSearch, tr("Previous search"), QString(),
                       tr("Show previous search results."));
  m_pages << newSearchPage;
  m_pages << prevSearchPage;
}

void NearLocationMainPage::createContent()
{
  QGraphicsWidget *panel = centralWidget();
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
  panel->setLayout(layout);

  // MList with fast view
  MList* pagesList = new MList();
  pagesList->setSelectionMode(MList::SingleSelection);

  // Content item creator and item model for the list
  PageItemCreator *cellCreator = new PageItemCreator();
  cellCreator->pages = m_pages;
  pagesList->setCellCreator(cellCreator);

  pagesList->setItemModel(new PagesModel(m_pages));
  layout->addItem(pagesList);

  connect (pagesList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotItemClicked(QModelIndex)));
}

void NearLocationMainPage::slotItemClicked(QModelIndex index)
{
  MApplicationPage* page;
  PageType pageType = (PageType) index.data(Qt::DisplayRole).toInt();

  switch (pageType) {
    case NewSearch:
      page = new NearLocationSearchPage();
      break;
    case PreviousSearch:
      page = new ArtistPage(DBManager::MEMORY,
                            ArtistPage::ARTIST_NEAR_LOCATION_SHOW);
      break;
    default:
      //"This shouldn't happen";
      return;
      break;
  }

  page->appear(MSceneWindow::DestroyWhenDismissed);
}
