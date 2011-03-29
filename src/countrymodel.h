#ifndef COUNTRYMODEL_H
#define COUNTRYMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QStringList>

#include "dbmanager.h"
#include "isqlquery.h"

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
    void setQuery(ISqlQuery query);
    int rowCount() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

  private slots:
    void slotLocationCreated(int locationID);

  private:
    void setup();
    QStringList getCountries();
    ISqlQuery getQuery();

    QStringList m_countries;
    ISqlQuery m_query;
    Mode m_mode;
    int m_artistID;
    DBManager::Storage m_dbStorage;
};

#endif // COUNTRYMODEL_H
