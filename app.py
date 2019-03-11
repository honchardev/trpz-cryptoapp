import atexit
from datetime import datetime

from flask import Flask, render_template, request, jsonify

from database import Database
from exchanges import Coinbase, Coinmarketcap
from scheduler import Scheduler

db = Database()
cb = Coinbase()
cmc = Coinmarketcap()
app = Flask(__name__)


@app.route('/')
def home():
    return render_template('home.html')


@app.route('/api/get')
def get():
    args = request.args
    if args:
        fromtime_str_value = request.args.get('fromtime')
        if not fromtime_str_value:
            return jsonify({'error': 'incorrect api usage'})
        fromtime_datetime_obj = datetime.strptime(fromtime_str_value, '%Y-%m-%d %H:%M:%S')
        fromtime_crypto_entries = db.get_btc_price(fromtime=fromtime_datetime_obj)
        return jsonify(fromtime_crypto_entries)
    else:
        last_crypto_entry = db.get_btc_price(onlylast=True)
        return jsonify(last_crypto_entry)


def scheduler_job():
    base = 'btc'
    currency = 'usd'
    coinmarketcap_data = cmc.get_cmc_data(base)
    coinbase_data = cb.get_bss_data(base, currency)
    db.add_btc_price(
        timestamp=datetime.now(),
        base=base,
        currency=currency,
        buy=coinbase_data.get('buy'),
        sell=coinbase_data.get('sell'),
        spot=coinbase_data.get('spot'),
        cmc=coinmarketcap_data,
    )


if __name__ == "__main__":
    db.create_tables()
    schdl = Scheduler(scheduler_job, 10)
    atexit.register(lambda: db.close_connection)
    atexit.register(lambda: schdl.bs.shutdown)
    app.run(host='0.0.0.0', port=5000)
