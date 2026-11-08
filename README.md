# ESP8266 Wi-Fi Train Controller

A browser-based train motor controller built with an ESP8266. The ESP8266 creates its own Wi-Fi access point and hosts a web dashboard for direction and throttle control.

## Features

- ESP8266-hosted Wi-Fi access point
- Browser-based train control dashboard
- Forward, backward, and STOP commands
- Throttle slider from 0 to 1023
- Speed gauge and periodic speed refresh
- Motor output through GPIO pins D1 and D2

## Hardware

- ESP8266 development board (for example, NodeMCU)
- Motor driver compatible with the motor and supply voltage
- DC train motor
- Suitable external motor power supply
- Jumper wires

**Do not connect a motor directly to ESP8266 GPIO pins.** Use a suitable motor driver, and connect the ESP8266 ground to the motor-driver ground. Ensure the motor supply and driver are correctly rated.

## Pin connections

| ESP8266 pin | Motor driver input |
|---|---|
| D1 | IN1 |
| D2 | IN2 |
| GND | GND (common ground) |

Connect the motor and its power supply to the motor driver's motor-output and power terminals according to that driver's documentation.

## Wi-Fi credentials — configure before use

The sketch intentionally does not contain the original hardcoded password. Before uploading, edit this line in `train_AP.ino`:

```cpp
const char* password = "CHANGE_THIS_PASSWORD";
```

Replace `CHANGE_THIS_PASSWORD` with your own unique WPA2 password of at least 8 characters. Do not commit a real personal or reused password to a public repository. If a real password was previously published, change it.

The access point SSID is currently:

```cpp
const char* ssid = "Train-Controller-AP";
```

You can change the SSID if required.

## Software setup

1. Install the Arduino IDE.
2. Install the ESP8266 board package using Boards Manager.
3. Open `train_AP.ino`.
4. Select the correct ESP8266 board and serial port.
5. Set your own Wi-Fi AP password as described above.
6. Upload the sketch.

## How to operate

1. Power the ESP8266 and motor-driver circuit.
2. Connect a phone, tablet, or computer to the `Train-Controller-AP` Wi-Fi network.
3. Enter the password configured in the sketch.
4. Open the access-point IP address printed in the Arduino IDE Serial Monitor (115200 baud). The ESP8266 soft-AP default is commonly `192.168.4.1`.
5. Use Forward, Backward, and STOP to control direction.
6. Adjust the throttle slider to set the PWM value from 0 to 1023.

## Important safety notes

- Test with the train wheels lifted or the motor mechanically unloaded first.
- Start with a low throttle value.
- Provide a motor driver and supply appropriate for the motor's stall current.
- Add appropriate hardware protection and an emergency power cutoff for a physical train installation.
- The web control endpoints are unauthenticated beyond joining the Wi-Fi access point. Anyone who joins the AP can issue motor commands. Use a strong AP password and operate in a controlled environment.

## Implementation notes

- `IN1` is mapped to D1 and `IN2` to D2.
- PWM range is configured to 1023 at 1 kHz.
- The dashboard uses Gauge.js from jsDelivr for the speed gauge, so that visual gauge requires internet access unless the library is hosted locally. The basic control requests are served by the ESP8266 itself.
- This project has not been physically tested as part of this repository preparation. Verify board compatibility, motor-driver wiring, and behavior with the actual hardware before use.

## License

MIT License. See [LICENSE](LICENSE).
