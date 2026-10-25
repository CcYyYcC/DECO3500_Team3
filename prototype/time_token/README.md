# Time Token

The personal device: a 60 mm cube, one activity per Token. Flip it to start a timed unit;
the LED matrix shows progress and plays a completion animation.

| Folder | What belongs here |
|---|---|
| `firmware/` | ESP32-S3-Zero sketch — orientation reading, timing cycle, LED patterns, transfer |
| `hardware/` | Wiring diagrams, pin assignments, bill of materials |
| `cad/` | Enclosure CAD and STL files, all four iterations |
| `photos/` | Build and detail photographs |

## Components

| Component | Purpose |
|---|---|
| Waveshare ESP32-S3-Zero | Controller, sensor reading, LED control, transfer |
| MMA8452Q accelerometer | Detects which face is up, starting a time unit |
| MAX7219 8 × 8 LED module | Timing progress and completion animation |
| 5 V three-pin vibration module | Haptic feedback on completion — **not working**, the module does not respond to the GPIO signal |
| Mini breadboard | Prototype wiring |

Powered over USB-C. There is no battery in this version, so the Token is not yet portable in
the way the concept assumes.

<!-- TODO: commit the firmware, wiring diagram and CAD. These folders are the gap between
     "we built it" and "someone else could build it". -->

Full build narrative: [`../development-record.md`](../development-record.md) ·
iteration figures: [`../figures/`](../figures)

---

[← Prototype](../README.md)
