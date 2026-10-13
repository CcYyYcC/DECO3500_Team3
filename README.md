# Time Tokens & Time Tower

A tangible time-awareness system for shared living spaces — DECO3500 (Assignment 3: Design Project), Team 3 ("GG").

Residents flip a physical cube to start a timed block (Work/Study · Entertainment · Rest) and dock it into a shared "Time Tower", which displays the day as accumulating coloured light. The Tower is visible to everyone in the share house, turning individual time use into shared, ambient awareness.

**Design opportunity:** the problem is not that people game too much, but that gaming sometimes takes priority over real-life commitments at the wrong moment. We help people make that comparison themselves, before they start — using peer awareness rather than restriction.

---

## Where everything lives

| Deliverable | Location |
|---|---|
| **Prototype** | [`/prototype/`](prototype/README.md) — in this repo, see below |
| **Ongoing documentation** | [`/research/`](research/README.md) · [`/design/`](design/README.md) · [`/process/`](process/README.md) |
| **Design Process Overview** | [Wiki page](../../wiki/Design-Process-Overview) |
| **Ethical Considerations** | [Wiki page](../../wiki/Ethical-Considerations) |
| **Stand-up records** | [Wiki page](../../wiki/Team-Meetings) · [Week 9 Stand-up](../../wiki/Week-9-Stand-up) |
| **Tasks & issues** | [GitHub Projects](../../projects) — tracked as issues, not as files |

The full [Wiki](../../wiki) also holds User Research, Requirements, Design Iterations, and User Evaluation.

---

## Prototype: Deploy & Usage

The prototype is **physical hardware** — two 3D-printed devices with embedded electronics. There is no web app, no server, and **no login or account of any kind**. "Deployment" means assembling the devices and flashing the controllers.

### What you need

**Time Token** (personal cube, ~60 × 60 × 60 mm)

| Component | Purpose |
|---|---|
| Waveshare ESP32-S3-Zero | Controller, sensor reading, LED control |
| MMA8452Q accelerometer | Detects which face is up → starts a time unit |
| MAX7219 8 × 8 LED module | Timing progress and completion animation |
| 5 V three-pin vibration module | Haptic feedback on completion *(not yet working — see limitations)* |
| Mini breadboard | Prototype wiring |

**Time Tower** (shared display, ~400 mm tall)

| Component | Purpose |
|---|---|
| WS2812B 8 × 32 RGB LED panel | Vertical record of accumulated time |
| Arduino UNO R4 Minima | Drives the Tower display |
| ESP32-S3-Zero | Planned receiver for Token data |
| CUKTECH 15 PB200 power bank | Portable power for the Tower |

Printed parts, CAD/STL files, and assembly photographs: [`/prototype/figures/`](prototype/figures/).

### Running it

1. **Power** — the Token runs on external USB-C (no internal battery in this version). The Tower runs from the PB200 power bank; high LED current must not pass through the UNO R4 Minima.
2. **Flash the Token** — upload the firmware to the ESP32-S3-Zero.
3. **Flash the Tower** — upload the display logic to the UNO R4 Minima.
4. **Use it** — place the Token in the assigned orientation for the activity you're starting. The accelerometer detects the position and begins a time unit; the LED matrix animates progress. *The current firmware uses an eight-second unit for demonstration — the real duration is a design setting, not a fixed value.* On completion, the LED plays a completion animation.
5. **Dock it** — the Token fits the 62 × 62 mm recess in the Tower's top cap.

> <!-- TODO: 以下由负责硬件的队友补齐 -->
> **Still to document here:** exact firmware upload steps (IDE/toolchain and board settings), library dependencies, pin assignments, and where the firmware source lives in this repo. Full hardware build record: [`/prototype/README.md`](prototype/README.md).

### Known limitations

| Function | Status |
|---|---|
| Orientation sensing | Verified separately |
| LED display | Partially verified — power stability still under review |
| Vibration feedback | **Not working** — GPIO switches correctly but the purchased module does not vibrate |
| Token → Tower communication | Intended function — ESP32 space reserved, not yet integrated |
| Tower time record | Structure designed, display logic in testing |
| Token-in-Tower fit | CAD interference check passed; final physical fitting photo pending |

Current development status and next iteration: [`/prototype/development-record.md`](prototype/development-record.md).

---

## Repository map

```text
.
├── README.md            this file — guide, deploy & usage
│
├── prototype/           the prototype itself
│   ├── README.md              build record: how it works, components, iterations
│   ├── development-record.md  iteration 1 development log
│   ├── deployment-guide.md    what deployment docs must cover
│   ├── testing-guide.md       scope of testing for this project
│   └── figures/               21 build figures
│
├── research/            user research & evaluation evidence
│   ├── interviews/            14 transcripts, named <interviewer>_<n>
│   └── user-testing/          prototype test transcripts + summaries
│
├── design/              design artefacts: poster, iteration process, literature, Miro record
│
├── process/             meetings, decisions, risks, AI-use disclosure, ethical disclaimer
│
├── team/                internal planning (weekly plan, project board setup)
└── .github/             issue template used by the Projects board
```

## Team

| Member | Focus |
|---|---|
| Yucheng Chen | Ethical considerations · wiki structure & documentation |
| Yiyang Qin | Hardware prototyping · electronics integration · modelling & 3D printing |
| Bo Wang | Prototype development · interaction implementation · user evaluation |
| Zhaoguo Huang | User research · evaluation planning · insight synthesis |
| Yuming Zhong | User research · interviews |

## Links

- [Wiki](../../wiki) — design process overview, ethical considerations, stand-up records
- [GitHub Projects](../../projects) — task and issue tracking
- [Miro board](https://miro.com/app/board/uXjVHzNW9XU=/) — design sprint and research workspace
