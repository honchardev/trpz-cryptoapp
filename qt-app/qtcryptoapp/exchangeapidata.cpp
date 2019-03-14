#include "exchangeapidata.h"

#include <QDebug>

ExchangeAPIData::~ExchangeAPIData() {}

BitfinexAPIData::BitfinexAPIData(DBWrapper *dbwrapper, QString base, QString currency) :
    ExchangeAPIData(dbwrapper)
{
    TICKER_API_ENDPOINT = QString("https://api.bitfinex.com/v1/pubticker/%1%2").arg(base).arg(currency);
}

CoinbaseAPIData::CoinbaseAPIData(DBWrapper *dbwrapper, QString base, QString currency) :
    ExchangeAPIData(dbwrapper)
{
    TICKER_API_ENDPOINT = QString("Not applicable");
    BUY_API_ENDPOINT = QString("https://api.coinbase.com/v2/prices/%1-%2/buy").arg(base).arg(currency);
    SELL_API_ENDPOINT = QString("https://api.coinbase.com/v2/prices/%1-%2/sell").arg(base).arg(currency);
    SPOT_API_ENDPOINT = QString("https://api.coinbase.com/v2/prices/%1-%2/spot").arg(base).arg(currency);
}

BitstampAPIData::BitstampAPIData(DBWrapper *dbwrapper, QString base, QString currency) :
    ExchangeAPIData(dbwrapper)
{
    TICKER_API_ENDPOINT = QString("https://api.bitfinex.com/v1/pubticker/%1%2").arg(base).arg(currency);
}
