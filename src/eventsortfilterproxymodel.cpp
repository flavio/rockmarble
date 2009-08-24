/* This file is part of rockmarble
  *
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
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

#include "eventsortfilterproxymodel.h"

#include <QDate>

EventSortFilterProxyModel::EventSortFilterProxyModel(QObject *parent)
  : QSortFilterProxyModel(parent)
{
}

 bool EventSortFilterProxyModel::lessThan(const QModelIndex &left,
                                       const QModelIndex &right) const
 {
     QVariant leftData = sourceModel()->data(left);
     QVariant rightData = sourceModel()->data(right);

     if (leftData.type() == QVariant::Date) {
         return leftData.toDate() < rightData.toDate();
     } else {
         QString leftString = leftData.toString();
         QString rightString = rightData.toString();

         return QString::localeAwareCompare(leftString, rightString) < 0;
     }
 }
