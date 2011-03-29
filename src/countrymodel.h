#ifndef COUNTRYMODEL_H
#define COUNTRYMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QStringList>
#include <QtSql/QSqlQuery>

#include "dbmanager.h"

class CountryModel : public QAbstractListModel
{
  Q_OBJECT
  public:
    enum Mode {BY_ARTIST, ALL_COUNTRIES};

    explicit CountryModel(const DBManager::Storage& storage,
                          const int& artistID,
                          QObject *parent = 0);
    explicit CountryModel(const DBManager::Storage& storage,
                          QObject *parent = 0);
    void setQuery(QSqlQuery query);
    int rowCount() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

  private slots:
    void slotLocationCreated(int locationID);

  private:
    void setup();
    QStringList getCountries();
    QSqlQuery getQuery();

    QStringList m_countries;
    QSqlQuery m_query;
    Mode m_mode;
    int m_artistID;
    DBManager::Storage m_dbStorage;
};

#endif // COUNTRYMODEL_H
