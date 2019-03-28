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
    QString dirname = QFileDialog::getExistingDirectory(this,
                                                        tr("Select a directory to save TXT files"),
                                                        "");

    QString bitfinex_txt_path(QString("%1/bitfinex_txt_%2.txt").arg(dirname).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    QString bitstamp_txt_path(QString("%1/bitstamp_txt_%2.txt").arg(dirname).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    QString coinbase_txt_path(QString("%1/coinbase_txt_%2.txt").arg(dirname).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));

    QFile bitfinex_txt_file(bitfinex_txt_path);
    QFile bitstamp_txt_file(bitstamp_txt_path);
    QFile coinbase_txt_file(coinbase_txt_path);

    if (!bitfinex_txt_file.open(QIODevice::WriteOnly|QFile::WriteOnly)) {
        QMessageBox::warning(nullptr, "Could not save bitfinex TXT file", QObject::tr( "\nCould not save bitfinex TXT file on disk"));
    }
    else {
        QTextStream bitfinex_stream(&bitfinex_txt_file);
        bitfinex_stream << "BITFINEX PLOT DATA\n\nTIME\tBID\tASK\tLAST\n";
        QList<double> bitfinex_keys_values = bitfinex_chart->keys->toList();
        QList<double> bitfinex_bid_values = bitfinex_chart->bid_values->toList();
        QList<double> bitfinex_ask_values = bitfinex_chart->ask_values->toList();
        QList<double> bitfinex_price_values = bitfinex_chart->price_values->toList();
        for (int i = 0; i < bitfinex_keys_values.length(); i++) {
            double bid_value = bitfinex_bid_values[i];
            double ask_value = bitfinex_ask_values[i];
            double price_value = bitfinex_price_values[i];
            if (static_cast<int>(bid_value) != -1 && static_cast<int>(ask_value) != -2 && static_cast<int>(price_value) != -3) {
                bitfinex_stream << i << QDateTime::fromTime_t(static_cast<uint>(bitfinex_keys_values[i])).toString("yyyy-MM-dd hh:mm:ss") <<
                                   "\t" << bid_value << "\t" << ask_value << "\t" << price_value << endl;
            }
        }
        bitfinex_stream << "FINISH" << endl;
    }
    if (!bitstamp_txt_file.open(QIODevice::WriteOnly|QFile::WriteOnly)) {
        QMessageBox::warning(nullptr, "Could not save bitstamp TXT file", QObject::tr( "\nCould not save bitstamp TXT file on disk"));
    }
    else {
        QTextStream bitstamp_stream(&bitstamp_txt_file);
        bitstamp_stream << "BITSTAMP PLOT DATA\n\nTIME\tBID\tASK\tLAST\n";
        QList<double> bitstamp_keys_values = bitstamp_chart->keys->toList();
        QList<double> bitstamp_bid_values = bitstamp_chart->bid_values->toList();
        QList<double> bitstamp_ask_values = bitstamp_chart->ask_values->toList();
        QList<double> bitstamp_price_values = bitstamp_chart->price_values->toList();
        for (int i = 0; i < bitstamp_keys_values.length(); i++) {
            double bid_value = bitstamp_bid_values[i];
            double ask_value = bitstamp_ask_values[i];
            double price_value = bitstamp_price_values[i];
            if (static_cast<int>(bid_value) != -1 && static_cast<int>(ask_value) != -2 && static_cast<int>(price_value) != -3) {
                bitstamp_stream << i << QDateTime::fromTime_t(static_cast<uint>(bitstamp_keys_values[i])).toString("yyyy-MM-dd hh:mm:ss") <<
                                   "\t" << bid_value << "\t" << ask_value << "\t" << price_value << endl;
            }
        }
        bitstamp_stream << "FINISH" << endl;
    }
    if (!coinbase_txt_file.open(QIODevice::WriteOnly|QFile::WriteOnly)) {
        QMessageBox::warning(nullptr, "Could not save coinbase TXT file", QObject::tr( "\nCould not save coinbase TXT file on disk"));
    }
    else {
        QTextStream coinbase_stream(&coinbase_txt_file);
        coinbase_stream << "coinbase PLOT DATA\n\nTIME\tBID\tASK\tLAST\n";
        QList<double> coinbase_keys_values = coinbase_chart->keys->toList();
        QList<double> coinbase_bid_values = coinbase_chart->bid_values->toList();
        QList<double> coinbase_ask_values = coinbase_chart->ask_values->toList();
        QList<double> coinbase_price_values = coinbase_chart->price_values->toList();
        for (int i = 0; i < coinbase_keys_values.length(); i++) {
            double bid_value = coinbase_bid_values[i];
            double ask_value = coinbase_ask_values[i];
            double price_value = coinbase_price_values[i];
            if (static_cast<int>(bid_value) != -1 && static_cast<int>(ask_value) != -2 && static_cast<int>(price_value) != -3) {
                coinbase_stream << i << QDateTime::fromTime_t(static_cast<uint>(coinbase_keys_values[i])).toString("yyyy-MM-dd hh:mm:ss") <<
                                   "\t" << bid_value << "\t" << ask_value << "\t" << price_value << endl;
            }
        }
        coinbase_stream << "FINISH" << endl;
    }
}

