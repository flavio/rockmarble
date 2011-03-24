#include "pagesmodel.h"
#include <QtCore/QtAlgorithms>

int PagesModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return m_pages.size();
}

QVariant PagesModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= m_pages.size())
    return QVariant();

  if (role == Qt::DisplayRole) {
    Page search;
    search.id = index.row();
    QList<Page>::const_iterator match;
    match = qFind(m_pages.begin(), m_pages.end(), search);
    if (match != m_pages.end()) {
      Page result = *match;
      return result.id;
    } else
      return QVariant();
  }
  else
    return QVariant();
}
