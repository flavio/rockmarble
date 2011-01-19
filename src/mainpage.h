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
class MTextEdit;
class QSqlQueryModel;
class QSortFilterProxyModel;

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

    void slotArtistAdded(const QString&);
    void slotArtistClicked(const QModelIndex& index);
    void slotFilterChanged();
    void slotShowFilter();
    void slotShowSearch();

  private:
    void addArtist(const QString& name);
    void addCity(const QString& name);
    QString artistsModelQuery() const;
    void refreshArtistsModel();

    void showMessage(const QString& message, bool error = false);

    bool m_filterVisible;
    bool m_showArtistsWithoutEvents;
    Lastfm* m_lastfm;
    MTextEdit* m_filter;
    QSqlQueryModel* m_artistsModel;
    QSortFilterProxyModel *m_proxyModel;
};

#endif // MAINPAGE_H
