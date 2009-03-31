/* This file is part of rockmarble
  *
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public License
  * along with this library; see the file COPYING.LIB.  If not, write to
  * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  * Boston, MA 02110-1301, USA.
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QMap>
#include <QtCore/QModelIndex>
#include <QVariant>

#include "ui_mainwindow.h"



class DataFetcher;
class EventModel;

class MainWindow : public QMainWindow, private Ui::MainWindowClass
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:
    void slotAddArtist();
    void slotImportLastfm();
    void slotAbout();

    void slotArtistEventsReady(QString,bool,QString);
    void slotArtistEventConverted(QVariant, bool, QString);

    void slotTopArtistsReady(QString,bool,QString);
    void slotTopArtistConverted(QVariant, bool, QString);

    void slotCurrentArtistRowChanged(int);
    void slotCurrentEventChanged(const QModelIndex & current, const QModelIndex & previous);
    void slotFilterTextChanged(const QString& text);

  private:
    void addArtist(const QString&name);


    DataFetcher* m_df;
    QMap<QString, EventModel*> m_artists;
};

#endif // MAINWINDOW_H
