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
