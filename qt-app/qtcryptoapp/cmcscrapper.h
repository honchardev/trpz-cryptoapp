#ifndef CMCSCRAPPER_H
#define CMCSCRAPPER_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include <QRegularExpression>

class CMCScrapper : public QObject
{
    Q_OBJECT
public:
    explicit CMCScrapper(QObject *parent = nullptr);

    void make_request(void);

    QString market_cap;
    QString price;
    QString volume_24h;
    QString circulating_supply;
    QString change_24h;

signals:

public slots:
    void qnam_finished(QNetworkReply *reply);

private:
    QNetworkAccessManager *qnam_cmc;

    QRegularExpression re_cmc;
};

#endif // CMCSCRAPPER_H
