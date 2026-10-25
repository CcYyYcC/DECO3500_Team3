# Five-Board Firmware: Pinout, Installation, and Upload Guide

## 1. Each board has its own sketch

| No. | Device | Board selection in Arduino IDE | Complete sketch file |
|---|---|---|---|
| 1 | Exercise Token | Waveshare ESP32-S3-Zero | `Firmware/Token_Exercise_Final180/Token_Exercise_Final180.ino` |
| 2 | Study Token | Waveshare ESP32-S3-Zero | `Firmware/Token_Study_Final180/Token_Study_Final180.ino` |
| 3 | Gaming Token | Waveshare ESP32-S3-Zero | `Firmware/Token_Gaming_Final180/Token_Gaming_Final180.ino` |
| 4 | Tower communication bridge | Waveshare ESP32-S3-Zero | `Firmware/Tower_Bridge_DemoCycle_v3/Tower_Bridge_DemoCycle_v3.ino` |
| 5 | Tower LED controller | Arduino UNO R4 Minima | `Firmware/R4_Tower_Breath5s_B20/R4_Tower_Breath5s_B20.ino` |

Every `.ino` is a **complete independent Arduino sketch**. Do not combine all five files into one sketch folder or compile them as a single program. Use **115200 baud** for the USB Serial Monitors. The Tower ESP32 ↔ R4 hardware UART runs at **9600 baud**.

## 2. Required libraries

- **Token ESP32 boards:** `SparkFun MMA8452Q Accelerometer` (three-axis acceleration data). The ESP32 board support package supplies WiFi, ESP-NOW, and related facilities.
- **Tower ESP32:** ESP-NOW, WiFi, `Preferences`, and other components provided by the ESP32 board support package.
- **UNO R4:** `Adafruit NeoPixel` for the 8×32 WS2812B panel.

## 3. Pin mapping

### All three personal Tokens

| Module connection | Token ESP32-S3-Zero pin |
|---|---|
| MMA8452Q SDA | GPIO1 |
| MMA8452Q SCL | GPIO2 |
| MAX7219 DIN | GPIO4 |
| MAX7219 CS | GPIO5 |
| MAX7219 CLK | GPIO6 |
| Reserved vibration output | GPIO45 (**does not mean vibration feedback has been verified**) |
| MAX7219 VCC / GND | Existing 5V/GND path; check power stability and signal-level compatibility |

### Time Tower

| Connection | Description |
|---|---|
| Tower ESP32 GPIO7 | Through the existing level converter → R4 D0/RX |
| R4 D1/TX | Through the existing level converter → Tower ESP32 GPIO8 |
| UART rate | 9600 baud |
| R4 D6 | WS2812B panel DIN |
| LED layout | Eight columns × 32 rows / 256 pixels; left x = 0..2, right x = 5..7 |
| GND | Appropriate common ground between R4, ESP32, level converter, and LED panel |

**Electrical safety warning:** The reported on-site wiring currently powers the panel through the R4's 5V path. A single successful test at brightness 20 is not enough to establish that this arrangement is safe for 256 WS2812B LEDs. Select a suitably rated, protected independent 5V power supply based on the panel's actual worst-case draw and verify power injection and wiring. Do not assume the R4 onboard regulator can supply an unknown high current. Do not connect two 5V supply outputs directly in parallel without an appropriate, verified power arrangement. **Disconnect all power before rewiring.**

## 4. Key program settings

| Location | Setting | Current value |
|---|---|---|
| Three Tokens | `DURATION` | 8000 ms: demo of one conceptual hour in eight seconds |
| Three Tokens | `CONFIRM_MS` | 250 ms |
| Three Tokens | `BRIGHTNESS` | 0: MAX7219 lowest intensity setting |
| Three Tokens | `RETURN_SWAP_MS` | 800 ms: alternates ✓ and clockwise return-flip arrow |
| Tower ESP32 | `ESPNOW_CHANNEL` | 6 |
| R4 | `BRIGHTNESS` | **20**: recently observed to be temporarily stable on site |
| R4 | `MAX_UNITS` | 10 |
| R4 | `SIM_INTERVAL_MS` | 10000 ms |
| R4 | `FULL_HOLD_MS` | 10000 ms |
| R4 | `FADE_MS` | **5000 ms**, total for all three green/blue/red phases |

## 5. Why old firmware should not be mixed in

All five files in this package use **DemoCycle v3** for the round-aware Token protocol, including Token ID, activity, round ID, boot ID, message number, and status acknowledgements. Older `FinalACK`, `ReturnLink_Test`, and `DropDone` sketches use different message formats. **Do not run the old Tower ESP32 or Token software together with these v3 sketches.** The R4 and Tower ESP32 also use a coordinated UART format.

## 6. Upload and startup procedure

1. Make a backup copy of the whole package before uploading; do not overwrite the earlier sketches that were used during previous physical tests.
2. Review external power sources and USB connections, especially around the Tower. Ensure that separate supply outputs are not accidentally connected directly together.
3. In Arduino IDE, upload each sketch separately, in the order **R4 → Tower ESP32 → Exercise Token → Study Token → Gaming Token**. Verify that the USB port selected belongs to the intended physical board.
4. After uploading, and only if existing demonstration data may be cleared, restart/power-cycle **R4 → Tower ESP32 → three Tokens**. Allow the Tower and R4 to synchronise.
5. In the Tower ESP32 Serial Monitor, expect an active-round message such as `ROUND ACTIVE: R4 synchronized.`. Each Token should report that the round is ready (for example `ROUND READY`) before you attempt to start timing by flipping it.
6. Test one Token at a time, verifying eight-second progress, final ACK, and the alternating ✓ / return arrow. Then test the second and third Token.
7. While the left side contains only a few real units, observe the right-side scripted player starting Drops every 10 seconds. Finally, bring the left side to 10 units and check the 10-second hold → five-second green/blue/red breathing sequence → coordinated restart.

A restart erases the demonstration's RAM-based activity data, counters, and any unfinished timer session. Save recordings and logs first if they are needed as evaluation evidence.

## 7. Issues still requiring verification

- The Tower previously displayed random colours over the full panel at brightness 70, but the fault has not reappeared in the reported test at brightness 20. Electrical supply and signal integrity have not been conclusively diagnosed. **Verify safe power delivery before extended demonstration testing.**
- The Exercise Token has occasionally gone black near rows 6–8 of the full-screen progress effect. It worked during a standalone power-bank USB-C test, but should be retested while several devices run simultaneously.
- Long-term natural multi-user use, real one-hour timing reliability, and privacy/psychological effects of shared activity display have not been demonstrated.
- The automatic restart is a **software reset**. Devices that lose power or communications cannot be guaranteed to follow the same reset plan.

## 8. What the current data actually mean

A success checkmark (✓) means **the matched R4 processing result reported that its in-RAM activity record was updated and a final LED refresh was issued**. This does not prove that the LEDs were optically verified or that data were written to permanent storage. If no final acknowledgement arrives, `!` means the result is **unknown**, not definitively lost. The simulated right side cannot be counted as actual data from a second participant, and a completed Time Unit does not prove that the person actually performed the selected activity.
