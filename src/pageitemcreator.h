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

#ifndef PAGEITEMCREATOR_H
#define PAGEITEMCREATOR_H

#include <MList>
#include <MContentItem>
#include <QtCore/QList>

#include "page.h"

class PageItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    void updateCell(const QModelIndex &index, MWidget *cell) const;

    QList<Page> pages;
};

#endif // PAGEITEMCREATOR_H
