function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIndustrialQuadRelayV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Industrial Quad Relay Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    iqr = handle(BrickletIndustrialQuadRelayV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Turn relays alternating on/off 10 times with 100 ms delay
    for i = 0:9
        pause(0.1);
        iqr.setValue([true false false false]);
        pause(0.1);
        iqr.setValue([false true false false]);
        pause(0.1);
        iqr.setValue([false false true false]);
        pause(0.1);
        iqr.setValue([false false false true]);
    end

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
