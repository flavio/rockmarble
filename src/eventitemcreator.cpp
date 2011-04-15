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

#include "eventitemcreator.h"
#include "event.h"
#include "location.h"

EventItemCreator::EventItemCreator(const DBManager::Storage& storage)
  : MAbstractCellCreator<MContentItem>(),
    m_dbStorage(storage)
{
}

void EventItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
  QVariant data = index.data(Qt::DisplayRole);
  int eventID = data.toInt();
  Event* event = DBManager::instance(m_dbStorage)->eventFromID(eventID);

  QDateTime startDate = event->dateTime();

  Location* location = event->location();
  QString city = location->city();
  QString location_name = location->name();

  contentItem->setTitle(QString("%1 - %2").arg(city).arg(location_name));
  contentItem->setSubtitle(startDate.toString(Qt::TextDate));
  contentItem->boundingRect();

  if (index.row() == 0)
      contentItem->setItemMode(MContentItem::SingleColumnTop);
  else if (index.sibling(index.row() + 1, 0).isValid())
      contentItem->setItemMode(MContentItem::SingleColumnCenter);
  else
      contentItem->setItemMode(MContentItem::SingleColumnBottom);

//  if (event->starred()) {
//    contentItem->setOptionalImageID("icon-m-toolbar-favorite-mark");
//  } else {
//    contentItem->setOptionalImageID("icon-m-toolbar-favorite-unmark");
//  }

  delete event;
}
