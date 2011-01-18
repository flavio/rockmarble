/* This file is part of rockmarble
  *
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
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

#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <MApplicationPage>
#include <QtCore/QModelIndex>
#include <QVariant>

class Lastfm;
class QSqlQueryModel;

class MainPage : public MApplicationPage
{
  Q_OBJECT

  public:
    MainPage(QGraphicsItem *parent = 0);
    ~MainPage();

  protected:
    void createContent();

  private slots:
    void slotAddArtist();
    void slotImportLastfm();
//    void slotAddCity();
    void slotAbout();

    void slotArtistClicked(const QModelIndex& index);
    void slotNewArtistAdded();

//    void slotCurrentTabChanged(int);
//    void slotCurrentArtistRowChanged(int);
//    void slotCurrentCityRowChanged(int);
//    void slotCurrentEventChanged(const QModelIndex & current, const QModelIndex & previous);
//    void slotFilterTextChanged(const QString& text);
//    void slotFilterIndexChanged();

  private:
    void addArtist(const QString& name);
    void addCity(const QString& name);

    void showMessage(const QString& message, bool error = false);

//    QString m_lastArtistFilterText;
//    int m_lastArtistFilterRule;
//    QModelIndex m_lastArtistTableItem;

//    QString m_lastCityFilterText;
//    int m_lastCityFilterRule;
//    QModelIndex m_lastCityTableItem;

    Lastfm* m_lastfm;
    QSqlQueryModel* m_artistsModel;
};

#endif // MAINPAGE_H
