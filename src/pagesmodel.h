#ifndef PAGESMODEL_H
#define PAGESMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>
#include <QtCore/QString>

#include "page.h"

class PagesModel  : public QAbstractListModel
{
  Q_OBJECT

public:
  PagesModel(const QList<Page>& pages, QObject *parent = 0)
      : QAbstractListModel(parent), m_pages(pages) {}

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role) const;

private:
    QList<Page> m_pages;
};

#endif // PAGESMODEL_H
