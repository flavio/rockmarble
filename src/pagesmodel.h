#ifndef PAGESMODEL_H
#define PAGESMODEL_H

#include "mainpage.h"
#include <QtCore/QAbstractListModel>
#include <QtCore/QMap>
#include <QtCore/QString>

class PagesModel  : public QAbstractListModel
{
  Q_OBJECT

public:
  PagesModel(const QMap<MainPage::PageType, QString>& pages, QObject *parent = 0)
      : QAbstractListModel(parent), m_pages(pages) {}

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role) const;

private:
    QMap<MainPage::PageType, QString> m_pages;
};

#endif // PAGESMODEL_H
