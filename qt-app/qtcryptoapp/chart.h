#ifndef CHART_H
#define CHART_H

#include <QDateTime>

#include <QWidget>

#include <QList>

#include <QGridLayout>
#include <QtCharts>

class Chart
{
public:
    Chart(QWidget *parent_widget);

    QList< QList<float> > points_y_data;
    QList< QDateTime > points_x_data;

    void add_point(float bid, float ask, float price);

private:
    void setup_chart(void);
    void gui_setup_chart(QWidget *parent_widget);

    QtCharts::QChart *chart;
    QtCharts::QChartView *chart_view;

    QtCharts::QDateTimeAxis *axisX;
    QtCharts::QValueAxis *axisY;

    QtCharts::QLineSeries *bid_series;
    QtCharts::QLineSeries *ask_series;
    QtCharts::QLineSeries *price_series;

    QGridLayout *grid_layout;

    QString MAX_POINTS_ON_GRAPH;
};

#endif // CHART_H
