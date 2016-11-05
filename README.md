# sauron-system


![demo](https://github.com/ageapps/sauron-system/blob/master/art/sauron_icon.png?raw=true)


Project developed for the Carrefour Challenge. The challenge was to develop a solution on "stock breaks", wich is a big problem in large supermarkets.

## System Overview
![demo](https://github.com/ageapps/sauron-system/blob/master/art/system-overview.jpg?raw=true)

Sauron is a system consisted in a network of sensors located in the back of the shelfs. This way, the whole stock in the shop is monitorized and managed from a central server through an administration panel.

### Node Server
Web server developed in NodeJS that would be located in the central node. The server contains the Control Panel and a database with all active sensors in the network
The control panel interface was ntegrated with NodeJS using [gentella] bootstrap template.


### Sensor server
![demo](https://github.com/ageapps/sauron-system/blob/master/art/sauron-sensor.jpg?raw=true)

The sensor scans the shelf from the back to check for possible wholes. It´s continuously waiting for instructions from the central server.

Using a SoC with the ESP8266 chip we are able to create a server inside the sensor and get all the data that it is receiving from the ultrasonics sensors.

## Demo
[![DEMO](http://img.youtube.com/vi/5hS7yR61zh0/0.jpg)](http://www.youtube.com/watch?v=5hS7yR61zh0)


## Team
+ [Karen Flóres]
+ [Adrián García]
+ [Alejandro Gómez]
+ [David González]

## TODOs

+ Try with more sensors
+ Ultrasonic sensor is not acurrate enough

[Karen Flóres]:https://github.com/kareen2707
[Adrián García]:https://github.com/ageapps
[Alejandro Gómez]:https://github.com/Alejo2313
[David González]: https://github.com/dgfiloso
[gentella]: https://github.com/puikinsh/gentelella
