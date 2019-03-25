#ifndef CMCSCRAPPER_H
#define CMCSCRAPPER_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include <QRegularExpression>

#include <QTimer>

class CMCScrapper : public QObject
{
    Q_OBJECT
public:
    explicit CMCScrapper(QObject *parent = nullptr);

    void make_request(void);

    QTimer *cmc_timer;

    QString market_cap;
    QString price;
    QString volume_24h;
    QString circulating_supply;
    QString change_24h;

signals:

public slots:
    void qnam_finished(QNetworkReply *reply);
    void cmc_timeout();

private:
    QNetworkAccessManager *qnam_cmc;

    QRegularExpression re_cmc;
};

#endif // CMCSCRAPPER_H