void MainWindow::on_actionCSV_triggered()
{
    QString dirname = QFileDialog::getExistingDirectory(this,
                                                        tr("Select a directory to save CSV files"),
                                                        "");

    QString bitfinex_txt_path(QString("%1/bitfinex_csv_%2.csv").arg(dirname).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss")));
    QString bitstamp_txt_path(QString("%1/bitstamp_csv_%2.csv").arg(dirname).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss")));
    QString coinbase_txt_path(QString("%1/coinbase_csv_%2.csv").arg(dirname).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss")));

    QFile bitfinex_txt_file(bitfinex_txt_path);
    QFile bitstamp_txt_file(bitstamp_txt_path);
    QFile coinbase_txt_file(coinbase_txt_path);

    if (!bitfinex_txt_file.open(QIODevice::WriteOnly|QFile::WriteOnly)) {
        QMessageBox::warning(nullptr, "Could not save bitfinex CSV file", QObject::tr( "\nCould not save bitfinex CSV file on disk"));
    }
    else {
        QTextStream bitfinex_stream(&bitfinex_txt_file);
        bitfinex_stream << "time,bid,ask,last" << endl;
        QList<double> bitfinex_keys_values = bitfinex_chart->keys->toList();
        QList<double> bitfinex_bid_values = bitfinex_chart->bid_values->toList();
        QList<double> bitfinex_ask_values = bitfinex_chart->ask_values->toList();
        QList<double> bitfinex_price_values = bitfinex_chart->price_values->toList();
        for (int i = 0; i < bitfinex_keys_values.length(); i++) {
            double bid_value = bitfinex_bid_values[i];
            double ask_value = bitfinex_ask_values[i];
            double price_value = bitfinex_price_values[i];
            if (static_cast<int>(bid_value) != -1 && static_cast<int>(ask_value) != -2 && static_cast<int>(price_value) != -3) {
                bitfinex_stream << QDateTime::fromTime_t(static_cast<uint>(bitfinex_keys_values[i])).toString("yyyy-MM-dd hh:mm:ss") <<
                                   "," << bid_value << "," << ask_value << "," << price_value << endl;
            }
        }
    }
    if (!bitstamp_txt_file.open(QIODevice::WriteOnly|QFile::WriteOnly)) {
        QMessageBox::warning(nullptr, "Could not save bitstamp CSV file", QObject::tr( "\nCould not save bitstamp CSV file on disk"));
    }
    else {
        QTextStream bitstamp_stream(&bitstamp_txt_file);
        bitstamp_stream << "time,bid,ask,last" << endl;
        QList<double> bitstamp_keys_values = bitstamp_chart->keys->toList();
        QList<double> bitstamp_bid_values = bitstamp_chart->bid_values->toList();
        QList<double> bitstamp_ask_values = bitstamp_chart->ask_values->toList();
        QList<double> bitstamp_price_values = bitstamp_chart->price_values->toList();
        for (int i = 0; i < bitstamp_keys_values.length(); i++) {
            double bid_value = bitstamp_bid_values[i];
            double ask_value = bitstamp_ask_values[i];
            double price_value = bitstamp_price_values[i];
            if (static_cast<int>(bid_value) != -1 && static_cast<int>(ask_value) != -2 && static_cast<int>(price_value) != -3) {
                bitstamp_stream << QDateTime::fromTime_t(static_cast<uint>(bitstamp_keys_values[i])).toString("yyyy-MM-dd hh:mm:ss") <<
                                   "," << bid_value << "," << ask_value << "," << price_value << endl;
            }
        }
    }
    if (!coinbase_txt_file.open(QIODevice::WriteOnly|QFile::WriteOnly)) {
        QMessageBox::warning(nullptr, "Could not save coinbase CSV file", QObject::tr( "\nCould not save coinbase CSV file on disk"));
    }
    else {
        QTextStream coinbase_stream(&coinbase_txt_file);
        coinbase_stream << "time,bid,ask,last" << endl;
        QList<double> coinbase_keys_values = coinbase_chart->keys->toList();
        QList<double> coinbase_bid_values = coinbase_chart->bid_values->toList();
        QList<double> coinbase_ask_values = coinbase_chart->ask_values->toList();
        QList<double> coinbase_price_values = coinbase_chart->price_values->toList();
        for (int i = 0; i < coinbase_keys_values.length(); i++) {
            double bid_value = coinbase_bid_values[i];
            double ask_value = coinbase_ask_values[i];
            double price_value = coinbase_price_values[i];
            if (static_cast<int>(bid_value) != -1 && static_cast<int>(ask_value) != -2 && static_cast<int>(price_value) != -3) {
                coinbase_stream << QDateTime::fromTime_t(static_cast<uint>(coinbase_keys_values[i])).toString("yyyy-MM-dd hh:mm:ss") <<
                                   "," << bid_value << "," << ask_value << "," << price_value << endl;
            }
        }
    }
}

void MainWindow::on_actionAs_png_image_triggered()
{
    QString dirname = QFileDialog::getExistingDirectory(this,
                                                        tr("Select a directory to save PNG images"),
                                                        "");

    QString bitfinex_img_path(QString("%1/bitfinex_img_%2.png").arg(dirname).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss")));
    QString bitstamp_img_path(QString("%1/bitstamp_img_%2.png").arg(dirname).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss")));
    QString coinbase_img_path(QString("%1/coinbase_img_%2.png").arg(dirname).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss")));

    QFile bitfinex_img_file(bitfinex_img_path);
    QFile bitstamp_img_file(bitstamp_img_path);
    QFile coinbase_img_file(coinbase_img_path);

    if (!bitfinex_img_file.open(QIODevice::WriteOnly|QFile::WriteOnly)) {
        QMessageBox::warning(nullptr, "Could not save bitfinex picture", QObject::tr( "\nCould not save bitfinex PNG picture on disk"));
    }
    else {
        ui->bitfinexdata_wdgt->savePng(bitfinex_img_path);
    }
    if (!bitstamp_img_file.open(QIODevice::WriteOnly|QFile::WriteOnly)) {
        QMessageBox::warning(nullptr, "Could not save bitstamp picture", QObject::tr( "\nCould not save bitstamp PNG picture on disk"));
    }
    else {
        ui->bitstampdata_wdgt->savePng(bitstamp_img_path);
    }
    if (!coinbase_img_file.open(QIODevice::WriteOnly|QFile::WriteOnly)) {
        QMessageBox::warning(nullptr, "Could not save coinbase picture", QObject::tr( "\nCould not save coinbase PNG picture on disk"));
    }
    else {
        ui->coinbasedata_wdgt->savePng(coinbase_img_path);
    }
}
