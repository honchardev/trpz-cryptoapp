#include <QDebug>

#include "dbwrapper.h"

DBWrapper::DBWrapper() :
    DRIVER("QSQLITE"),
    DB_FILE_PATH("cryptodb.sqlite")
{
    open_conn();
    create_tables();
}

bool DBWrapper::open_conn()
{
    if (QSqlDatabase::isDriverAvailable((DRIVER))) {
        db_conn = QSqlDatabase::addDatabase(DRIVER);
        db_conn.setDatabaseName(DB_FILE_PATH);
        if(db_conn.open()) {
            qDebug() << "SUCCESS DB open connection";
            return true;
        }
    }
    qWarning() << "ERROR open connection db_conn: " << db_conn.lastError();
    return false;
}

bool DBWrapper::create_tables()
{
    QSqlQuery create_table_query;
    create_table_query.prepare("CREATE TABLE IF NOT EXISTS exchangeprices ("
                               "id         INTEGER     PRIMARY KEY,"
                               "timestamp  DATETIME    NOT NULL,"
                               "exchange   VARCHAR(50) NOT NULL,"
                               "base       VARCHAR(25) NOT NULL,"
                               "currency   VARCHAR(25) NOT NULL,"
                               "bid        FLOAT       NOT NULL DEFAULT -1.0,"
                               "ask        FLOAT       NOT NULL DEFAULT -1.0,"
                               "price      FLOAT       NOT NULL DEFAULT -1.0"
                               ");");
    create_table_query.exec();
    if (create_table_query.isActive()) {
        qDebug() << "SUCCESS DB create table exchangeprices";
        return true;
    }
    qWarning() << "ERROR create table exchangeprices: " << create_table_query.lastError().text();
    return false;
}

bool DBWrapper::insert_value(ExchangePrice &ep)
{
    QSqlQuery insert_price_query;
    insert_price_query.prepare("INSERT INTO exchangeprices (timestamp, exchange, base, currency, bid, ask, price)"
                               "VALUES (:timestamp, :exchange, :base, :currency, :bid, :ask, :price);");
    insert_price_query.bindValue(":timestamp", ep.timestamp.toString("yyyy-MM-dd hh:mm:ss"));
    insert_price_query.bindValue(":exchange", ep.exchange);
    insert_price_query.bindValue(":base", ep.base);
    insert_price_query.bindValue(":currency", ep.currency);
    insert_price_query.bindValue(":bid", ep.bid);
    insert_price_query.bindValue(":ask", ep.ask);
    insert_price_query.bindValue(":price", ep.price);
    insert_price_query.exec();
    if (insert_price_query.isActive()) {
        qDebug() << "SUCCESS insert exchangeprice";
        return true;
    }
    qWarning() << "ERROR inserting exchangeprice: " << insert_price_query.lastError().text();
    return false;
}

bool DBWrapper::get_exchange_prices(QString epname, QVector<ExchangePrice *> *epvector)
{
    QSqlQuery select_exchange_prices_query;
    select_exchange_prices_query.prepare("SELECT * "
                                         "FROM exchangeprices "
                                         "WHERE exchange = :exchangename;");
    select_exchange_prices_query.bindValue(":exchangename", epname);
    select_exchange_prices_query.exec();
    if (select_exchange_prices_query.isActive()) {
        while (select_exchange_prices_query.next()) {
            ExchangePrice *eptoadd = new ExchangePrice(
                        select_exchange_prices_query.value(1).toDateTime(),   // timestamp
                        select_exchange_prices_query.value(2).toString(),     // exchange
                        select_exchange_prices_query.value(3).toString(),     // base
                        select_exchange_prices_query.value(4).toString(),     // currency
                        select_exchange_prices_query.value(5).toFloat(),      // bid
                        select_exchange_prices_query.value(6).toFloat(),      // ask
                        select_exchange_prices_query.value(7).toFloat()       // price
                        );
            epvector->push_back(eptoadd);
        }
        qDebug() << "SUCCESS select exchangeprice";
        return true;
    }
    qWarning() << "ERROR select exchangeprice: " << select_exchange_prices_query.lastError().text();
    return false;
}
