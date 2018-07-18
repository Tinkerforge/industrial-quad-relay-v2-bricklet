#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_industrial_quad_relay_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Industrial Quad Relay Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
iqr = BrickletIndustrialQuadRelayV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Turn relays alternating on/off 10 times with 100 ms delay
for _ in 0..9
  sleep 0.1
  iqr.set_value [true, false, false, false]
  sleep 0.1
  iqr.set_value [false, true, false, false]
  sleep 0.1
  iqr.set_value [false, false, true, false]
  sleep 0.1
  iqr.set_value [false, false, false, true]
end

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
