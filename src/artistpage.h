#ifndef ARTISTPAGE_H
#define ARTISTPAGE_H

#include <MApplicationPage>
#include <QtCore/QModelIndex>
#include <QVariant>

class Lastfm;
class MTextEdit;
class QSqlQueryModel;
class QSortFilterProxyModel;

class ArtistPage : public MApplicationPage
{
  Q_OBJECT

  public:
    ArtistPage(QGraphicsItem *parent = 0);
    ~ArtistPage();

  protected:
    void createContent();

  private slots:
    void slotAddArtist();
    void slotImportLastfm();

    void slotArtistAdded(const QString&, bool);
    void slotArtistClicked(const QModelIndex& index);
    void slotFilterChanged();
    void slotShowFilter();
    void slotShowSearch();

  private:
    QString artistsModelQuery() const;
    void refreshArtistsModel();

    void showMessage(const QString& message, bool error = false);

    bool m_filterVisible;
    bool m_showArtistsWithoutEvents;
    Lastfm* m_lastfm;
    MTextEdit* m_filter;
    QSqlQueryModel* m_artistsModel;
    QSortFilterProxyModel *m_proxyModel;
    QStringList m_manuallyAddedArtists;
};

#endif // ARTISTPAGE_H
