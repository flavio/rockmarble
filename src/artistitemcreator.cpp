#include "artistitemcreator.h"
#include "dbmanager.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtGui/QImage>

ArtistItemCreator::ArtistItemCreator(const ArtistPage::PageMode &pageMode,
                                     const QString &country)
  : m_pageMode(pageMode), m_country(country)
{
}

void ArtistItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
  DBManager* db = DBManager::instance();
  QVariant data = index.data(Qt::DisplayRole);
  int artistID = data.toInt();
  QString artist = db->artistFromID(artistID);

  contentItem->setTitle(artist);
  contentItem->boundingRect();

  int eventsNum;
  if (m_pageMode == ArtistPage::ALL_ARTISTS)
    eventsNum = db->eventsWithArtistNum(artistID);
  else
    eventsNum = db->eventsWithArtistInCountryNum(artistID, m_country);

  if (eventsNum > 1)
    contentItem->setSubtitle(QObject::tr("%1 concerts").arg(eventsNum));
  else if (eventsNum == 1)
    contentItem->setSubtitle(QObject::tr("1 event"));
  else if (eventsNum == 0)
    contentItem->setSubtitle(QObject::tr("No events yet."));
  else
    contentItem->setSubtitle(QObject::tr("Error while retrieving data."));

  if (db->artistHasImage(artistID)) {
    QString imagePath = QString("%1/.rockmarble/%2/%3").arg(QDir::homePath())\
                                           .arg("images")\
                                           .arg(artistID);
    if (QFile::exists(imagePath)) {
      QImage image (imagePath);
      contentItem->setImage(image);
    }
  }

  if (index.row() == 0)
      contentItem->setItemMode(MContentItem::SingleColumnTop);
  else if (index.sibling(index.row() + 1, 0).isValid())
      contentItem->setItemMode(MContentItem::SingleColumnCenter);
  else
      contentItem->setItemMode(MContentItem::SingleColumnBottom);
}
