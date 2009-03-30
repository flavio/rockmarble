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
