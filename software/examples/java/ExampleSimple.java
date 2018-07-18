import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialQuadRelayV2;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Industrial Quad Relay Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialQuadRelayV2 iqr =
		  new BrickletIndustrialQuadRelayV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Turn relays alternating on/off 10 times with 100 ms delay
		for(int i = 0; i < 10; i++) {
			Thread.sleep(100);
			iqr.setValue(new boolean[]{true, false, false, false});
			Thread.sleep(100);
			iqr.setValue(new boolean[]{false, true, false, false});
			Thread.sleep(100);
			iqr.setValue(new boolean[]{false, false, true, false});
			Thread.sleep(100);
			iqr.setValue(new boolean[]{false, false, false, true});
		}

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
