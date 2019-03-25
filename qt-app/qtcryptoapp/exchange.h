#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <QObject>

#include <QDateTime>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include <QJsonDocument>
#include <QJsonObject>

#include <QTimer>

#include "dbwrapper.h"

class Exchange : public QObject
{
    Q_OBJECT

public:
    explicit Exchange(QObject *parent = nullptr, QString api_endpoint = "N/A", DBWrapper *dbwrapper = nullptr);

    void make_request(void);

    DBWrapper *dbwrapper;

    ExchangePrice *exchange_price;

    QTimer *exchange_timer;

    QString api_endpoint;

signals:

public slots:
    virtual void qnam_finished(QNetworkReply *reply) = 0;
    virtual void tim_timeout() = 0;

private:
    QNetworkAccessManager *qnam_exchange;
};

class BitfinexExchange : public Exchange
{
public:
    BitfinexExchange(QObject *parent = nullptr, QString api_endpoint = "N/A", DBWrapper *dbwrapper = nullptr);

    void qnam_finished(QNetworkReply *reply) override;

    void tim_timeout() override;
};

class BitstampExchange : public Exchange
{
public:
    BitstampExchange(QObject *parent = nullptr, QString api_endpoint = "N/A", DBWrapper *dbwrapper = nullptr);

    void qnam_finished(QNetworkReply *reply) override;

    void tim_timeout() override;
};

class CoinbaseExchange : public Exchange
{
public:
    CoinbaseExchange(QObject *parent = nullptr);

    void set_api_endpoint(QString api_endpoint);

    void qnam_finished(QNetworkReply *reply) override;

    void tim_timeout() override;
};

#endif // EXCHANGE_H
