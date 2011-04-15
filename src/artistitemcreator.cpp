/* This file is part of rockmarble
  *
  * Copyright (C) 2011 Flavio Castelli <flavio@castelli.name>
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2  of  the  License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be  useful, but
  * WITHOUT  ANY  WARRANTY;  without  even  the   implied   warranty  of
  * MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  GNU
  * General Public License for more details.
  *
  * You should have received a copy of the GNU  General  Public  License
  * along with  this  program;  if  not,  write  to  the  Free  Software
  * Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
  */

#include "artistitemcreator.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtGui/QImage>

ArtistItemCreator::ArtistItemCreator(const ArtistPage::PageMode &pageMode,
                                     const DBManager::Storage& storage,
                                     const QString &country)
  : MAbstractCellCreator<MContentItem>(),
    m_pageMode(pageMode),
    m_dbStorage(storage),
    m_country(country)
{
}

MWidget* ArtistItemCreator::createCell(const QModelIndex &index,
                                           MWidgetRecycler &recycler) const
{
  QVariant data = index.data(Qt::DisplayRole);
  int artistID = data.toInt();
  QString recycledID = QString("%1%2").arg(MContentItem::staticMetaObject.className())\
                                      .arg(artistID);
  MContentItem* cell = qobject_cast<MContentItem*>(recycler.take(recycledID));
  if(cell == NULL)
    cell = new MContentItem();

  updateCell(index, cell);

  return cell;
}

void ArtistItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);

  DBManager* db = DBManager::instance(m_dbStorage);
  QVariant data = index.data(Qt::DisplayRole);
  int artistID = data.toInt();
  QString artist = db->artistNameFromID(artistID);

  contentItem->setTitle(artist);
  contentItem->boundingRect();

  int eventsNum;
  if (m_pageMode == ArtistPage::ALL_ARTISTS ||
      m_pageMode == ArtistPage::ARTIST_NEAR_LOCATION_SEARCH ||
      m_pageMode == ArtistPage::ARTIST_NEAR_LOCATION_SHOW)
    eventsNum = db->eventsWithArtistNum(artistID);
  else
    eventsNum = db->eventsWithArtistInCountryNum(artistID, m_country);

  if (db->artistHasAllData(artistID)) {
    if (eventsNum > 1)
      contentItem->setSubtitle(QObject::tr("%1 concerts.").arg(eventsNum));
    else if (eventsNum == 1)
      contentItem->setSubtitle(QObject::tr("1 event."));
    else if (eventsNum == 0) {
      contentItem->setSubtitle(QObject::tr("No events."));
      contentItem->setEnabled(false);
    } else
      contentItem->setSubtitle(QObject::tr("Error while retrieving data."));
  } else {
    if (eventsNum > 1)
      contentItem->setSubtitle(QObject::tr("Downloading, %1 concerts found...").arg(eventsNum));
    else if (eventsNum == 1)
      contentItem->setSubtitle(QObject::tr("Downloading, 1 event found..."));
    else if (eventsNum == 0) {
      contentItem->setSubtitle(QObject::tr("Downloading, no events found yet..."));
      contentItem->setEnabled(false);
    } else
      contentItem->setSubtitle(QObject::tr("Error while retrieving data."));
  }

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
