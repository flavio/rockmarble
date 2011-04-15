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

#ifndef COUNTRYPAGE_H
#define COUNTRYPAGE_H

#include "countrymodel.h"
#include "dbmanager.h"
#include "event.h"

#include <MApplicationPage>

class MLabel;
class MLinearLayoutPolicy;
class MList;

class CountryPage : public MApplicationPage
{
  Q_OBJECT

  public:
    CountryPage(DBManager::Storage storage, QGraphicsItem *parent = 0);
    CountryPage(const int& artistID, DBManager::Storage storage,
                QGraphicsItem *parent = 0);
    ~CountryPage();

  protected:
    virtual void createContent();

  private slots:
    void slotCountryClicked(const QModelIndex& index);
    void slotLocationCreated(int locationID);

  private:
    ISqlQuery getQuery();

    int m_artistID;
    CountryModel::Mode m_pageMode;
    CountryModel* m_countryModel;
    DBManager::Storage m_dbStorage;
    MLabel* m_noCountryLabel;
    MLinearLayoutPolicy* m_policy;
    bool m_noCountryLabelVisible;
};

#endif // COUNTRYPAGE_H
