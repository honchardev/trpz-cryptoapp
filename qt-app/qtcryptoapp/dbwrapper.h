#ifndef DBWRAPPER_H
#define DBWRAPPER_H

#include <QVector>
#include <QDateTime>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

class ExchangePrice
{
public:
    ExchangePrice(QDateTime timestamp,
                  QString exchange,
                  QString base, QString currency,
                  float bid, float ask, float price
                  );

    QString toString(void);

    QDateTime timestamp;
    QString exchange;
    QString base;
    QString currency;
    float bid;
    float ask;
    float price;
};

class DBWrapper
{
public:
    DBWrapper();

    bool insert_value(ExchangePrice &ep);
    bool get_exchange_prices(QString ep_name, QVector<ExchangePrice *> *ep_vector);

private:
    bool open_conn(void);
    bool create_tables(void);

    const QString DRIVER;
    const QString DB_FILE_PATH;

    QSqlDatabase db_conn;
};



#endif // DBWRAPPER_H
