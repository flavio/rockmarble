#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindowClass
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:
    void search();
    void loadKdeDevelopers();
};

#endif // MAINWINDOW_H
