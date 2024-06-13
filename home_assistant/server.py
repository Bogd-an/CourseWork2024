# pip install paho-mqtt flask

import subprocess

def run_commands(commands):
    for command in commands:
        print(f'${command}')
        result = subprocess.run(command, shell=True)

from flask import Flask, request, jsonify
import sqlite3
from datetime import datetime
import json
import paho.mqtt.client as mqtt

app = Flask(__name__)

# Ключ для перевірки
SECRET_KEY = "qwerty1234"

# MQTT налаштування
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "sensor/jsondata"

# Підключення до бази даних SQLite та створення таблиці, якщо її ще немає
def init_db():
    conn = sqlite3.connect('database.db')
    print("Connected to database successfully")

    # Створення таблиці, якщо її ще немає
    conn.execute('''
        CREATE TABLE IF NOT EXISTS requests (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            pass TEXT,
            pir BOOLEAN,
            lock BOOLEAN,
            timestamp DATETIME
        )
    ''')
    print("Table created successfully")

    conn.close()

init_db()

# Функція для відправки даних в топік MQTT
def publish_to_mqtt(data):
    client = mqtt.Client()
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.publish(MQTT_TOPIC, json.dumps(data))
    client.disconnect()

# Маршрут для збереження JSON у базу даних
@app.route('/sensor', methods=['POST'])
def save_to_db():
    try:
        data = request.json
        print(f'{data = }')
        key = data.get('key')

        # Перевірка ключа
        if key != SECRET_KEY:
            return jsonify({"error": "Unauthorized"}), 403

        pass_str = data.get('pass')
        pir = data.get('pir')
        lock = data.get('lock')
        timestamp = datetime.now()

        # Підготовка даних для вставки та публікації
        db_data = (pass_str, pir, lock, timestamp)
        mqtt_data = {
            "pass": pass_str,
            "pir": pir,
            "lock": lock,
            "timestamp": timestamp.isoformat()
        }

        # Підключення до бази даних
        conn = sqlite3.connect('database.db')
        print("Connected to database successfully")

        # Вставка даних у базу даних
        conn.execute('INSERT INTO requests (pass, pir, lock, timestamp)'+
                     ' VALUES (?, ?, ?, ?)', db_data)
        conn.commit()
        conn.close()

        # Публікація даних в MQTT
        publish_to_mqtt(mqtt_data)

        return jsonify({"message": "Data saved successfully"}), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    run_commands([
        'docker compose build',
        'docker compose stop',
        'docker compose rm -f', 
        'docker compose up -d'
    ])
    app.run(host='0.0.0.0', port=1234, debug=True)
