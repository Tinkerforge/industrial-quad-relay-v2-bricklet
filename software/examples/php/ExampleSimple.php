<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialQuadRelayV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialQuadRelayV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Industrial Quad Relay Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$iqr = new BrickletIndustrialQuadRelayV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Turn relays alternating on/off 10 times with 100 ms delay
for($i = 0; $i < 10; $i++) {
    usleep(100*1000);
    $iqr->setValue(array(TRUE, FALSE, FALSE, FALSE));
    usleep(100*1000);
    $iqr->setValue(array(FALSE, TRUE, FALSE, FALSE));
    usleep(100*1000);
    $iqr->setValue(array(FALSE, FALSE, TRUE, FALSE));
    usleep(100*1000);
    $iqr->setValue(array(FALSE, FALSE, FALSE, TRUE));
}

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
