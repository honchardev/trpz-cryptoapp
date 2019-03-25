#include "exchange.h"

#include <QDebug>

Exchange::Exchange(DBWrapper *dbwrapper, QString endpoint) :
    dbwrapper(dbwrapper),
    exchangemanager(new QNetworkAccessManager),
    gotexchangeanswer(false),
    exchangeanswer(QString("")),
    tickerapiendpoint(endpoint)
{
    connect(exchangemanager, &QNetworkAccessManager::finished,
            this, &Exchange::exchangemanagerfinished);
}

Exchange::~Exchange()
{
    delete exchangemanager;
}

bool Exchange::make_request(float *buy, float *sell, float *last)
{
    QNetworkRequest exchangereq;
    exchangereq.setUrl(tickerapiendpoint);
    exchangemanager->get(exchangereq);
    if (gotexchangeanswer) {
        gotexchangeanswer = false;
        qDebug() << "Exchange::exchangemanagerfinished data: " << exchangeanswer;
        // todo: parse data
        *buy = 0.0f;
        *sell = 0.0f;
        *last = 0.0f;
        qDebug() << *buy << "\t" << *sell << "\t" << *last;
        return true;
    }
    return false;
}

void Exchange::exchangemanagerfinished(QNetworkReply *reply)
{
    if (reply->error()) {
        qWarning() << "coinbasemanagerfinished ERROR: " << reply->errorString();
        return;
    }
    exchangeanswer = reply->readAll();
    gotexchangeanswer = true;
}

Coinbase::Coinbase(DBWrapper *dbwrapper) :
    Exchange (dbwrapper, QString("N/A"))
{
    QString base("btc");
    QString currency("usd");
    buyapiendpoint = QString("https://api.coinbase.com/v2/prices/%1-%2/buy").arg(base).arg(currency);
    sellapiendpoint = QString("https://api.coinbase.com/v2/prices/%1-%2/sell").arg(base).arg(currency);
    spotapiendpoint = QString("https://api.coinbase.com/v2/prices/%1-%2/spot").arg(base).arg(currency);
}

bool Coinbase::make_request(float *buy, float *sell, float *last)
{
    QNetworkRequest exchangereq;

    exchangereq.setUrl(buyapiendpoint);
    exchangemanager->get(exchangereq);
    if (gotexchangeanswer) {
        gotexchangeanswer = false;
        qDebug() << "Coinbase::make_request buy data: " << exchangeanswer;
        // todo: parse data
        *buy = 0.0f;
        qDebug() << *buy;
    }

    exchangereq.setUrl(sellapiendpoint);
    exchangemanager->get(exchangereq);
    if (gotexchangeanswer) {
        gotexchangeanswer = false;
        qDebug() << "Coinbase::make_request sell data: " << exchangeanswer;
        // todo: parse data
        *sell = 0.0f;
        qDebug() << *sell;
    }

    exchangereq.setUrl(spotapiendpoint);
    exchangemanager->get(exchangereq);
    if (gotexchangeanswer) {
        gotexchangeanswer = false;
        qDebug() << "Coinbase::make_request last data: " << exchangeanswer;
        // todo: parse data
        *last = 0.0f;
        qDebug() << *last;
        return true;
    }

    return false;
}
