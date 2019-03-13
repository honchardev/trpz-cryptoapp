from datetime import datetime

from sqlalchemy import Column, DateTime, Float, Integer, String
from sqlalchemy.ext.declarative import DeclarativeMeta

from app import db


class BTCPrice(db.Model):
    __tablename__ = 'btcprices'

    btcprice_id = Column(Integer, primary_key=True)
    timestamp = Column(DateTime, nullable=False)
    base = Column(String(25), nullable=False)
    currency = Column(String(25), nullable=False)
    buy = Column(Float, nullable=False, default=-1.0)
    sell = Column(Float, nullable=False, default=-1.0)
    spot = Column(Float, nullable=False, default=-1.0)
    cmc = Column(Float, nullable=False, default=-1.0)

    def as_dict(self):
        return {
            "timestamp": datetime.strftime(self.timestamp, '%Y-%m-%d %H:%M:%S'),
            "base": self.base,
            "currency": self.currency,
            "buy": self.buy,
            "sell": self.sell,
            "spot": self.spot,
            "cmc": self.cmc
        }

    def __init__(self, ts, base, cur, buy, sell, spot, cmc):
        self.timestamp = ts
        self.base = base
        self.currency = cur
        self.buy = buy
        self.sell = sell
        self.spot = spot
        self.cmc = cmc

    def __repr__(self):
        return '<id {0}. values {1}>'.format(self.btcprice_id, self.as_dict())
