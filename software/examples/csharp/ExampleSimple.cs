using System;
using System.Threading;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Industrial Quad Relay Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialQuadRelayV2 iqr =
		  new BrickletIndustrialQuadRelayV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Turn relays alternating on/off 10 times with 100 ms delay
		for(int i = 0; i < 10; i++)
		{
			Thread.Sleep(100);
			iqr.SetValue(new bool[]{true, false, false, false});
			Thread.Sleep(100);
			iqr.SetValue(new bool[]{false, true, false, false});
			Thread.Sleep(100);
			iqr.SetValue(new bool[]{false, false, true, false});
			Thread.Sleep(100);
			iqr.SetValue(new bool[]{false, false, false, true});
		}

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
