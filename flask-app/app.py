import os

from datetime import datetime

from flask import Flask, render_template, request, jsonify
from flask_sqlalchemy import SQLAlchemy


app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = os.environ['DATABASE_URL']
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = True
db = SQLAlchemy(app)

from database import Database

dbutils = Database()

@app.route('/')
def home():
    return render_template('home.html')


@app.route('/api/get')
def get():
    args = request.args
    fromtime_str_value = request.args.get('fromtime')
    if fromtime_str_value:
        if not fromtime_str_value:
            return jsonify({'error': 'incorrect api usage'})
        fromtime_crypto_entries = dbutils.get_btc_price(fromtime=fromtime_str_value)
        return jsonify([entry.as_dict() for entry in fromtime_crypto_entries])
    else:
        return jsonify({'error': 'incorrect api usage'})

