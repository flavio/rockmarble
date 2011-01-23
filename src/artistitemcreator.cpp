#include "artistitemcreator.h"
#include "dbmanager.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtGui/QImage>

void ArtistItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
  DBManager* db = DBManager::instance();
  QVariant data = index.data(Qt::DisplayRole);
  int artistID = data.toInt();
  QString artist = db->artistFromID(artistID);

  contentItem->setTitle(artist);
  contentItem->boundingRect();

  int eventsNum = db->eventsWithArtistNum(artistID);
  if (eventsNum > 1)
    contentItem->setSubtitle(QString("%1 concerts").arg(eventsNum));
  else if (eventsNum == 1)
    contentItem->setSubtitle("1 event");
  else if (eventsNum == 0)
    contentItem->setSubtitle("No events yet.");
  else
    contentItem->setSubtitle("Error while retrieving data.");

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
