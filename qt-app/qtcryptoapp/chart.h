#ifndef CHART_H
#define CHART_H

#include <QWidget>

#include <QVector>

#include <QGridLayout>
#include <QtCharts>

class Chart
{
public:
    Chart(QWidget *chart_parent);

    QVector< QVector<float> > points_data;

    void add_point(float bid, float ask, float price);

private:
    QtCharts::QChart *chart;
    QtCharts::QChartView *chart_view;

    QtCharts::QDateTimeAxis *axisX;
    QtCharts::QValueAxis *axisY;

    QtCharts::QLineSeries *bid_series;
    QtCharts::QLineSeries *ask_series;
    QtCharts::QLineSeries *price_series;

    QGridLayout *grid_layout;
};

#endif // CHART_H
