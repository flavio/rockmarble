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

#ifndef ARTISTMODEL_H
#define ARTISTMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>

#include "dbmanager.h"
#include "isqlquery.h"

class ArtistModel : public QAbstractListModel
{
  Q_OBJECT
  public:
    explicit ArtistModel(const DBManager::Storage& storage, ISqlQuery query,
                         QObject *parent = 0);
    void setQuery(ISqlQuery query);
    int rowCount() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

  private slots:
    void slotArtistAdded(const int artistID, bool favourite);
    void slotArtistAddedToEvent(int artistID, int eventID);
    void slotArtistUpdated(int artistID);

  private:
    QList<unsigned int> getArtistIDs();

    QList<unsigned int> m_artistIDs;
    ISqlQuery m_query;
    DBManager::Storage m_dbStorage;
};

#endif // ARTISTMODEL_H
