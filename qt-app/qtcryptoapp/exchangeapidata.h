#ifndef EXCHANGEAPIDATA_H
#define EXCHANGEAPIDATA_H

#include <QString>

#include <QNetworkReply>
#include <QUrl>

#include "dbwrapper.h"

class ExchangeAPIData
{
public:
    ExchangeAPIData(DBWrapper *dbwrapper) : dbwrapper(dbwrapper) {}
    virtual ~ExchangeAPIData();

    DBWrapper *dbwrapper;

    float buy;
    float sell;
    float last;

    QString TICKER_API_ENDPOINT;
};

class BitfinexAPIData : public ExchangeAPIData
{
public:
    BitfinexAPIData(DBWrapper *dbwrapper, QString base, QString currency);
};

class CoinbaseAPIData : public ExchangeAPIData
{
public:
    CoinbaseAPIData(DBWrapper *dbwrapper, QString base, QString currency);

    QString BUY_API_ENDPOINT;
    QString SELL_API_ENDPOINT;
    QString SPOT_API_ENDPOINT;
};

class BitstampAPIData : public ExchangeAPIData
{
public:
    BitstampAPIData(DBWrapper *dbwrapper, QString base, QString currency);
};

#endif // EXCHANGEAPIDATA_H
