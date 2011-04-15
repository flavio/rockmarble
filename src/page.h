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

#ifndef PAGE_H
#define PAGE_H

#include <QtCore/QString>

class Page
{
  public:
    Page() {}
    Page(const int id, const QString& title,
         const QString& imageID, const QString& subtitle) {
      this->id = id;
      this->title = title;
      this->imageID = imageID;
      this->subtitle = subtitle;
    }

    bool operator ==(const Page& p2) const {
      return id == p2.id;
    }

    Page& operator =(const Page& other) {
      this->id = other.id;
      this->title = other.title;
      this->imageID = other.imageID;
      this->subtitle = other.subtitle;
      return *this;
    }

    int id;
    QString title;
    QString imageID;
    QString subtitle;

};

#endif // PAGE_H
