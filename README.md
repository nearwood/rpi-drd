rpi-drd
=======

Raspberry Pi powered robot. Eventually semi-autonomous.

## What is it?
* The code that runs a Pi powered DAGU Multi-Chassis with two motors, encoders, and a camera
* Basically just a $250, multiple man-month long project to antagonize my cats
* Daemon or ncurses output
* Setup pins via config file (TODO)
* Fastcgi web interface for control and status (TODO)
* Camera output to the web interface (TODO)

### Modules
1. Server - The intermediary interface between the Web module and Controller. This is the main entry point.
1. Controller - The motor controller interface. Steering, driving, camera controller.
1. Web - The static website with AJAX controls.

## Objectives
 - [x] <del>Basic I/O, SIG handling</del>
 - [x] <del>Motor PWM output test</del>
 - [x] <del>Motor Encoder input test</del>
 - [x] Motor PWM/encoder PID loops
1. Encapsulate the above
1. Daemon mode
1. FastCGI interface

## Hardware
### Required:
* Raspberry Pi B+ (need the 2nd PWM output pin)
* DAGU Multi-Chassis
* 2 Motor Encoders
* Dual channel PWM motor controller
* Lots of batteries

#### Optional:
* Pi Camera
* Distance Sensors

## Software
### Build Dependancies
* bcm2835
* ncurses
* rt

### Deploy Dependancies
* rsync/scp

#### Run Dependancies
* lighttpd / apache
* fastcgi
* spawn-fcgi

## Building
For ease of development, it is cross-compiled.

bcm2835:
 `./configure --build=arm-linux --host=arm-rpi-linux-gnueabi --prefix=/home/nick/dev/rpi`

Since I don't know enough about make to have it autodetect/find the right toolchain, run this before running make:
`export PATH=$PATH:/home/nick/x-tools/arm-rpi-linux-gnueabi/bin`

## Installation/Deployment

1. Install packages on the Pi:
  * pacman -S lighttpd fcgi spawn-fcgi rsync ncurses
1. configure lighttpd and fcgi on the Pi
1. make upload

## Running the server process
 `spawn-fcgi -s /var/run/lighttpd/drd-server.socket -u alarm -U http -- /home/alarm/server`
