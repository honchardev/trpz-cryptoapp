#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    webconnstatus_lbl = new QLabel("Connection: No data", this);
    statusBar()->addWidget(webconnstatus_lbl);
    statusbar_ln = new QFrame(this);
    statusbar_ln->setFrameShape(QFrame::VLine);
    statusBar()->addWidget(statusbar_ln);
    dbconnstatus_lbl = new QLabel("DB Connection: No data", this);
    statusBar()->addWidget(dbconnstatus_lbl);

    bitfinex_chart = new Chart(this, ui->bitfinexdata_wdgt);
    bitstamp_chart = new Chart(this, ui->bitstampdata_wdgt);
    coinbase_chart = new Chart(this, ui->coinbasedata_wdgt);

    dbwrapper = new DBWrapper(this);
    connect(dbwrapper, &DBWrapper::db_conn_ok,
            this, &MainWindow::updategui_db_conn_ok);
    connect(dbwrapper, &DBWrapper::db_conn_fail,
            this, &MainWindow::updategui_db_conn_fail);

    cmcscrapper = new CMCScrapper(this);
    connect(cmcscrapper, &CMCScrapper::data_receive_ok,
            this, &MainWindow::updategui_cmc_block_conn_ok);
    connect(cmcscrapper, &CMCScrapper::data_receive_fail,
            this, &MainWindow::updategui_cmc_block_conn_fail);

    bitfinex = new BitfinexExchange(this, QString("https://api.bitfinex.com/v1/pubticker/btcusd"), dbwrapper);
    connect(bitfinex, &Exchange::data_receive_ok,
            this, &MainWindow::updategui_bitfinex_block_conn_ok);
    connect(bitfinex, &Exchange::data_receive_fail,
            this, &MainWindow::updategui_bitfinex_block_conn_fail);

    bitstamp = new BitstampExchange(this, QString("https://www.bitstamp.net/api/v2/ticker/btcusd"), dbwrapper);
    connect(bitstamp, &Exchange::data_receive_ok,
            this, &MainWindow::updategui_bitstamp_block_conn_ok);
    connect(bitstamp, &Exchange::data_receive_fail,
            this, &MainWindow::updategui_bitstamp_block_conn_fail);

    coinbase = new CoinbaseExchange(this, dbwrapper);
    connect(coinbase, &Exchange::data_receive_ok,
            this, &MainWindow::updategui_coinbase_block_conn_ok);
    connect(coinbase, &Exchange::data_receive_fail,
            this, &MainWindow::updategui_coinbase_block_conn_fail);

    if (dbwrapper->isalive()) {
        cmcscrapper->make_request();
        bitfinex->make_request();
        bitstamp->make_request();
        coinbase->set_api_endpoint(QString("https://api.coinbase.com/v2/prices/btc-usd/buy"));
        coinbase->make_request();
        coinbase->set_api_endpoint(QString("https://api.coinbase.com/v2/prices/btc-usd/sell"));
        coinbase->make_request();
        coinbase->set_api_endpoint(QString("https://api.coinbase.com/v2/prices/btc-usd/spot"));
        coinbase->make_request();

        cmcscrapper->start_timer();
        bitfinex->start_timer();
        bitstamp->start_timer();
        coinbase->start_timer();
    }
    else {
        int reply = QMessageBox::critical(this, QString("Database fail"), QString("Couldn't connect to the database"));
        if (reply == QMessageBox::Ok) {
            this->close();
            this->~MainWindow();
        }
    }

    this->resize(this->width(), static_cast<int>(this->height()*0.5f));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete webconnstatus_lbl;
    delete dbconnstatus_lbl;
    delete statusbar_ln;
    delete dbwrapper;
    delete cmcscrapper;
    delete bitfinex;
    delete bitstamp;
    delete coinbase;
}

void MainWindow::updategui_db_conn_ok()
{
    dbconnstatus_lbl->setText(QString("DB Connection: OK"));
    dbconnstatus_lbl->setStyleSheet("QLabel {color : green}");
}

void MainWindow::updategui_db_conn_fail()
{
    dbconnstatus_lbl->setText(QString("DB Connection: FAIL"));
    dbconnstatus_lbl->setStyleSheet("QLabel {color : red}");
}

void MainWindow::updategui_cmc_block_conn_ok()
{
    webconnstatus_lbl->setText(QString("Connection: OK"));
    webconnstatus_lbl->setStyleSheet("QLabel {color : green}");
    ui->cmc_updrate->setStyleSheet("QLabel {color : green}");

    ui->marketcapval_lbl->setText(cmcscrapper->market_cap);
    ui->averpriceval_lbl->setText(cmcscrapper->price);
    ui->volumeval_lbl->setText(cmcscrapper->volume_24h);
    ui->supplyval_lbl->setText(cmcscrapper->circulating_supply);
    ui->changeval_lbl->setText(cmcscrapper->change_24h);
    if (cmcscrapper->change_24h.contains(QChar('-'))) {
        ui->changeval_lbl->setStyleSheet("QLabel {color : red}");
    }
    else {
        ui->changeval_lbl->setStyleSheet("QLabel {color : green}");
    }
}

