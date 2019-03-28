#include "exchange.h"

#include <QDebug>

Exchange::Exchange(QObject *parent, QString api_endpoint, DBWrapper *dbwrapper) :
    QObject (parent),
    dbwrapper(dbwrapper),
    exchange_price(nullptr),
    api_endpoint(api_endpoint)
{
    exchange_timer = new QTimer(this);
    connect(exchange_timer, SIGNAL(timeout()),
            this, SLOT(tim_timeout()));
}

void Exchange::make_request()
{
    qnam_exchange = new QNetworkAccessManager(this);
    connect(qnam_exchange, &QNetworkAccessManager::finished,
            this, &Exchange::qnam_finished);
    qnam_exchange->get(QNetworkRequest(QUrl(api_endpoint)));
}

void Exchange::start_timer()
{
    exchange_timer->start(15 * 1000);
}

BitfinexExchange::BitfinexExchange(QObject *parent, QString api_endpoint, DBWrapper *dbwrapper) :
    Exchange (parent, api_endpoint, dbwrapper)
{
}

void BitfinexExchange::qnam_finished(QNetworkReply *reply)
{
    if (reply->error()) {
        emit(data_receive_fail());
        qWarning() << "BitfinexExchange::qnam_finished ERROR: " << reply->errorString();
    }
    else {
        QString bitfinex_response = reply->readAll();
        QJsonDocument json_response_doc = QJsonDocument::fromJson(bitfinex_response.toUtf8());
        QJsonObject json_response_obj = json_response_doc.object();
        exchange_price = new ExchangePrice(
                    QDateTime::currentDateTime(),
                    "bitfinex",
                    "btc",
                    "usd",
                    static_cast<float>(json_response_obj["bid"].toString().toDouble()),
                static_cast<float>(json_response_obj["ask"].toString().toDouble()),
                static_cast<float>(json_response_obj["last_price"].toString().toDouble())
                );
        dbwrapper->insert_value(*exchange_price);
        emit(data_receive_ok());
        delete exchange_price;
    }
    reply->deleteLater();
}

void BitfinexExchange::tim_timeout()
{
    make_request();
}

BitstampExchange::BitstampExchange(QObject *parent, QString api_endpoint, DBWrapper *dbwrapper) :
    Exchange (parent, api_endpoint, dbwrapper)
{
}

void BitstampExchange::qnam_finished(QNetworkReply *reply)
{
    if (reply->error()) {
        emit(data_receive_fail());
        qWarning() << "BitstampExchange::qnam_finished ERROR: " << reply->errorString();
    }
    else {
        QString bitstamp_response = reply->readAll();
        QJsonDocument json_response_doc = QJsonDocument::fromJson(bitstamp_response.toUtf8());
        QJsonObject json_response_obj = json_response_doc.object();
        exchange_price = new ExchangePrice(
                    QDateTime::currentDateTime(),
                    "bitstamp",
                    "btc",
                    "usd",
                    static_cast<float>(json_response_obj["bid"].toString().toDouble()),
                static_cast<float>(json_response_obj["ask"].toString().toDouble()),
                static_cast<float>(json_response_obj["last"].toString().toDouble())
                );
        dbwrapper->insert_value(*exchange_price);
        emit(data_receive_ok());
        delete exchange_price;
    }
    reply->deleteLater();
}

void BitstampExchange::tim_timeout()
{
    make_request();
}

CoinbaseExchange::CoinbaseExchange(QObject *parent, DBWrapper *dbwrapper) :
    Exchange (parent, "NA", dbwrapper)
{
}

void CoinbaseExchange::set_api_endpoint(QString api_endpoint)
{
    this->api_endpoint = api_endpoint;
}

void CoinbaseExchange::qnam_finished(QNetworkReply *reply)
{
    if (reply->error()) {
        emit(data_receive_fail());
        qWarning() << "CoinbaseExchange::qnam_finished ERROR: " << reply->errorString();
    }
    else {
        QString coinbase_response = reply->readAll();
        QJsonDocument json_response_doc = QJsonDocument::fromJson(coinbase_response.toUtf8());
        QJsonObject json_response_obj = json_response_doc.object();
        QJsonObject json_coinbase_data = json_response_obj["data"].toObject();
        if (!exchange_price) {
            exchange_price = new ExchangePrice(
                        QDateTime::currentDateTime(),
                        "coinbase",
                        "btc",
                        "usd",
                        0.0f,
                        0.0f,
                        0.0f
                        );
        }
        QString reply_url = reply->url().toString();
        if (reply_url.contains("spot")) {
            exchange_price->price = static_cast<float>(json_coinbase_data["amount"].toString().toDouble());
        }
        else if (reply_url.contains("sell")) {
            exchange_price->bid = static_cast<float>(json_coinbase_data["amount"].toString().toDouble());
        }
        else if (reply_url.contains("buy")) {
            exchange_price->ask = static_cast<float>(json_coinbase_data["amount"].toString().toDouble());
        }
        if (exchange_price->bid != 0.0f && exchange_price->ask != 0.0f && exchange_price->price != 0.0f) {
            dbwrapper->insert_value(*exchange_price);
            emit(data_receive_ok());
            delete exchange_price;
            exchange_price = nullptr;
        }
    }
    reply->deleteLater();
}

void CoinbaseExchange::tim_timeout()
{
    set_api_endpoint(QString("https://api.coinbase.com/v2/prices/btc-usd/buy"));
    make_request();

    set_api_endpoint(QString("https://api.coinbase.com/v2/prices/btc-usd/sell"));
    make_request();

    set_api_endpoint(QString("https://api.coinbase.com/v2/prices/btc-usd/spot"));
    make_request();
}
