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

#include "countryitemcreator.h"

void CountryItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
  QVariant data = index.data(Qt::DisplayRole);
  contentItem->setTitle(data.toString());
  contentItem->boundingRect();

  if (index.row() == 0)
      contentItem->setItemMode(MContentItem::SingleColumnTop);
  else if (index.sibling(index.row() + 1, 0).isValid())
      contentItem->setItemMode(MContentItem::SingleColumnCenter);
  else
      contentItem->setItemMode(MContentItem::SingleColumnBottom);
}
