#include "exchangeprice.h"

ExchangePrice::ExchangePrice(QDateTime timestamp, QString exchange, QString base, QString currency, float bid, float ask, float price) :
    timestamp(timestamp),
    exchange(exchange),
    base(base),
    currency(currency),
    bid(bid),
    ask(ask),
    price(price)
{

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
