#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>

#include <QLabel>
#include <QFrame>
#include <QMessageBox>

#include <QVector>

#include "chart.h"
#include "cmcscrapper.h"
#include "dbwrapper.h"
#include "exchange.h"

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
    void updategui_db_conn_ok();
    void updategui_db_conn_fail();

    void updategui_cmc_block_conn_ok();
    void updategui_cmc_block_conn_fail();

    void updategui_bitfinex_block_conn_ok();
    void updategui_bitfinex_block_conn_fail();

    void updategui_bitstamp_block_conn_ok();
    void updategui_bitstamp_block_conn_fail();

    void updategui_coinbase_block_conn_ok();
    void updategui_coinbase_block_conn_fail();

    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionQuit_triggered();

    void on_actionText_triggered();
    void on_actionCSV_triggered();
    void on_actionAs_png_image_triggered();

private:
    Ui::MainWindow *ui;

    QLabel *webconnstatus_lbl;
    QLabel *dbconnstatus_lbl;
    QFrame *statusbar_ln;

    Chart *bitfinex_chart;
    Chart *bitstamp_chart;
    Chart *coinbase_chart;

    DBWrapper *dbwrapper;

    CMCScrapper *cmcscrapper;

    BitfinexExchange *bitfinex;
    BitstampExchange *bitstamp;
    CoinbaseExchange *coinbase;
};

#endif // MAINWINDOW_H
