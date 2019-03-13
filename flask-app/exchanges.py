import bs4
import requests


class Coinmarketcap(object):
    def get_cmc_data(self, base):
        endpoint_path = "https://coinmarketcap.com"
        endpoint_response = requests.get(endpoint_path)
        soup = bs4.BeautifulSoup(endpoint_response.text, 'lxml')
        table = soup.find('table')
        for row in table.findAll('tr')[1:]:  # [1:] to skip table heading.
            span_name_elem = row.findAll('span', {'class': 'currency-symbol'})[0]
            name_elem = span_name_elem.findAll('a')[0]
            name = name_elem.contents[0].lower()
            if name == base:
                price_elem = row.findAll('a', {'class': 'price'})[0]
                price = price_elem.contents[0][1:]  # [1:] to remove trailing "$" sign.
                return float(price)
            else:
                continue
        return -1


class Coinbase(object):
    def get_bss_data(self, base, currency):
        coinbase_data = {
            'buy': self._get_coinbase_data('buy', base, currency),
            'sell': self._get_coinbase_data('sell', base, currency),
            'spot': self._get_coinbase_data('spot', base, currency)
        }
        return coinbase_data

    def _get_coinbase_data(self, data_type, base, currency):
        endpoint_template = "https://api.coinbase.com/v2/prices/{0}-{1}/{2}"
        endpoint_path = endpoint_template.format(base, currency, data_type)
        endpoint_response = requests.get(endpoint_path)
        if endpoint_response.status_code == 200:
            response_json = endpoint_response.json()
            return response_json.get('data').get('amount')
        else:
            return -1
