#!/usr/bin/env python3

"""A MQTT to InfluxDB Bridge

This script receives MQTT data and saves those to InfluxDB.

"""

import re
from typing import NamedTuple

import paho.mqtt.client as mqtt

MQTT_ADDRESS = 'stats.home.fidonode.me'
MQTT_TOPIC = '/topic/sensors/+'  # [temperature|humidity|battery|status]/[id]
MQTT_REGEX = '([^/]+)/([^/]+)'
MQTT_CLIENT_ID = 'MQTT-listener'


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


def _parse_mqtt_message(topic, payload):
    match = re.match(MQTT_REGEX, topic)
    if match:
        measurement = match.group(1)
        location = match.group(2)
        if measurement == 'status':
            return None
        return SensorData(location, measurement, float(payload))
    else:
        return None


def main():

    mqtt_client = mqtt.Client(MQTT_CLIENT_ID)
    #mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.loop_forever()


if __name__ == '__main__':
    main()
