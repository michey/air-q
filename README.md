# ESP32 Air quality measurement dev

### Overview
Combination of ESP-32, SDS011, MHZ-19B and BME-280. You can collect information about temperature, pressure, humidity, CO2 concentraton and ultrafine particles(pm2.5 and pm10). Device can be connecten to a/b/g/n Wi-Fi. It uses MQTT brocker to collect information. Each device reports its own stats to `/topic/sensors/{MAC_ADRESS}`. 
Each sensor reports its own data as separate JSON message.

F.E.
```
 {"type":"dust","pm25":1.1,"pm10":3.1}
 {"type":"pressure","pressure":756.90203579679394,"temperature":25.158885109786933,"humidity":31.470163959978709}
 {"type":"co2","co2":1494}
```
You can use `mqtt-py/mqtt_listener.py` for debug purposes and `mqtt-py/mqtt_reporter.py` to collect data from MQTT and store it in InfluxDB.

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py -p PORT flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.
