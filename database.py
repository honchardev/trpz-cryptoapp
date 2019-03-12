from datetime import datetime

from sqlalchemy.exc import DatabaseError

from models import BTCPrice

from app import db


class Database(object):

    def __init__(self):
        self.cached_last_saved_value = -1

    def add_btc_price(self, **params):
        table_column_names = ['timestamp', 'base', 'currency', 'buy', 'sell', 'spot', 'cmc']
        if all(column_name in table_column_names for column_name in params.keys()):
            new_btcprice = BTCPrice(
                ts=params.get('timestamp'),
                base=params.get('base'),
                cur=params.get('currency'),
                buy=params.get('buy'),
                sell=params.get('sell'),
                spot=params.get('spot'),
                cmc=params.get('cmc')
            )
            db.session.add(new_btcprice)
            db.session.commit()
            self.cached_last_saved_value = dict(params)
        else:
            raise ValueError("Missing required fields for new btcprices entry.")

    def get_btc_price(self, **params):
        if params.get('onlylast') == True:
            return self.cached_last_saved_value
        elif params.get('fromtime'):
            from_time = datetime.strptime(params.get('fromtime'), '%Y-%m-%d %H:%M:%S')
            btc_prices = BTCPrice.query.filter(BTCPrice.timestamp > from_time).all()
            return btc_prices
        else:
            raise ValueError("Missing required fields to get btcprices entry.")
