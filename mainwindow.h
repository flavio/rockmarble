#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

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

  private:
    DataFetcher* m_df;
};

#endif // MAINWINDOW_H
