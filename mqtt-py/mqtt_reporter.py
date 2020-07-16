#!/usr/bin/env python3

"""A MQTT to InfluxDB Bridge

This script receives MQTT data and saves those to InfluxDB.

"""

import json
import re
from typing import NamedTuple

import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient

INFLUXDB_ADDRESS = 'stats.home.fidonode.me'
INFLUXDB_USER = ''
INFLUXDB_PASSWORD = ''
INFLUXDB_DATABASE = 'mydb'


MQTT_ADDRESS = 'stats.home.fidonode.me'
MQTT_TOPIC = '/topic/sensors/+'  # [temperature|humidity|battery|status]/[id]
MQTT_REGEX = '([^/]+)/([^/]+)'
MQTT_CLIENT_ID = 'MQTTInfluxDBBridge'

influxdb_client = InfluxDBClient(
    INFLUXDB_ADDRESS, 8086, INFLUXDB_USER, INFLUXDB_PASSWORD, None)


class SensorData(NamedTuple):
    location: str
    measurement: str
    value: float


def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)


def on_message(client, userdata, msg):
    """The callback for when a PUBLISH message is received from the server."""
    print(msg.topic + ' ' + str(msg.payload))
    parts = msg.topic.split("/")
    if len(parts) == 4:
        id = parts[3]
        sensor_data = json.loads(msg.payload.decode('utf-8'))
        type = sensor_data['type']
        if type == 'pressure':
            send_sensor_data_to_influxdb(
                "pressure", id, sensor_data['pressure'])
            send_sensor_data_to_influxdb(
                "temperature", id, sensor_data['temperature'])
            send_sensor_data_to_influxdb(
                "humidity", id, sensor_data['humidity'])
        elif type == 'co2':
            send_sensor_data_to_influxdb("co2", id, sensor_data['co2'])
        elif type == 'dust':
            send_sensor_data_to_influxdb("pm25", id, sensor_data['pm25'])
            send_sensor_data_to_influxdb("pm10", id, sensor_data['pm10'])


def send_sensor_data_to_influxdb(measurement, id, value):
    json_body = [
        {
            'measurement': measurement,
            'tags': {
                'id': id
            },
            'fields': {
                'value': value
            }
        }
    ]
    print(json.dumps(json_body))
    influxdb_client.write_points(json_body)


def _init_influxdb_database():
    databases = influxdb_client.get_list_database()
    if len(list(filter(lambda x: x['name'] == INFLUXDB_DATABASE, databases))) == 0:
        influxdb_client.create_database(INFLUXDB_DATABASE)
    influxdb_client.switch_database(INFLUXDB_DATABASE)


def main():
    _init_influxdb_database()

    mqtt_client = mqtt.Client(MQTT_CLIENT_ID)
    #mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.loop_forever()


if __name__ == '__main__':
    print('MQTT to InfluxDB bridge')
    main()
