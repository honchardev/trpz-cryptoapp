#ifndef DBWRAPPER_H
#define DBWRAPPER_H

#include <QObject>

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

class DBWrapper : public QObject
{
    Q_OBJECT

public:
    explicit DBWrapper(QObject *parent = nullptr);

    bool insert_value(ExchangePrice &ep);

    bool get_exchange_prices(QString ep_name, QVector<ExchangePrice *> *ep_vector);

    bool isalive(void);

signals:
    void db_conn_ok();
    void db_conn_fail();

private slots:

private:
    bool open_conn(void);

    bool create_tables(void);

    QString DRIVER;
    QString DB_FILE_PATH;

    QSqlDatabase db_conn;
};

#endif // DBWRAPPER_H
