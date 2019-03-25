#include "cmcscrapper.h"

#include <QFile>

CMCScrapper::CMCScrapper(QObject *parent) :
    QObject(parent)
{
}

void CMCScrapper::make_request()
{
    qnam_cmc = new QNetworkAccessManager(this);
    connect(qnam_cmc, &QNetworkAccessManager::finished,
            this, &CMCScrapper::qnam_finished);
    qnam_cmc->get(QNetworkRequest(QUrl(QString("https://coinmarketcap.com"))));
}

void CMCScrapper::qnam_finished(QNetworkReply *reply)
{
    if (reply->error()) {
        qWarning() << "CMCScrapper::qnam_finished ERROR: " << reply->errorString();
        return;
    }
    else {
        QString cmc_data = reply->readAll();

        re_cmc.setPattern("<td(.*?)market-cap (.*?)>\n(.*?)\n");
        QRegularExpressionMatch marketcap_match = re_cmc.match(cmc_data);
        if (marketcap_match.hasMatch()) {
            market_cap = marketcap_match.captured(3);
        }

        re_cmc.setPattern("<a(.*?)\"price\"(.*?)>(.*?)</a>");
        QRegularExpressionMatch price_match = re_cmc.match(cmc_data);
        if (price_match.hasMatch()) {
            price = price_match.captured(3);
        }

        re_cmc.setPattern("<a(.*?)\"volume\"(.*?)>(.*?)</a>");
        QRegularExpressionMatch volume_match = re_cmc.match(cmc_data);
        if (volume_match.hasMatch()) {
            volume_24h = volume_match.captured(3);
        }

        re_cmc.setPattern("<span data-supply-container>(.*?)</span>");
        QRegularExpressionMatch supply_match = re_cmc.match(cmc_data);
        if (supply_match.hasMatch()) {
            circulating_supply = supply_match.captured(1);
        }

        re_cmc.setPattern("<td(.*?) percent-change (.*?)>(.*?)</td>");
        QRegularExpressionMatch change_match = re_cmc.match(cmc_data);
        if (change_match.hasMatch()) {
            change_24h = change_match.captured(3);
        }

        qDebug() << "CMCScrapper::qnam_finished" << market_cap << price << volume_24h << circulating_supply << change_24h;
    }
    reply->deleteLater();
}
