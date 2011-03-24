#ifndef ARTISTPAGE_H
#define ARTISTPAGE_H

#include <MApplicationPage>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>

#include "dbmanager.h"

class ArtistModel;
class Lastfm;
class MLabel;
class MLinearLayoutPolicy;
class MTextEdit;
class MWidget;

class ArtistPage : public MApplicationPage
{
  Q_OBJECT

  public:
    enum PageMode {ALL_ARTISTS, ARTISTS_BY_COUNTRY,
                   ARTIST_NEAR_LOCATION_SHOW, ARTIST_NEAR_LOCATION_SEARCH};

    ArtistPage(const DBManager::Storage& storage,
               const PageMode& pageMode = ALL_ARTISTS,
               QGraphicsItem *parent = 0);
    ArtistPage(const DBManager::Storage& storage, const QString& country,
               QGraphicsItem *parent = 0);
    ArtistPage(const double latitude, const double longitude,
               const int& distance, QGraphicsItem *parent = 0);
    ~ArtistPage();

  protected:
    void createContent();

  private slots:
    void slotAddArtist();
    void slotImportLastfm();

    void slotArtistAdded(const int, bool);
    void slotArtistClicked(const QModelIndex& index);
    void slotFilterChanged();
    void slotShowFilter();
    void slotShowSearch();
    void slotFilterAnimationFinished();
    void slotRefreshArtistsModel();
    void slotRefreshEvents();

  private:
    QSqlQuery artistsModelQuery();

    void showMessage(const QString& message, bool error = false);

    QString m_country;
    PageMode m_pageMode;
    DBManager::Storage m_dbStorage;
    bool m_filterVisible;
    bool m_showArtistsWithoutEvents;
    Lastfm* m_lastfm;
    MWidget* m_filterWidget;
    MTextEdit* m_filter;
    MLabel* m_noArtistLabel;
    MLinearLayoutPolicy *m_policy;
    ArtistModel* m_artistsModel;
    QStringList m_manuallyAddedArtists;
    double m_latitude;
    double m_longitude;
    int m_distance;
};

#endif // ARTISTPAGE_H
