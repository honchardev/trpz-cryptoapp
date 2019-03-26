#include "dbwrapper.h"

#include <QDebug>

DBWrapper::DBWrapper(QObject *parent) :
    QObject(parent)
{
    DRIVER = QString("QSQLITE");
    DB_FILE_PATH = QString("cryptodb.sqlite");
    open_conn();
    create_tables();
}

ExchangePrice::ExchangePrice(QDateTime timestamp, QString exchange, QString base, QString currency, float bid, float ask, float price) :
    timestamp(timestamp),
    exchange(exchange),
    base(base),
    currency(currency),
    bid(bid),
    ask(ask),
    price(price)
{}

bool DBWrapper::open_conn()
{
    if (QSqlDatabase::isDriverAvailable((DRIVER))) {
        db_conn = QSqlDatabase::addDatabase(DRIVER);
        db_conn.setDatabaseName(DB_FILE_PATH);
        if(db_conn.open()) {
            emit(db_conn_ok());
            qDebug() << "SUCCESS DB open connection";
            return true;
        }
    }
    emit(db_conn_fail());
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
        emit(db_conn_ok());
        qDebug() << "SUCCESS DB create table exchangeprices";
        return true;
    }
    emit(db_conn_fail());
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
        emit(db_conn_ok());
        qDebug() << "SUCCESS insert exchangeprice";
        return true;
    }
    emit(db_conn_fail());
    qWarning() << "ERROR inserting exchangeprice: " << insert_price_query.lastError().text();
    return false;
}

bool DBWrapper::get_exchange_prices(QString ep_name, QVector<ExchangePrice *> *ep_vector)
{
    QSqlQuery select_exchange_prices_query;
    select_exchange_prices_query.prepare("SELECT * "
                                         "FROM exchangeprices "
                                         "WHERE exchange = :exchangename;");
    select_exchange_prices_query.bindValue(":exchangename", ep_name);
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
            ep_vector->push_back(eptoadd);
        }
        emit(db_conn_ok());
        qDebug() << "SUCCESS select exchangeprice";
        return true;
    }
    emit(db_conn_fail());
    qWarning() << "ERROR select exchangeprice: " << select_exchange_prices_query.lastError().text();
    return false;
}

bool DBWrapper::isalive()
{
    QSqlQuery testing_isalive_select_query;
    testing_isalive_select_query.prepare("SELECT * FROM exchangeprices;");
    testing_isalive_select_query.exec();
    if (testing_isalive_select_query.isActive()) {
        emit(db_conn_ok());
        qDebug() << "SUCCESS DB isalive check";
        return true;
    }
    emit(db_conn_fail());
    qWarning() << "ERROR isalive check" << testing_isalive_select_query.lastError().text();
    return false;
}

QString ExchangePrice::toString()
{
    return QString("ExchangePrice obj. %1 %2 %3 %4 %5 %6 %7").arg(timestamp.toString("yyyy-MM-dd hh:mm:ss")).
            arg(exchange).
            arg(base).arg(currency).
            arg(QString::number(static_cast<double>(bid))).
            arg(QString::number(static_cast<double>(ask))).
            arg(QString::number(static_cast<double>(price)));
}