void MainWindow::updategui_cmc_block_conn_fail()
{
    webconnstatus_lbl->setText(QString("Connection: FAIL"));
    webconnstatus_lbl->setStyleSheet("QLabel {color : red}");
    ui->cmc_updrate->setStyleSheet("QLabel {color : red}");

    ui->marketcapval_lbl->setText(QString("Connection fail"));
    ui->averpriceval_lbl->setText(QString("Connection fail"));
    ui->volumeval_lbl->setText(QString("Connection fail"));
    ui->supplyval_lbl->setText(QString("Connection fail"));
    ui->changeval_lbl->setText(QString("Connection fail"));

    ui->changeval_lbl->setStyleSheet("QLabel {color : black}");
}

void MainWindow::updategui_bitfinex_block_conn_ok()
{
    webconnstatus_lbl->setText(QString("Connection: OK"));
    webconnstatus_lbl->setStyleSheet("QLabel {color : green}");
    ui->bitfinex_updrate->setStyleSheet("QLabel {color : green}");

    bitfinex_chart->add_data_point(bitfinex->exchange_price->timestamp.toTime_t(),
                                   static_cast<double>(bitfinex->exchange_price->bid),
                                   static_cast<double>(bitfinex->exchange_price->ask),
                                   static_cast<double>(bitfinex->exchange_price->price));
}

void MainWindow::updategui_bitfinex_block_conn_fail()
{
    webconnstatus_lbl->setText(QString("Connection: FAIL"));
    webconnstatus_lbl->setStyleSheet("QLabel {color : red}");
    ui->bitfinex_updrate->setStyleSheet("QLabel {color : red}");

    bitfinex_chart->add_data_point(QDateTime::currentDateTime().toTime_t(),
                                   static_cast<double>(bitfinex_chart->bid_values->last()),
                                   static_cast<double>(bitfinex_chart->ask_values->last()),
                                   static_cast<double>(bitfinex_chart->price_values->last()));
}

void MainWindow::updategui_bitstamp_block_conn_ok()
{
    webconnstatus_lbl->setText(QString("Connection: OK"));
    webconnstatus_lbl->setStyleSheet("QLabel {color : green}");
    ui->bitstamp_updrate->setStyleSheet("QLabel {color : green}");

    bitstamp_chart->add_data_point(bitstamp->exchange_price->timestamp.toTime_t(),
                                   static_cast<double>(bitstamp->exchange_price->bid),
                                   static_cast<double>(bitstamp->exchange_price->ask),
                                   static_cast<double>(bitstamp->exchange_price->price));
}

void MainWindow::updategui_bitstamp_block_conn_fail()
{
    webconnstatus_lbl->setText(QString("Connection: FAIL"));
    webconnstatus_lbl->setStyleSheet("QLabel {color : red}");
    ui->bitstamp_updrate->setStyleSheet("QLabel {color : red}");

    bitstamp_chart->add_data_point(QDateTime::currentDateTime().toTime_t(),
                                   static_cast<double>(bitstamp_chart->bid_values->last()),
                                   static_cast<double>(bitstamp_chart->ask_values->last()),
                                   static_cast<double>(bitstamp_chart->price_values->last()));
}

void MainWindow::updategui_coinbase_block_conn_ok()
{
    webconnstatus_lbl->setText(QString("Connection: OK"));
    webconnstatus_lbl->setStyleSheet("QLabel {color : green}");
    ui->coinbase_updrate->setStyleSheet("QLabel {color : green}");

    coinbase_chart->add_data_point(coinbase->exchange_price->timestamp.toTime_t(),
                                   static_cast<double>(coinbase->exchange_price->bid),
                                   static_cast<double>(coinbase->exchange_price->ask),
                                   static_cast<double>(coinbase->exchange_price->price));
}

void MainWindow::updategui_coinbase_block_conn_fail()
{
    webconnstatus_lbl->setText(QString("Connection: FAIL"));
    webconnstatus_lbl->setStyleSheet("QLabel {color : red}");
    ui->coinbase_updrate->setStyleSheet("QLabel {color : red}");

    coinbase_chart->add_data_point(QDateTime::currentDateTime().toTime_t(),
                                   static_cast<double>(coinbase_chart->bid_values->last()),
                                   static_cast<double>(coinbase_chart->ask_values->last()),
                                   static_cast<double>(coinbase_chart->price_values->last()));
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox help_msgbox;
    QString help_msg = "<p align='center'>For any questions related to the application "
                       "please contact the developer<br>t.me/ruki_wgoru_ce_policia</p>";
    help_msgbox.setWindowTitle("Help");
    help_msgbox.setText(help_msg);
    help_msgbox.setStandardButtons(QMessageBox::Ok);
    help_msgbox.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox about_msgbox;
    QString about_msg = "<p align='center'>CryptoData<br>"
                        "1.0.0<br><br>"
                        "CryptoData is a TRPZ project for Windows, Linux and MacOS<br><br>"
                        "Credits: created by Maksym Honchar<br>"
                        "Kyiv 2019</p>";
    about_msgbox.setWindowTitle("About");
    about_msgbox.setText(about_msg);
    about_msgbox.addButton(QMessageBox::Ok);
    about_msgbox.exec();
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionText_triggered()
{

}

void MainWindow::on_actionCSV_triggered()
{

}

void MainWindow::on_actionAs_png_image_triggered()
{

}
