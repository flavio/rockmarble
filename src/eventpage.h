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

#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <MApplicationPage>
#include <QtCore/QModelIndex>

#include "dbmanager.h"
#include "event.h"

class EventPage : public MApplicationPage
{
  Q_OBJECT

  public:
    enum PageMode {STARRED, ARTIST_BY_COUNTRY, ARTIST_NEAR_LOCATION};

    // This constructor is used to show all the starred events
    EventPage(const DBManager::Storage& storage, QGraphicsItem *parent = 0);
    EventPage(const int& artistID,
              const DBManager::Storage& storage,
              const QString& country,
              QGraphicsItem *parent = 0);
    EventPage(const int& artistID,
              const DBManager::Storage& storage,
              QGraphicsItem *parent = 0);
    ~EventPage();

  protected:
    virtual void createContent();

  private slots:
    void slotEventClicked(const QModelIndex& index);

  private:
    PageMode m_pageMode;
    int m_artistID;
    DBManager::Storage m_dbStorage;
    QString m_country;
};

#endif // EVENTPAGE_H
