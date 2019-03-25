#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>

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
    void on_bitfinex_btn_clicked();
    void on_bitstamp_btn_clicked();
    void on_coinbase_btn_clicked();

private:
    Ui::MainWindow *ui;

    CMCScrapper *cmcscrapper;

    DBWrapper *dbwrapper;

    BitfinexExchange *bitfinex;
    BitstampExchange *bitstamp;
    CoinbaseExchange *coinbase;
};

#endif // MAINWINDOW_H
