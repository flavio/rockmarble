#include "pagesmodel.h"

int PagesModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return m_pages.keys().count();
}

QVariant PagesModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= m_pages.keys().size())
    return QVariant();

  if (role == Qt::DisplayRole)
    return m_pages.keys().at(index.row());
  else
    return QVariant();
}
