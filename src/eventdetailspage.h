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

#ifndef EVENTDETAILSPAGE_H
#define EVENTDETAILSPAGE_H

#include "dbmanager.h"

#include <MApplicationPage>
#include <QGraphicsGeoMap>
#include <QtCore/QList>

class QGraphicsLinearLayout;
class QNetworkSession;
class MAction;
class MapWidget;
class MComboBox;
class MSlider;

namespace QtMobility {
  class QGeoServiceProvider;
  class QGeoMappingManager;
  class QGeoRoutingManager;
  class QGeoSearchManager;
}

class EventDetailsPage : public MApplicationPage
{
  Q_OBJECT

  public:
    EventDetailsPage(const DBManager::Storage& storage,const int& event_id,
                     QGraphicsItem *parent=0);

  protected:
    virtual void createContent();

  private slots:
    void slotChangeStar();
    void networkSessionOpened();
    void sliderValueChanged(int zoomLevel);
    void slotMapTypeChanged(int);

  private:
    void createMap();
    void updateStarredAction();
    void setProvider(QString providerId);

    QtMobility::QGeoServiceProvider *m_serviceProvider;
    QtMobility::QGeoMappingManager *m_mapManager;
    QtMobility::QGeoRoutingManager *m_routingManager;
    QtMobility::QGeoSearchManager *m_searchManager;

    DBManager::Storage m_dbStorage;
    MAction* m_actionStar;
    int  m_eventID;
    bool m_starred;
    QNetworkSession *m_session;
    MapWidget* m_mapWidget;
    QGraphicsLinearLayout* m_infoLayout;
    MSlider* m_slider;
    QList<QtMobility::QGraphicsGeoMap::MapType> m_mapControlTypes;
    MComboBox* m_comboBox;
};

#endif // EVENTDETAILSPAGE_H
