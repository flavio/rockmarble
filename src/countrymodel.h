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

#ifndef COUNTRYMODEL_H
#define COUNTRYMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QStringList>

#include "dbmanager.h"
#include "isqlquery.h"

class CountryModel : public QAbstractListModel
{
  Q_OBJECT
  public:
    enum Mode {BY_ARTIST, ALL_COUNTRIES};

    explicit CountryModel(const DBManager::Storage& storage,
                          const int& artistID,
                          QObject *parent = 0);
    explicit CountryModel(const DBManager::Storage& storage,
                          QObject *parent = 0);
    void setQuery(ISqlQuery query);
    int rowCount() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

  private slots:
    void slotLocationCreated(int locationID);

  private:
    void setup();
    QStringList getCountries();
    ISqlQuery getQuery();

    QStringList m_countries;
    ISqlQuery m_query;
    Mode m_mode;
    int m_artistID;
    DBManager::Storage m_dbStorage;
};

#endif // COUNTRYMODEL_H
