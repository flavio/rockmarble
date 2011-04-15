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

#include "eventpage.h"
#include "eventdetailspage.h"
#include "eventmodel.h"
#include "eventitemcreator.h"

#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>


EventPage::EventPage(const DBManager::Storage& storage, QGraphicsItem *parent)
  : MApplicationPage(parent),
    m_dbStorage(storage)
{
  m_pageMode = STARRED;
  setTitle(tr("Starrred events"));
}

EventPage::EventPage(const int& artistID,
                     const DBManager::Storage& storage,
                     const QString& country,
                     QGraphicsItem *parent)
  : MApplicationPage(parent),
    m_artistID(artistID),
    m_dbStorage(storage),
    m_country(country)
{
  m_pageMode = ARTIST_BY_COUNTRY;
  setTitle(DBManager::instance(m_dbStorage)->artistNameFromID(artistID) +
           " - " + country);
}

EventPage::EventPage(const int& artistID,
                     const DBManager::Storage& storage,
                     QGraphicsItem *parent)
  : MApplicationPage(parent),
    m_artistID(artistID),
    m_dbStorage(storage)
{
  m_pageMode = ARTIST_NEAR_LOCATION;
  setTitle(DBManager::instance(m_dbStorage)->artistNameFromID(artistID) +
           " - " + tr("near you"));
}

EventPage::~EventPage()
{
}

void EventPage::createContent()
{
  QGraphicsWidget *panel = centralWidget();
  MLayout *layout = new MLayout(panel);
  layout->setAnimation(NULL);
  panel->setLayout(layout);
  MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

  // MList with fast view
  MList *eventList = new MList();
  eventList->setSelectionMode(MList::SingleSelection);

  // Content item creator and item model for the list
  EventModel* eventModel = 0;
  switch (m_pageMode) {
    case ARTIST_BY_COUNTRY:
      eventModel = new EventModel(m_dbStorage, m_artistID, m_country);
      break;
    case ARTIST_NEAR_LOCATION:
      eventModel = new EventModel(m_dbStorage, m_artistID);
      break;
    case STARRED:
      eventModel = new EventModel(m_dbStorage, true);
      break;
  }

  EventItemCreator *cellCreator = new EventItemCreator(m_dbStorage);
  eventList->setCellCreator(cellCreator);
  eventList->setItemModel(eventModel);
  policy->addItem(eventList);

  connect (eventList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotEventClicked(QModelIndex)));
}

void EventPage::slotEventClicked(const QModelIndex& index)
{
  int event_id = index.data(Qt::DisplayRole).toInt();
  EventDetailsPage *eventDetailsPage = new EventDetailsPage(m_dbStorage,
                                                            event_id);
  eventDetailsPage->appear(MSceneWindow::DestroyWhenDismissed);
}
