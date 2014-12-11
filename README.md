rpi-drd
=======

Raspberry Pi powered robot

What is it?
=======
* The code that runs a Pi powered DAGU Multi-Chassis with two motors, encoders, and a camera
* Daemon or ncurses output
* Setup pins via config file (TODO)
* Fastcgi web interface for control and status (TODO)
* Camera output to the web interface (TODO)

Objectives
==========

1. ~Basic I/O, SIG handling~
1. ~Motor PWM output test~ Need software PWM :(
1. ~Motor Encoder input test~
1. Motor PWM/encoder PID loop
1. Encapsulate the above
1. Daemon mode
1. FastCGI interface
