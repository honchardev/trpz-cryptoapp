import datetime

import pymysql


class Database(object):
    def __init__(self):
        self.con = pymysql.connect(
            host='localhost',
            port=3306,
            user='',
            password='',
            db='',
            cursorclass=pymysql.cursors.DictCursor
        )
        self.cur = self.con.cursor()

    def close_connection(self):
        self.cur.close()
        self.con.close()

    def create_tables(self):
        create_tables_script = """
        CREATE TABLE IF NOT EXISTS btcprices (
            btcprice_id INT         AUTO_INCREMENT,
            timestamp   DATETIME    NOT NULL,
            base        VARCHAR(25) NOT NULL,
            currency    VARCHAR(25) NOT NULL,
            buy         FLOAT       NOT NULL DEFAULT -1.0,
            sell        FLOAT       NOT NULL DEFAULT -1.0,
            spot        FLOAT       NOT NULL DEFAULT -1.0,
            cmc         FLOAT       NOT NULL DEFAULT -1.0,
            PRIMARY KEY (btcprice_id)
        ) ENGINE=INNODB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1;
        """
        self.cur.execute(create_tables_script)

    def add_btc_price(self, **params):
        table_column_names = ['timestamp', 'base', 'currency', 'buy', 'sell', 'spot', 'cmc']
        if all(column_name in table_column_names for column_name in params.keys()):
            script_values = (
                params.get('timestamp'),
                params.get('base'),
                params.get('currency'),
                params.get('buy'),
                params.get('sell'),
                params.get('spot'),
                params.get('cmc')
            )
            add_btc_price_entry_script = """
            INSERT INTO btcprices (timestamp, base, currency, buy, sell, spot, cmc)
            VALUES (%s, %s, %s, %s, %s, %s, %s);
            """
            self.cur.execute(add_btc_price_entry_script, script_values)
            self.con.commit()
        else:
            raise ValueError("Missing required fields for new btcprices entry.")

    def get_btc_price(self, **params):
        if params.get('onlylast') == True:
            get_btc_price_entry_script = """
            SELECT * FROM btcprices ORDER BY btcprice_id DESC LIMIT 1;
            """
            self.cur.execute(get_btc_price_entry_script)
            return self.cur.fetchone()
        elif params.get('fromtime'):
            script_values = (
                params.get('fromtime').strftime('%Y-%m-%d %H:%M:%S'),
                datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            )
            get_btc_price_entry_script = """
            SELECT * FROM btcprices
            WHERE timestamp BETWEEN %s AND %s;
            """
            self.cur.execute(get_btc_price_entry_script, script_values)
            return self.cur.fetchall()
        else:
            raise ValueError("Missing required fields to get btcprices entry.")
