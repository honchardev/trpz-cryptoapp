#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include "dbwrapper.h"

class Exchange : public QObject
{
    Q_OBJECT

public:
    explicit Exchange(QObject *parent = nullptr, QString api_endpoint = "N/A");

    void make_request(void);

    float buy;
    float sell;
    float last;

    QString api_endpoint;

signals:

public slots:
    virtual void qnam_finished(QNetworkReply *reply) = 0;

private:
    QNetworkAccessManager *qnam_exchange;
};

class BitfinexExchange : public Exchange
{
public:
    BitfinexExchange(QObject *parent = nullptr, QString api_endpoint = "N/A");

    void qnam_finished(QNetworkReply *reply) override;
};

class BitstampExchange : public Exchange
{
public:
    BitstampExchange(QObject *parent = nullptr, QString api_endpoint = "N/A");

    void qnam_finished(QNetworkReply *reply) override;
};

class CoinbaseExchange : public Exchange
{
public:
    CoinbaseExchange(QObject *parent = nullptr);

    void set_api_endpoint(QString api_endpoint);

    void qnam_finished(QNetworkReply *reply) override;
};

#endif // EXCHANGE_H
