#ifndef LASTFM_H
#define LASTFM_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

class DataFetcher;

class Lastfm : public QObject
{
  Q_OBJECT
  public:
    Lastfm(QObject* parent = 0);

    void getTopArtists(const QString& user);
    void getEventsForArtist(const QString& artist);

  private slots:
    void slotArtistEventsReady(QString,bool,QString);
    void slotArtistEventConverted(QVariant, bool, QString);

    void slotTopArtistsReady(QString,bool,QString);
    void slotTopArtistConverted(QVariant, bool, QString);

    void slotEventsNearLocationReady(QString,bool,QString);
    void slotEventsNearLocationConverted(QVariant, bool, QString);

  signals:
    void error(const QString& message);

private:
    DataFetcher* m_df;
};

#endif // LASTFM_H
