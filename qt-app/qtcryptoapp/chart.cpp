#include "chart.h"

Chart::Chart(QObject *parent, QCustomPlot *plot_widget) :
    QObject(parent),
    plot_widget(plot_widget)
{
    create_plot();

    plot_cursor = new QCPItemLine(plot_widget);

    connect(plot_widget, SIGNAL(mouseMove(QMouseEvent*)),
            this, SLOT(on_hover_show_data(QMouseEvent*)));
}

Chart::~Chart()
{
    delete keys;
    delete bid_values;
    delete ask_values;
    delete price_values;
    delete plot_widget;
    delete plot_cursor;
}

void Chart::add_data_point(double timestamp, double bid, double ask, double price)
{
    keys->append(timestamp);
    bid_values->append(bid);
    ask_values->append(ask);
    price_values->append(price);

    if (bid > max_graph_y_value) max_graph_y_value = bid;
    if (bid < min_graph_y_value) min_graph_y_value = bid;
    if (ask > max_graph_y_value) max_graph_y_value = ask;
    if (ask < min_graph_y_value) min_graph_y_value = ask;
    if (price > max_graph_y_value) max_graph_y_value = price;
    if (price < min_graph_y_value) min_graph_y_value = price;

    plot_widget->graph(0)->addData(timestamp, bid);
    plot_widget->graph(1)->addData(timestamp, ask);
    plot_widget->graph(2)->addData(timestamp, price);

    plot_widget->yAxis->rescale();
    plot_widget->yAxis->setRange(min_graph_y_value - 0.5, max_graph_y_value + 0.5);
    plot_widget->xAxis->setRange(timestamp - 1*15*100, timestamp + 15);

    plot_widget->replot();
}

void Chart::on_hover_show_data(QMouseEvent *event)
{
    double hovered_y_value = plot_widget->yAxis->pixelToCoord(event->pos().y());
    plot_widget->setToolTip(QString("%1 USD").arg(hovered_y_value));
}

void Chart::create_plot()
{
    QDateTime current_time = QDateTime::currentDateTime();
    double min_time_value = current_time.addSecs(-1 * 15 * 100).toTime_t(); // 25 min ago

    keys = new QVector<double>(100);
    bid_values = new QVector<double>(100);
    ask_values = new QVector<double>(100);
    price_values = new QVector<double>(100);
    for (int j = 0; j < 100; j++) {
        (*keys)[j] = min_time_value + 15*j;
        (*bid_values)[j] = -1;
        (*ask_values)[j] = -2;
        (*price_values)[j] = -3;
    }

    max_graph_y_value = 0;
    min_graph_y_value = 1e6;

    plot_widget->addGraph();
    plot_widget->graph(0)->setPen(QPen(QColor(255, 1, 1), 4));
    plot_widget->graph(0)->setData(*keys, *bid_values);
    plot_widget->addGraph();
    plot_widget->graph(1)->setPen(QPen(QColor(1, 255, 1), 4));
    plot_widget->graph(1)->setData(*keys, *ask_values);
    plot_widget->addGraph();
    plot_widget->graph(2)->setPen(QPen(QColor(1, 1, 1), 4));
    plot_widget->graph(2)->setData(*keys, *price_values);

    QSharedPointer<QCPAxisTickerDateTime> time_ticker(new QCPAxisTickerDateTime);
    time_ticker->setDateTimeFormat("hh:mm:ss");
    plot_widget->xAxis->setTicker(time_ticker);

    plot_widget->xAxis->setTickLabelFont(QFont("Consolas", 8));
    plot_widget->yAxis->setTickLabelFont(QFont("Consolas", 8));
    plot_widget->yAxis2->setTickLabelFont(QFont("Consolas", 8));

    plot_widget->xAxis->setLabel("Time");
    plot_widget->yAxis->setLabel("Value, USD");

    plot_widget->xAxis2->setVisible(true);
    plot_widget->xAxis2->setTicks(false);
    plot_widget->xAxis2->setTickLabels(false);
    plot_widget->yAxis2->setVisible(true);
    plot_widget->yAxis2->setTicks(true);
    plot_widget->yAxis2->setTickLabels(true);

    plot_widget->legend->setVisible(true);
    plot_widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    plot_widget->legend->setFont(QFont("Consolas", 8));
    plot_widget->legend->setBrush(QColor(255, 255, 255, 150));

    connect(plot_widget->xAxis, SIGNAL(rangeChanged(QCPRange)),
            plot_widget->xAxis2, SLOT(setRange(QCPRange)));
    connect(plot_widget->yAxis, SIGNAL(rangeChanged(QCPRange)),
            plot_widget->yAxis2, SLOT(setRange(QCPRange)));

    plot_widget->rescaleAxes();
    plot_widget->xAxis->setRange(min_time_value, current_time.toTime_t() + 15);
    plot_widget->yAxis->setRange(plot_widget->yAxis->range().lower - 10,
                                 plot_widget->yAxis->range().upper + 10);
    plot_widget->replot();
}
