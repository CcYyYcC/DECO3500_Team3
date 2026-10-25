# Time Tower

The shared device: roughly 400 mm tall, standing in the common area. Submitted units
accumulate as a vertical stack of coloured blocks, one block to an hour, in two columns.

| Folder | What belongs here |
|---|---|
| `firmware/` | Arduino UNO R4 Minima display logic and the ESP32-S3-Zero receiver sketch |
| `hardware/` | Wiring, power distribution, pin assignments, bill of materials |
| `cad/` | Three-part body, LED rails, control-board platforms |
| `photos/` | Build and detail photographs |

## Components

| Component | Purpose |
|---|---|
| WS2812B 8 × 32 RGB LED panel | The vertical record of accumulated time |
| Arduino UNO R4 Minima | Drives the Tower display |
| ESP32-S3-Zero | Receives Token data and passes it to the display logic |
| CUKTECH 15 PB200 power bank | Portable power |

High LED current must not pass through the UNO R4 Minima — the panel is fed from the power
bank directly, with the boards sharing ground.

<!-- TODO: commit the firmware, wiring and CAD. -->

Full build narrative: [`../development-record.md`](../development-record.md)

---

[← Prototype](../README.md)
