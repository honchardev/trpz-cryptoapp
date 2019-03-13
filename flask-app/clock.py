from datetime import datetime

from apscheduler.schedulers.blocking import BlockingScheduler

from app import dbutils
from exchanges import Coinbase, Coinmarketcap


cb = Coinbase()
cmc = Coinmarketcap()
sched = BlockingScheduler()


@sched.scheduled_job('interval', minutes=999)
def scheduler_job():
    base = 'btc'
    currency = 'usd'
    coinmarketcap_data = cmc.get_cmc_data(base)
    coinbase_data = cb.get_bss_data(base, currency)
    dbutils.add_btc_price(
        timestamp=datetime.now(),
        base=base,
        currency=currency,
        buy=coinbase_data.get('buy'),
        sell=coinbase_data.get('sell'),
        spot=coinbase_data.get('spot'),
        cmc=coinmarketcap_data,
    )

sched.start()
