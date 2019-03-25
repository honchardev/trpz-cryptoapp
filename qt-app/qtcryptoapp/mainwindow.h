#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    DBWrapper *dbwrapper;

    Exchange *bitfinex;
    Exchange *bitstamp;
    Coinbase *coinbase;
};

#endif // MAINWINDOW_H
