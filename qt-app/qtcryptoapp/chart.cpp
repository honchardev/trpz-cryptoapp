#include "chart.h"

#include <QDebug>

QT_CHARTS_USE_NAMESPACE

Chart::Chart(QWidget *chart_parent)
{
    // todo: fillup 20 values & shift when updating vector

    grid_layout = new QGridLayout();
    grid_layout->setMargin(0);

    bid_series = new QtCharts::QLineSeries();
    bid_series->setName("Bid");
    bid_series->setColor(QColor(255, 0, 0));
    ask_series = new QtCharts::QLineSeries();
    ask_series->setName("Ask");
    ask_series->setColor(QColor(0, 0, 255));
    price_series = new QtCharts::QLineSeries();
    price_series->setName("Price");
    price_series->setColor(QColor(1, 1, 1));

    chart = new QtCharts::QChart();

    chart->addSeries(bid_series);
    chart->addSeries(ask_series);
    chart->addSeries(price_series);

    chart->layout()->setContentsMargins(0, 0, 0, 0);

    axisX = new QtCharts::QDateTimeAxis();
    axisX->setTickCount(10);
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Time");
    axisX->setTitleFont(QFont("Consolas", 8, 1, true));
    axisX->setMax(QDateTime::currentDateTime());
    chart->addAxis(axisX, Qt::AlignBottom);
    bid_series->attachAxis(axisX);
    ask_series->attachAxis(axisX);
    price_series->attachAxis(axisX);

    axisY = new QtCharts::QValueAxis();
    axisY->setLabelFormat("%.02f");
    axisY->setTitleText("Value USD");
    axisY->setTitleFont(QFont("Consolas", 8, 1, true));
    chart->addAxis(axisY, Qt::AlignLeft);
    bid_series->attachAxis(axisY);
    ask_series->attachAxis(axisY);
    price_series->attachAxis(axisY);

    chart->legend()->setAlignment(Qt::AlignLeft);

    chart_view = new QtCharts::QChartView(chart, chart_parent);
    chart_view->setRenderHint(QPainter::Antialiasing);

    chart_parent->setLayout(grid_layout);
    chart_parent->layout()->addWidget(chart_view);
}

void Chart::add_point(float bid, float ask, float price)
{
    QVector<float> point_to_add({bid, ask, price});
    points_data.append(point_to_add);

//    bid_series->append()
}
