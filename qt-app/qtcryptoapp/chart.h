#ifndef CHART_H
#define CHART_H

#include <QObject>
#include <QWidget>
#include <QVector>

#include "qcustomplot.h"

#include "dbwrapper.h"

class Chart : public QObject
{
    Q_OBJECT

public:
    explicit Chart(QObject *parent = nullptr, QCustomPlot *plot_widget = nullptr);
    ~Chart();

    void add_data_point(double timestamp, double bid, double ask, double price);

    QVector<double> *keys;
    QVector<double> *bid_values;
    QVector<double> *ask_values;
    QVector<double> *price_values;

private slots:
    void on_hover_show_data(QMouseEvent *event);

private:
    void create_plot(void);

    QCustomPlot *plot_widget;

    QCPItemLine *plot_cursor;

    double max_graph_y_value;
    double min_graph_y_value;
};

#endif // CHART_H
