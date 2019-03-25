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
    Exchange(DBWrapper *dbwrapper, QString endpoint);
    virtual ~Exchange();

    virtual bool make_request(float *buy, float *sell, float *last);

    DBWrapper *dbwrapper;

    QNetworkAccessManager *exchangemanager;

    bool gotexchangeanswer;
    QString exchangeanswer;

public slots:
    void exchangemanagerfinished(QNetworkReply *reply);

private:
    QString tickerapiendpoint;
};

class Coinbase : public Exchange
{
public:
    Coinbase(DBWrapper *dbwrapper);

    bool make_request(float *buy, float *sell, float *last) override;

private:
    QString buyapiendpoint;
    QString sellapiendpoint;
    QString spotapiendpoint;
};

#endif // EXCHANGE_H
