#include "isqlquery.h"

#include <QtDebug>
#include <QtCore/QVariant>

ISqlQuery::ISqlQuery ( const QString & query, QSqlDatabase db)
  : QSqlQuery(query, db)
{
}

ISqlQuery::ISqlQuery ( QSqlDatabase db )
  : QSqlQuery(db)
{
}

QVariant ISqlQuery::value ( int index ) const
{
  QVariant v = QSqlQuery::value(index);
  if (!v.isValid()) {
    qWarning() << "Got an invalid value while trying to access index" << index
               << "of query" << lastQuery();
  }
  return v;
}
