#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QMap>
#include <QtCore/QModelIndex>

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
    void slotArtistEventsReady(QVariant,bool,QString);
    void slotCurrentArtistRowChanged(int);
    void slotCurrentEventChanged(const QModelIndex & current, const QModelIndex & previous);

  private:
    DataFetcher* m_df;
    QMap<QString, EventModel*> m_artists;
};

#endif // MAINWINDOW_H
