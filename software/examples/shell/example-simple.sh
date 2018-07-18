#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Industrial Quad Relay Bricklet 2.0

# Turn relays alternating on/off 10 times with 100 ms delay
for i in 0 1 2 3 4 5 6 7 8 9; do
	sleep 0.1
	tinkerforge call industrial-quad-relay-v2-bricklet $uid set-value true,false,false,false
	sleep 0.1
	tinkerforge call industrial-quad-relay-v2-bricklet $uid set-value false,true,false,false
	sleep 0.1
	tinkerforge call industrial-quad-relay-v2-bricklet $uid set-value false,false,true,false
	sleep 0.1
	tinkerforge call industrial-quad-relay-v2-bricklet $uid set-value false,false,false,true
done
