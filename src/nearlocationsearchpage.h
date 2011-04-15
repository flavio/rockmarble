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

#ifndef NEARLOCATIONPAGE_H
#define NEARLOCATIONPAGE_H

#include <MApplicationPage>

#include <QGeoPositionInfo>

class MButton;
class MComboBox;

using namespace QtMobility;

class NearLocationSearchPage : public MApplicationPage
{
  Q_OBJECT
  public:
    NearLocationSearchPage(QGraphicsItem *parent = 0);
    ~NearLocationSearchPage();

  protected:
    void createContent();

  private slots:
    void slotSearchEvents();
    void updatePosition(const QGeoPositionInfo& pos);
    void updatePositionTimeout();

  private:
    void showMessage(const QString &message, bool error);

    QGeoPositionInfo m_position;
    MButton* m_btnSearchEvents;
    bool m_trackingLocation;
    MComboBox* m_kmBox;
};

#endif // NEARLOCATIONPAGE_H
