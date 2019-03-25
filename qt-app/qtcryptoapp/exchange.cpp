#include "exchange.h"

#include <QDebug>

Exchange::Exchange(QObject *parent, QString api_endpoint) :
    QObject (parent),
    api_endpoint(api_endpoint)
{
}

void Exchange::make_request()
{
    qnam_exchange = new QNetworkAccessManager(this);
    connect(qnam_exchange, &QNetworkAccessManager::finished,
            this, &Exchange::qnam_finished);
    qnam_exchange->get(QNetworkRequest(QUrl(api_endpoint)));
}

BitfinexExchange::BitfinexExchange(QObject *parent, QString api_endpoint) :
    Exchange (parent, api_endpoint)
{
}

void BitfinexExchange::qnam_finished(QNetworkReply *reply)
{
    if (reply->error()) {
        qWarning() << "BitfinexExchange::qnam_finished ERROR: " << reply->errorString();
        return;
    }
    else {
        // todo: parse data
        qDebug() << reply->readAll();
    }
    reply->deleteLater();
}

BitstampExchange::BitstampExchange(QObject *parent, QString api_endpoint) :
    Exchange (parent, api_endpoint)
{
}

void BitstampExchange::qnam_finished(QNetworkReply *reply)
{
    if (reply->error()) {
        qWarning() << "BitstampExchange::qnam_finished ERROR: " << reply->errorString();
        return;
    }
    else {
        // todo: parse data
        qDebug() << reply->readAll();
    }
    reply->deleteLater();
}

CoinbaseExchange::CoinbaseExchange(QObject *parent) :
    Exchange (parent)
{
}

void CoinbaseExchange::set_api_endpoint(QString api_endpoint)
{
    this->api_endpoint = api_endpoint;
}

void CoinbaseExchange::qnam_finished(QNetworkReply *reply)
{
    if (reply->error()) {
        qWarning() << "CoinbaseExchange::qnam_finished ERROR: " << reply->errorString();
        return;
    }
    else {
        // todo: parse data
        qDebug() << reply->readAll();
    }
    reply->deleteLater();
}
