#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialQuadRelayV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Industrial Quad Relay Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $iqr = Tinkerforge::BrickletIndustrialQuadRelayV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Turn relays alternating on/off 10 times with 100 ms delay
for (my $i = 0; $i < 10; $i++)
{
    select(undef, undef, undef, 0.1);
    $iqr->set_value([1, 0, 0, 0]);
    select(undef, undef, undef, 0.1);
    $iqr->set_value([0, 1, 0, 0]);
    select(undef, undef, undef, 0.1);
    $iqr->set_value([0, 0, 1, 0]);
    select(undef, undef, undef, 0.1);
    $iqr->set_value([0, 0, 0, 1]);
}

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
