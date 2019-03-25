#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbwrapper = new DBWrapper();

    bitfinex = new BitfinexExchange(this, QString("https://api.bitfinex.com/v1/pubticker/btcusd"));
    bitstamp = new BitstampExchange(this, QString("https://www.bitstamp.net/api/v2/ticker/btcusd"));
    coinbase = new CoinbaseExchange(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dbwrapper;
    delete bitfinex;
}

void MainWindow::on_bitfinex_btn_clicked()
{
    bitfinex->make_request();
}

void MainWindow::on_bitstamp_btn_clicked()
{
    bitstamp->make_request();
}

void MainWindow::on_coinbase_btn_clicked()
{
    coinbase->set_api_endpoint(QString("https://api.coinbase.com/v2/prices/btc-usd/buy"));
    coinbase->make_request();

    coinbase->set_api_endpoint(QString("https://api.coinbase.com/v2/prices/btc-usd/sell"));
    coinbase->make_request();

    coinbase->set_api_endpoint(QString("https://api.coinbase.com/v2/prices/btc-usd/spot"));
    coinbase->make_request();
}
