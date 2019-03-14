#ifndef EXCHANGEPRICE_H
#define EXCHANGEPRICE_H

#include <QDateTime>

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

#endif // EXCHANGEPRICE_H
