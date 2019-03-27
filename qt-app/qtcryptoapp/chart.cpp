#include "chart.h"

#include <QDebug>

QT_CHARTS_USE_NAMESPACE

Chart::Chart(QWidget *parent_widget) :
    MAX_POINTS_ON_GRAPH(25)
{
    QDateTime current_date = QDateTime::currentDateTime();
    for (int i = 0; i < MAX_POINTS_ON_GRAPH; i++) {
        points_x_data.push_back(current_date.addSecs(-5 * i));
        points_y_data.push_back(QList< float >({-1, 1, 0}));
    }
    setup_chart();
    gui_setup_chart(parent_widget);
}

void Chart::add_point(float bid, float ask, float price)
{
    points_y_data.push_back(QList<float>({bid, ask, price}));

    // todo: shift when updating QList
}

void Chart::setup_chart()
{
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
    chart->addAxis(axisX, Qt::AlignBottom);
    bid_series->attachAxis(axisX);
    ask_series->attachAxis(axisX);
    price_series->attachAxis(axisX);

    axisY = new QtCharts::QValueAxis();
    axisY->setLabelFormat("%.02f");
    axisY->setTitleText("Value, USD");
    axisY->setTitleFont(QFont("Consolas", 8, 1, true));
    chart->addAxis(axisY, Qt::AlignLeft);
    bid_series->attachAxis(axisY);
    ask_series->attachAxis(axisY);
    price_series->attachAxis(axisY);

    chart->legend()->setAlignment(Qt::AlignLeft);

    for (int i = 0; i < points_x_data.length(); i++) {
//        qDebug() << points_x_data;
    }
    qDebug() << "??";

    QDateTime current_date = QDateTime::currentDateTime();
    for (int i = 0; i < MAX_POINTS_ON_GRAPH; i++) {
        bid_series->append(current_date.addSecs(-5 * i).toMSecsSinceEpoch(), i-20);
        ask_series->append(current_date.addSecs(-5 * i).toMSecsSinceEpoch(), i+20);
        price_series->append(current_date.addSecs(-5 * i).toMSecsSinceEpoch(), i);
        qDebug() << i << points_x_data.takeAt(i);
        //        bid_series->append(points_x_data->takeAt(0).toMSecsSinceEpoch(),
        //                           static_cast<qreal>(points_y_data->takeAt(i).takeAt(0)));
        //        ask_series->append(points_x_data->takeAt(0).toMSecsSinceEpoch(),
        //                           static_cast<qreal>(points_y_data->takeAt(i).takeAt(1)));
        //        price_series->append(points_x_data->takeAt(0).toMSecsSinceEpoch(),
        //                           static_cast<qreal>(points_y_data->takeAt(i).takeAt(2)));
    }
}

void Chart::gui_setup_chart(QWidget *parent_widget)
{
    chart_view = new QtCharts::QChartView(chart, parent_widget);
    chart_view->setRenderHint(QPainter::Antialiasing);

    parent_widget->setLayout(grid_layout);
    parent_widget->layout()->addWidget(chart_view);
}
