#ifndef ISQLQUERY_H
#define ISQLQUERY_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

//! This is just a convenience class which makes easier to debug certain SQL
//! errors I got.
class ISqlQuery : public QSqlQuery
{
  public:
    ISqlQuery ( const QString & query = QString(),
                QSqlDatabase db = QSqlDatabase() );
    ISqlQuery ( QSqlDatabase db );

    QVariant value ( int index ) const;
};

#endif // ISQLQUERY_H
