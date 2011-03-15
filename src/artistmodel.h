#ifndef ARTISTMODEL_H
#define ARTISTMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>
#include <QtSql/QSqlQuery>

#include "dbmanager.h"

class ArtistModel : public QAbstractListModel
{
  Q_OBJECT
  public:
    explicit ArtistModel(const DBManager::Storage& storage, QSqlQuery query,
                         QObject *parent = 0);
    void setQuery(QSqlQuery query);
    int rowCount() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

  private slots:
    void slotArtistAdded(const int artistID, bool favourite);
    void slotArtistAddedToEvent(int artistID, int eventID);
    void slotArtistUpdated(int artistID);

  private:
    QList<unsigned int> getArtistIDs();

    QList<unsigned int> m_artistIDs;
    QSqlQuery m_query;
    DBManager::Storage m_dbStorage;
};

#endif // ARTISTMODEL_H
