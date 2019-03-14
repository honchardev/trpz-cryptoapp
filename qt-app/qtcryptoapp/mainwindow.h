#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "dbwrapper.h"
#include "exchangeapidata.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void bitfinexmanagerfinished(QNetworkReply *apireply);
    void coinbasemanagerfinished(QNetworkReply *apireply);
    void bitstampmanagerfinished(QNetworkReply *apireply);

    void on_bitfinex_btn_clicked();
    void on_coinbase_btn_clicked();
    void on_bitstamp_btn_clicked();

private:
    Ui::MainWindow *ui;

    DBWrapper *dbwrapper;

    BitfinexAPIData *bitfinexapidata;
    CoinbaseAPIData *coinbaseapidata;
    BitstampAPIData *bitstampapidata;

    QNetworkAccessManager *bitfinexmanager;
    QNetworkAccessManager *coinbasemanager;
    QNetworkAccessManager *bitstampmanager;

    QNetworkRequest bitfinexreq;
    QNetworkRequest coinbasereq;
    QNetworkRequest bitstampreq;

    QString base;
    QString currency;
};

#endif // MAINWINDOW_H
