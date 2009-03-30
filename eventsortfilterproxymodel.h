#ifndef EVENTSORTFILTERPROXYMODEL_H
#define EVENTSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QModelIndex>

class EventSortFilterProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT

  public:
    EventSortFilterProxyModel(QObject *parent = 0);
    virtual ~EventSortFilterProxyModel() {}

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // EVENTSORTFILTERPROXYMODEL_H
