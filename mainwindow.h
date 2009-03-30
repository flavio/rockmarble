#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QMap>

#include "ui_mainwindow.h"

class Artist;
class DataFetcher;

class MainWindow : public QMainWindow, private Ui::MainWindowClass
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:
    void search();
    void loadKdeDevelopers();
    void searchEvents();
    void slotArtistEventsReady(QVariant,bool,QString);
    void slotCurrentArtistRowChanged(int);
    void slotCurrentEventChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

  private:
    void setupEventTable(const QString& artistName);

    DataFetcher* m_df;
    QMap<QString, Artist*> m_artists;
};

#endif // MAINWINDOW_H
