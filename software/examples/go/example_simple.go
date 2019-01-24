package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/industrial_quad_relay_v2_bricklet"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"time"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Industrial Quad Relay Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	iqr, _ := industrial_quad_relay_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Turn relays alternating on/off 10 times with 100 ms delay
	for i := 0; i < 10; i++ {
		time.Sleep(100 * time.Millisecond)
		iqr.SetValue([4]bool{true, false, false, false})
		time.Sleep(100 * time.Millisecond)
		iqr.SetValue([4]bool{false, true, false, false})
		time.Sleep(100 * time.Millisecond)
		iqr.SetValue([4]bool{false, false, true, false})
		time.Sleep(100 * time.Millisecond)
		iqr.SetValue([4]bool{false, false, false, true})
	}

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
