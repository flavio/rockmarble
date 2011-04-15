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

#ifndef ARTISTPAGE_H
#define ARTISTPAGE_H

#include <MApplicationPage>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>

#include "dbmanager.h"
#include "isqlquery.h"

class ArtistModel;
class Lastfm;
class MLabel;
class MLinearLayoutPolicy;
class MTextEdit;
class MWidget;

class ArtistPage : public MApplicationPage
{
  Q_OBJECT

  public:
    enum PageMode {ALL_ARTISTS, ARTISTS_BY_COUNTRY,
                   ARTIST_NEAR_LOCATION_SHOW, ARTIST_NEAR_LOCATION_SEARCH};

    ArtistPage(const DBManager::Storage& storage,
               const PageMode& pageMode = ALL_ARTISTS,
               QGraphicsItem *parent = 0);
    ArtistPage(const DBManager::Storage& storage, const QString& country,
               QGraphicsItem *parent = 0);
    ArtistPage(const double latitude, const double longitude,
               const int& distance, QGraphicsItem *parent = 0);
    ~ArtistPage();

  protected:
    void createContent();

  private slots:
    void slotAddArtist();
    void slotImportLastfm();

    void slotArtistAdded(const int, bool);
    void slotArtistClicked(const QModelIndex& index);
    void slotFilterChanged();
    void slotShowFilter();
    void slotShowSearch();
    void slotFilterAnimationFinished();
    void slotRefreshArtistsModel();
    void slotRefreshEvents();

  private:
    ISqlQuery artistsModelQuery();

    void showMessage(const QString& message, bool error = false);

    QString m_country;
    PageMode m_pageMode;
    DBManager::Storage m_dbStorage;
    bool m_filterVisible;
    bool m_showArtistsWithoutEvents;
    Lastfm* m_lastfm;
    MWidget* m_filterWidget;
    MTextEdit* m_filter;
    MLabel* m_noArtistLabel;
    MLinearLayoutPolicy *m_policy;
    ArtistModel* m_artistsModel;
    QStringList m_manuallyAddedArtists;
    double m_latitude;
    double m_longitude;
    int m_distance;
};

#endif // ARTISTPAGE_H
