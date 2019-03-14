#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    base("btc"),
    currency("usd")
{
    ui->setupUi(this);

    dbwrapper = new DBWrapper();

    bitfinexapidata = new BitfinexAPIData(dbwrapper, base, currency);
    coinbaseapidata = new CoinbaseAPIData(dbwrapper, base, currency);
    bitstampapidata = new BitstampAPIData(dbwrapper, base, currency);

    bitfinexmanager = new QNetworkAccessManager();
    coinbasemanager = new QNetworkAccessManager();
    bitstampmanager = new QNetworkAccessManager();

    connect(bitfinexmanager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(bitfinexmanagerfinished(QNetworkReply*)));
    connect(coinbasemanager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(coinbasemanagerfinished(QNetworkReply*)));
    connect(bitstampmanager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(bitstampmanagerfinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dbwrapper;
    delete bitfinexapidata;
    delete coinbaseapidata;
    delete bitstampapidata;
    delete bitfinexmanager;
    delete coinbasemanager;
    delete bitstampmanager;
}

void MainWindow::bitfinexmanagerfinished(QNetworkReply *apireply)
{
    if (apireply->error()) {
        qWarning() << "bitfinexmanagerfinished ERROR: " << apireply->errorString();
        return;
    }
    QString bitfinexanswer = apireply->readAll();
    qDebug() << "bitfinexmanagerfinished data: " << bitfinexanswer;
}

void MainWindow::coinbasemanagerfinished(QNetworkReply *apireply)
{
    if (apireply->error()) {
        qWarning() << "coinbasemanagerfinished ERROR: " << apireply->errorString();
        return;
    }
    QString coinbaseanswer = apireply->readAll();
    qDebug() << "coinbasemanagerfinished data: " << coinbaseanswer;
}

void MainWindow::bitstampmanagerfinished(QNetworkReply *apireply)
{
    if (apireply->error()) {
        qWarning() << "bitstampmanagerfinished ERROR: " << apireply->errorString();
        return;
    }
    QString bitstampanswer = apireply->readAll();
    qDebug() << "bitstampmanagerfinished data: " << bitstampanswer;
}

void MainWindow::on_bitfinex_btn_clicked()
{
    bitfinexreq.setUrl(bitfinexapidata->TICKER_API_ENDPOINT);
    bitfinexmanager->get(bitfinexreq);
}

void MainWindow::on_coinbase_btn_clicked()
{
    coinbasereq.setUrl(coinbaseapidata->BUY_API_ENDPOINT);
    coinbasemanager->get(coinbasereq);

    coinbasereq.setUrl(coinbaseapidata->SELL_API_ENDPOINT);
    coinbasemanager->get(coinbasereq);

    coinbasereq.setUrl(coinbaseapidata->SPOT_API_ENDPOINT);
    coinbasemanager->get(coinbasereq);
}

void MainWindow::on_bitstamp_btn_clicked()
{
    bitstampreq.setUrl(bitstampapidata->TICKER_API_ENDPOINT);
    bitstampmanager->get(bitstampreq);
}
