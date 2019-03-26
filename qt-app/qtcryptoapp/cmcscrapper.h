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

    void start_timer(void);

    QString market_cap;
    QString price;
    QString volume_24h;
    QString circulating_supply;
    QString change_24h;

signals:
    void data_receive_ok();
    void data_receive_fail();

public slots:
    void qnam_finished(QNetworkReply *reply);

    void cmc_timeout();

private:
    QTimer *cmc_timer;

    QNetworkAccessManager *qnam_cmc;

    QRegularExpression re_cmc;
};

#endif // CMCSCRAPPER_H
