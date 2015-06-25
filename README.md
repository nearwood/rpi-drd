rpi-drd
=======

Raspberry Pi powered robot. Eventually semi-autonomous.

## What is it?
* The code that runs a Pi powered DAGU Multi-Chassis with two motors, encoders, and a camera
* Basically just a $250 man-month long project to antagonize my cats
* Daemon or ncurses output
* Setup pins via config file (TODO)
* Fastcgi web interface for control and status (TODO)
* Camera output to the web interface (TODO)

## Objectives
1. <del>Basic I/O, SIG handling</del>
1. <del>Motor PWM output test</del>
1. <del>Motor Encoder input test</del>
1. Motor PWM/encoder PID loops
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

## Software Dependancies
* bcm2835
* ncurses
* rt

## Building
For ease of development, it is cross-compiled.

bcm2835:
`./configure --build=arm-linux --host=arm-rpi-linux-gnueabi --prefix=/home/nick/dev/rpi`

rpi-drd:
`export PATH=$PATH:/home/nick/x-tools/arm-rpi-linux-gnueabi/bin`
