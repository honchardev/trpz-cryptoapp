#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbwrapper = new DBWrapper();

    QString base("btc");
    QString currency("usd");

    QString bitfinexendpoint = QString("https://api.bitfinex.com/v1/pubticker/%1%2").arg(base).arg(currency);
    QString bitstampendpoint = QString("https://www.bitstamp.net/api/v2/ticker/%1%2").arg(base).arg(currency);

    bitfinex = new Exchange(dbwrapper, bitfinexendpoint);
    bitstamp = new Exchange(dbwrapper, bitstampendpoint);
    coinbase = new Coinbase(dbwrapper);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dbwrapper;
    delete bitfinex;
    delete bitstamp;
    delete coinbase;
}

void MainWindow::on_bitfinex_btn_clicked()
{
    float buy = 0;
    float sell = 0;
    float last = 0;

    bitfinex->make_request(&buy, &sell, &last);
}

void MainWindow::on_bitstamp_btn_clicked()
{
    float buy = 0;
    float sell = 0;
    float last = 0;

    bitstamp->make_request(&buy, &sell, &last);
}

void MainWindow::on_coinbase_btn_clicked()
{
    float buy = 0;
    float sell = 0;
    float last = 0;

    coinbase->make_request(&buy, &sell, &last);
}
