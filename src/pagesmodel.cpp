/* This file is part of rockmarble
  *
  * Copyright (C) 2011 Flavio Castelli <flavio@castelli.name>
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
