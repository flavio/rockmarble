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

#ifndef NEARLOCATIONMAINPAGE_H
#define NEARLOCATIONMAINPAGE_H

#include <MApplicationPage>
#include <QtCore/QList>
#include <QtCore/QModelIndex>

#include "page.h"

class NearLocationMainPage : public MApplicationPage
{
  Q_OBJECT
  public:
    NearLocationMainPage(QGraphicsItem *parent = 0);

    enum PageType {
      NewSearch,
      PreviousSearch
    };

  protected:
    void createContent();

  private slots:
    void slotItemClicked(QModelIndex);

  private:
    QList<Page> m_pages;
};

#endif // NEARLOCATIONMAINPAGE_H
