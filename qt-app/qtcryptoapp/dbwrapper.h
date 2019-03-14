#ifndef DBWRAPPER_H
#define DBWRAPPER_H

#include <QVector>
#include <QDateTime>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include "exchangeprice.h"

class DBWrapper
{
public:
    DBWrapper();

    bool insert_value(ExchangePrice &ep);
    bool get_exchange_prices(QString epname, QVector<ExchangePrice *> *epvector);

private:
    bool open_conn(void);
    bool create_tables(void);

    const QString DRIVER;
    const QString DB_FILE_PATH;

    QSqlDatabase db_conn;
};

#endif // DBWRAPPER_H
