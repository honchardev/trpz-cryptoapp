import json
from datetime import datetime

from sqlalchemy.exc import DatabaseError

from app import db
from models import BTCPrice


class Database(object):

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
            print('added {0} to add_btc_price'.format(new_btcprice.as_dict()))
        else:
            raise ValueError("Missing required fields for new btcprices entry.")

    def get_btc_price(self, **params):
        if params.get('fromtime'):
            from_time = datetime.strptime(params.get('fromtime'), '%Y-%m-%d %H:%M:%S')
            btc_prices = BTCPrice.query.filter(BTCPrice.timestamp > from_time).all()
            return btc_prices
        else:
            raise ValueError("Missing required fields to get btcprices entry.")
