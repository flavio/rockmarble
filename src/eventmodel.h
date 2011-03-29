#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>
#include <QtSql/QSqlQuery>

#include "dbmanager.h"

class EventModel : public QAbstractListModel
{
  Q_OBJECT
  public:
    enum Mode {STARRED, ARTIST_BY_COUNTRY, ARTIST_NEAR_LOCATION};
    explicit EventModel(const DBManager::Storage& storage,
                        const int artistID, const QString& country,
                        QObject *parent = 0);
    explicit EventModel(const DBManager::Storage& storage,
                        bool starred,
                        QObject *parent = 0);
    explicit EventModel(const DBManager::Storage& storage,
                        const int artistID,
                        QObject *parent = 0);
    int rowCount() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

  private slots:
    void slotEventCreated(int eventID);

  private:
    void setup();
    QList<int> getEventIDs();
    QSqlQuery getQuery();

    QList<int> m_eventIDs;
    DBManager::Storage m_dbStorage;
    Mode m_mode;
    int m_artistID;
    QString m_country;
    bool m_starred;
};

#endif // EVENTMODEL_H
