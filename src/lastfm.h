#ifndef LASTFM_H
#define LASTFM_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "dbmanager.h"

class DataFetcher;

class Lastfm : public QObject
{
  Q_OBJECT
  public:
    Lastfm(const DBManager::Storage& storage, QObject* parent = 0);

    void getTopArtists(const QString& user);
    void getArtistImage(const QString& artist);
    void getEventsForArtist(const QString& artist);

  private slots:
    void slotEventsForArtistReady(const QString&, bool, const QString&);
    void slotEventsForArtistConverted(QVariant, bool, QString);

    void slotArtistInfoReady(QString,bool,QString);
    void slotArtistInfoConverted(QVariant, bool, QString);

    void slotArtistImageReady(const QString& artist,
                              const QByteArray& data,
                              bool  error,
                              const QString& errMsg);

    void slotTopArtistsReady(QString,bool,QString);
    void slotTopArtistConverted(QVariant, bool, QString);

    void slotEventsNearLocationReady(QString,bool,QString);
    void slotEventsNearLocationConverted(QVariant, bool, QString);

  signals:
    void error(const QString& message);

private:
    DataFetcher* m_df;
    DBManager::Storage m_dbStorage;
};

#endif // LASTFM_H
