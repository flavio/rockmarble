#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QtCore/QAbstractTableModel>

class Artist;

class EventModel : public QAbstractTableModel
{
  public:
    EventModel(Artist* artist, QObject* parent = 0);
    virtual ~EventModel();

    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;

    bool getCoordinates(const QModelIndex& index, qreal* latitude, qreal* longitude);

  private:
    Artist* m_artist;
};

#endif // EVENTMODEL_H
