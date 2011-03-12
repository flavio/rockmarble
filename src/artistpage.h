#ifndef ARTISTPAGE_H
#define ARTISTPAGE_H

#include <MApplicationPage>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>

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
    enum PageMode {ALL_ARTISTS, ARTISTS_BY_COUNTRY};

    ArtistPage(QGraphicsItem *parent = 0);
    ArtistPage(const QString& country, QGraphicsItem *parent = 0);
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

  private:
    QSqlQuery artistsModelQuery();

    void showMessage(const QString& message, bool error = false);

    QString m_country;
    PageMode m_pageMode;
    bool m_filterVisible;
    bool m_showArtistsWithoutEvents;
    Lastfm* m_lastfm;
    MWidget* m_filterWidget;
    MTextEdit* m_filter;
    MLabel* m_noArtistLabel;
    MLinearLayoutPolicy *m_policy;
    ArtistModel* m_artistsModel;
    QStringList m_manuallyAddedArtists;
};

#endif // ARTISTPAGE_H
