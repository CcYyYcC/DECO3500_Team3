# DECO3500 — Time Tokens & Time Tower: Complete Design and Prototype Development Process

> **Document version: 25 October 2026** | English development record / Markdown  
> **Project type:** Social & Mobile Computing — tangible interaction prototype  
> **Scope:** This document separates early concepts, team research, features actually implemented in firmware, findings from user evaluations, and assumptions still requiring verification. “Final version” means the **latest integrated demonstration-firmware baseline** in this package, not a product that has passed long-term field evaluation, production reliability testing, or a complete resolution of all hardware faults.

## 0. Project overview: What problem are we addressing?

Time Tokens began with the course-assigned domain **Combatting Computer Game Addiction**. As the research developed, however, we realised that **spending a long time gaming does not automatically mean addiction, nor does it mean that the time was wasted**. Two people may each play for two hours: one might deliberately choose to relax, while another might be avoiding an important responsibility. We therefore shifted toward **time management and task prioritisation**: helping users actively choose an activity, see how they allocate time, and reconsider their choice at regular moments.

The final system has two layers of tangible interaction:

- **Personal Time Tokens:** Three separate devices represent Study, Exercise, and Gaming. A user selects a Token, flips it, observes the LED timer, automatically sends one completed time unit to the Time Tower, receives a confirmation of actual processing, and returns the Token to its initial orientation before beginning another cycle.
- **Shared Time Tower:** The left side collects activity records from the three real Tokens. The right side displays a **clearly identified simulated player** to illustrate another person's activity pattern. Each record appears as an activity-specific colour and 3×3 shape that drops from the top and joins a stack.

The intended unit is **1 hour = 1 Time Unit**, but the **current demonstration firmware simulates one hour in eight seconds**. A completed timer records the completion of an interval; it does not independently verify whether the person actually studied, exercised, or stopped gaming.

### Current physical interaction flow

```text
User chooses a Study / Exercise / Gaming Token
                   |
                   v
             Flips it over
                   |  Three-sample Z-axis smoothing + 250 ms stable hold
                   v
         Eight-second demo timer
        MAX7219 adds one full row per second
                   |
                   v
          All 64 LEDs light up
                   |
                   v
     Token sends the record over ESP-NOW
                   |  Channel 6
                   v
        Tower ESP32 --> UART --> UNO R4
                   |
                   v
      Tower's left column plays a 3x3 Drop
                   |
                   v
      R4 returns a genuine DROP_DONE result
                   |
                   v
      Tower ESP32 relays the final acknowledgement
                   |
                   v
     Token alternates an inverted checkmark
          and clockwise return arrow
                   |
                   v
      User returns Token upright; READY again
```

---

## 1. Stage One: Understanding the original domain and research question

**Starting point:** Combatting Computer Game Addiction. We first explored how gaming rewards, reinforcement, daily quests, social obligations, and sunk cost might encourage continued play. An early interview draft placed particular emphasis on MOBA players.

**Problem identified:** These mechanisms are not unique to MOBA games, and longer gaming time does not necessarily mean a loss of control. Treating “playing a lot” as the problem can pathologise ordinary leisure.

**What we did:** The team revised the semi-structured interview guide to be **independent of game genre**. It asked why people start or stop gaming, how gaming interacts with study and other responsibilities, what motivates different players, and what kinds of support would be acceptable. We also considered contrasting accounts from people who played rarely or not at all. The Project Development Notebook records **14 in-depth interviews conducted by five team members**. This is the **team-wide research total**, not 14 interviews independently conducted by one member.

**Stage outcome:** We reframed the project from “reducing gaming hours” toward **autonomy**, **priority conflict**, and **reconsideration**. We wanted the product to support people's understanding of their choices rather than punish gaming.

**Main evidence:** `DECO3500_Project_Development_Notebook.pdf`, pp. 3–5 (initial domain, interview revision, and sample information).

## 2. Stage Two: Interview organisation, story synthesis, and the design opportunity

After transcription, the team grouped interview material into **seven initial story clusters**: difficulty leaving social games; competition and immediate feedback; immersion and the feeling of an unfinished experience; collecting and self-set goals; live-service and daily-task obligations; gaming as an escape from stress; and the finding that gaming duration alone does not establish addiction.

We then developed **Story 1** and **Story 5** in greater depth:

| Research story | Key finding | Design implication |
|---|---|---|
| Story 1 | Reducing playtime through external restrictions does not necessarily change a person's desire or relationship with gaming. | Do not make forced lock-outs or punishment the core interaction. |
| Story 5 | The same amount of playtime may mean very different things in different life situations. | Do not use one universal threshold to decide whether a user is “addicted.” |

**Combined opportunity:** Provide an understandable time cue while the user can still make a choice, helping them compare allocations among **gaming, study, work, and rest** rather than allowing the device to decide which activities are worth more.

The resulting design values were **autonomy, visibility, personal priorities, the ability to reconsider, and non-judgement**.

**Main evidence:** Project Development Notebook, pp. 6–8; team coding and refined Story 1/5 synthesis records.

## 3. Stage Three: Moving from complex metaphors to simple time units

The team explored several concepts after clarifying the problem. We did not move directly to the Token design.

| Concept explored | Useful idea retained | Why it was not selected as the final form |
|---|---|---|
| Sea Monster / Deadline Shark | Represent pressure and progress through visible physical movement. | Progress in different real-world tasks is difficult to measure objectively; a threat metaphor could increase anxiety. |
| Revolver Commitment | Let users choose how many discrete units of effort they want to commit. | The weapon metaphor did not fit the intended experience and focused too heavily on completing work. |
| Conquest Wheel | Turn planning and time checkpoints into playful tangible interaction. | Mechanical parts, characters, and rules became too complicated. |
| Time as a Physical Landscape | Use different activity blocks to show planned versus actual allocation. | Clear and direct, but physically large with too many loose components. |
| **Time Tokens + Time Tower** | Keep activity categories, discrete units, tangible actions, and shared visibility. | A smaller and more focused system was more practical to build and evaluate. |

**Key decision:** Instead of measuring “how much work a person has completed,” record the **time interval the user chose and completed**. Study, Exercise, and Gaming are parallel activity categories, not good/bad scores.

**Evidence:** Project Development Notebook, pp. 9–14, including concept illustrations and comparisons.

## 4. Stage Four: Choosing the final physical form and interaction

Our form exploration moved through capsule/twist mechanisms, a rolling cylinder, and a minimal cylinder with three individual LEDs and a ball tilt switch. The eventual direction was a **rounded cuboid Token with two stable opposing faces**.

**Reasons for the changes:**

- A cylinder could roll and did not communicate a clear normal orientation → use two stable resting faces.
- Three individual LEDs could communicate only a few states → use an 8×8 matrix for hourglasses, arrows, progress, and acknowledgements.
- A ball tilt switch could respond unpredictably to brief movement → use a three-axis accelerometer and a stable-orientation confirmation window.
- A twist mechanism increased mechanical complexity → remove moving enclosure parts and make **flipping** the central action.

**Interaction principle:** Flipping is not merely an on/off switch; it is a deliberate commitment to a period of time. Flipping back after completion signals that the user must make a new choice before the next interval.

**Scope decisions over time:** Early specifications discussed button confirmation, preserving `pendingUnits` across power loss, pause/cancel states, and other possibilities. **These are not all implemented in the current final firmware.** The demonstration firmware uses a fixed interval, flip-to-start, automatic sending, a final acknowledgement, and a return flip to prepare for the next cycle. Counters are primarily stored in RAM.

**Evidence:** Project Development Notebook, pp. 15–21; `TIME_TOKEN_CONCEPT_SPEC.md` (an earlier proposal, not proof of implemented features).

## 5. Stage Five: Selecting hardware and testing components separately

We did not assemble the complete system in one step. We first verified the **microcontroller, orientation sensor, LED matrix, and communication links** separately and then integrated them.

### 5.1 Personal Token hardware

| Component | Current purpose and connection | Reason for iteration |
|---|---|---|
| Waveshare ESP32-S3-Zero | Microcontroller in each Token. | Moved from an early Arduino UNO R4 Minima direction to a smaller ESP32 with wireless communication. |
| MMA8452Q / XC3732 | I²C: SDA = GPIO1, SCL = GPIO2. | Reads the Z-axis orientation; more suitable for stable sensing than a simple ball tilt switch. |
| MAX7219 8×8 LED matrix | DIN = GPIO4, CS = GPIO5, CLK = GPIO6. | Replaced the earlier HT16K33/I²C-matrix direction; displays several states as pixel patterns. |
| Power | In the current setup, power is supplied to the ESP32 over USB-C and MAX7219 VCC is connected to ESP32 5V. | Reliability with all 64 LEDs lit still needs checking; identical stability has not been demonstrated for all three units. |
| Vibration module | GPIO45 reserved. | **This package does not treat vibration feedback as a field-verified feature; the current interaction mainly relies on LEDs.** |

MAX7219 software intensity is set to `BRIGHTNESS = 0` (its lowest intensity level). This does not mean that the matrix draws no electrical power.

### 5.2 Shared Time Tower hardware

| Component | Current purpose and connection |
|---|---|
| UNO R4 Minima | Controls the 8×32 WS2812B matrix (256 LEDs) with data on D6. |
| Tower ESP32-S3-Zero | Receives ESP-NOW packets and bridges to serial communication. |
| XC4486 bidirectional level converter | Tower ESP32 GPIO7 → R4 D0/RX; R4 D1/TX → ESP32 GPIO8. |
| UART between the two boards | 9600 baud, with shared ground. |
| LED area mapping | x = 0–2 for the real user; x = 3–4 as a separator; x = 5–7 for the simulated user. |

**Important current hardware condition:** The recent on-site description was that the R4 receives power through its round DC jack and supplies the large LED matrix through its 5V path. At brightness 70 the full panel displayed random colours; after changing brightness to 20 the problem was no longer observed. The fact that it seems stable at 20 **does not certify the safety of the power arrangement**. A more appropriate improvement would be an adequately rated independent 5V power supply for the 256 WS2812B LEDs, with common ground and only the D6 data signal coming from the R4. The actual supply and wiring must be checked before modification.

## 6. Stage Six: Building a minimal timing cycle on one Token

The first implementation established a basic Token state machine: **READY → flip confirmation → RUNNING → COMPLETED → wait for return to upright**.

1. **Orientation confirmation:** Read Z-axis acceleration and apply a three-sample moving average. Distinguish `NORMAL > +0.60` from `FLIPPED < -0.30`, and accept the orientation only after it remains stable for approximately 250 ms.
2. **Starting cue:** Initially the action was not clear enough. We later added a continuously repeating clockwise arrow to indicate when the user should flip.
3. **Timing:** The current demo uses `DURATION = 8000`, with one simulated Time Unit generated per completed cycle.
4. **Completion:** A short LED completion cue appears. The device then moves into wireless transfer and result confirmation rather than treating the completion animation itself as successful uploading.
5. **Restarting:** The user must return the Token upright, hold it stable, and make another active flip before starting a new interval.

### 6.1 Improving orientation recognition

Earlier trials revealed a low flip-detection success rate, unstable readings caused by movement, and a retry problem in which users had to return to the normal orientation after a failed attempt. We subsequently introduced smoothed Z-axis readings, a stable hold window, and **the ability to keep waiting for another valid flip after a failed attempt**, rather than restarting the whole interaction because of one brief movement.

This was a usability concern, not merely an algorithmic improvement: if orientation recognition fails, users need to understand **what they can do next**.

### 6.2 Changing LED progress from “one row of dots” to “64 LEDs filling by rows”

**Initial version:** During the eight-second timer, one row gradually accumulated 1–8 dots. The display communicated only a limited sense of progression.

**Updated version:** Every second adds an entire row of eight LEDs. At the eighth second all 64 LEDs are lit. Because the Token has already been physically inverted during timing, we then **rotated the progress display by 180°**, so the user sees the panel fill from bottom to top in the inverted viewing orientation.

```cpp
// Core idea of the current eight-second progress display.
// See the complete showProgress() function in each Token .ino file.
clearMatrix();
for (uint8_t i = 0; i < stage; ++i) {
  maxWrite(1 + i, 0xFF);  // Add one full row, using the inverted register order.
}
```

**Important distinction retained:** The full-screen timing progress, brief completion blink, three-dot transfer wait, and final checkmark/return arrow are **different display states**. Rotating the timer pattern must not globally rotate every other icon or alter their activation conditions.

## 7. Stage Seven: Time Tower shapes, falling animations, and accumulation

We began by testing the Tower LED wiring and address mapping. The matrix is **8 columns × 32 rows** with a serpentine row order, and pixel 0 at the bottom left: even-numbered rows run left to right, and odd-numbered rows right to left. We then introduced 3×3 activity symbols:

```text
Study (blue)       Exercise (green)    Gaming (pink/red)
XXX                XXX                 .X.
X.X                .X.                 XXX
XXX                XXX                 .X.
```

Each Time Unit uses three rows, allowing up to 10 units per lane (30 of the available 32 rows). We first implemented the animation of a block **falling from the top to the current stack height**, rather than simply appearing instantly when a message arrived. `DROP_DELAY = 70 ms` controls the step interval.

We then stored each activity type in the current-round arrays, redrew previously landed shapes, and placed each new unit on top of the existing stack.

## 8. Stage Eight: From “wirelessly received” to “Drop actually completed”

This was one of the most important sequences of iterations for communication reliability. **Successful ESP-NOW delivery or receipt does not mean the large LED panel has already completed the display update.** We therefore extended the acknowledgement chain step by step.

| Iteration | What worked | Why it was insufficient / next improvement |
|---|---|---|
| A. One-way data | Token sent information including `tokenID`, `activity`, `newUnits`, and `messageNumber`; the Tower received it and forwarded it to the R4. | The Token had no evidence that the data was received. |
| B. Receiver-only ACK | Tower ESP32 sent an initial acknowledgement after receiving a valid wireless packet. | This proved only receipt by the Tower bridge, not that the R4 executed a Drop. |
| C. Return UART link test | Connected R4 D1/TX → ESP32 GPIO8; the R4 periodically sent `R4_LINK_TEST`. | This proved that the return wire worked, but did not correlate with a particular Time Unit. |
| D. Genuine R4 completion acknowledgement | Included the message number in UART requests. After an actual Drop, RAM update, and final LED refresh request, the R4 replied with `DROP_DONE`; a full lane returned `DROP_FAIL`. | The Tower now knew the result, but the personal Token still did not. |
| E. End-to-end Final ACK | Tower ESP32 relayed the matched result over ESP-NOW to the Token, which checked message identity and displayed ✓ / X / !. | Success feedback now had a basis in the receiving device's processing, rather than just a timed animation. |
| F. DemoCycle v3 | Added `roundID`, `bootID`, and `messageNumber` to coordinate software resets and duplicate detection across rounds. | Prevents old messages or reused sequence numbers from being mistaken for current-round records after a reboot. |

**Current communication path:**

```text
Token ESP32 --ESP-NOW Ch6--> Tower ESP32 --UART GPIO7--> R4 D0/RX
Token ESP32 <--final ACK---- Tower ESP32 <--UART GPIO8-- R4 D1/TX
```

- Protocol v3 uses a fixed **32-byte wireless message** containing message type, activity, Token ID, round ID, boot ID, message number, unit count, and related state information. **It cannot be mixed with the earlier 10-byte data packet and 16-byte acknowledgement.**
- The R4 returns `DROP_DONE` only after processing the batch. Neither `DROP_FAIL` nor a timeout may be treated as successful completion.
- The bridge uses queues and matching checks to reduce the risk of confusing concurrent requests. A ✓ on the Token means only that **R4 software reported a RAM update and submitted the final LED refresh**. It is not an optical measurement proving that every LED lit correctly or evidence of permanent storage.
- A timeout symbol `!` means **outcome unknown**. The Tower may already have shown the activity while its acknowledgement was lost. Therefore, the system does not automatically count another unit after a timeout.

**Observed evidence from physical tests:** A serial screenshot showed `[R4 -> ESP32] DROP_DONE,1,2,1,1` followed by `=== R4 DROP CONFIRMED ===`. This demonstrates that at least that UART acknowledgement exchange was executed and matched on the physical system.

## 9. Stage Nine: Iterations driven by usability and social evaluation

The prototype underwent small-scale, prompted user tests and scenarios involving a simulated housemate B. The **P01/P02 analysis is formative evidence**, not long-term evidence from natural household use. Some dialogue from staged scenarios has uncertain speaker attribution and must not be presented as a verified quotation from a named participant.

| Observation or evaluation finding | Design implication | Response or current status |
|---|---|---|
| Users were unsure when to flip a Token. | An action should not depend entirely on verbal instructions. | Added a continuous clockwise arrow and flip-confirmation feedback (**implemented**). |
| Failed flips were unclear and recognition was unreliable. | Users could not tell whether the timer had started or whether to retry. | Revised Z-axis filtering, stable-orientation checks, and retry behaviour (**implemented**). |
| After timing, users could not tell whether data reached the Tower. | “The activity ended” and “the record transferred successfully” were being confused. | Built a layered Receiver ACK → R4 Drop ACK → final Token ✓ chain (**implemented**). |
| Users were unsure how to begin the next cycle after completion. | The state transition lacked a visible cue. | Alternating ✓ and clockwise return arrow, shown correctly for inverted viewing (**implemented**). |
| Tower activity symbols and lane ownership were not immediately understood by everyone. | Shared displays need an activity key, owner identification, and an explanation of the unit. | Colour/shape coding retained; legend and ownership cues still need testing (**not fully resolved**). |
| Shared records might lead to comparison, judgement, or dishonest recording of gaming time. | Social visibility may support conversation while also creating psychological pressure. | No leaderboard added; anonymity and optional disclosure remain design considerations, **not solved privacy risks**. |
| Manually flipping every hour could interrupt study. | There is a trade-off between a reflection checkpoint and an uninterrupted workflow. | The team **chose to retain one active interaction per hour**. This is a design trade-off, not evidence that the checkpoint is beneficial. |
| Exercise Token mobility outside the home was uncertain. | Power and cables may limit mobile use. | Explored standalone power-bank operation; sustained use outdoors is still unverified. |
| Overly bright LEDs affected viewing comfort. | Visual comfort is part of the shared-space experience. | Reduced brightness and continued technical observation; do not describe a fault mitigation as a complete user-experience evaluation. |

In P01's reported experience, some personal activity units could be identified after prompting, but not every symbol was self-explanatory. The participant also indicated that not revealing exact activity start times could reduce exposure of private information. Some statements about **comparison pressure** and **record ownership** came from staged discussion and carry weaker evidential weight than observed behaviour in natural settings.

**Evidence:** `DECO3500_Evaluation_Analysis_P01_P02.md` (themes T1–T6 and numbered P01/P02 segments), plus physical Token test logs and photos.

## 10. Stage Ten: Adding a simulated player for the social demonstration

In the eventual configuration, all three real activity Tokens contribute to the **Tower's left side** (one real user), while the right side displays a **script-generated simulated housemate**. This makes side-by-side visibility and different activity patterns available for classroom discussion. **It must not be described as two real people both using Tokens.**

The right-side sequence is fixed:

```text
Study → Study → Gaming → Gaming → Gaming → Exercise
      → Study → Study → Study → Gaming
```

A Drop **starts** every 10 seconds, at 10, 20, ... 100 seconds. After reaching 10 units, the simulated side stops; it **does not initiate a system-wide reset**. The left and right falling animations were reworked as separate, non-blocking frame updates so real Token messages and the scripted activity sequence can appear within one demonstration.

This supports classroom observation and discussion, but real shared-household behaviour, mutual interpretations of activity traces, and willingness to disclose records still require direct evaluation.

## 11. Stage Eleven: Full-lane closing animation and automatic restart

**Goal:** Create an understandable ending and an automatic fresh start for successive classroom visitors without manually pressing Reset between demonstrations.

The final sequence is:

```text
The 10th REAL Time Unit lands in the LEFT lane
                    |
                    v
     Send final success acknowledgement first
                    |
                    v
      Hold both current displays for 10 seconds
                    |
                    v
       Whole-panel breathing: green → blue → red
                  5 seconds total
                    |
                    v
             All 256 LEDs turn off
                    |
                    v
      Approximately 2 seconds of coordinated
             next-round reset messaging
                    |
                    v
    R4, Tower ESP32, and online Tokens restart
                    |
                    v
        RAM cleared; next demonstration begins
```

**Animation iteration:** We initially tried gradually fading the existing activity shapes together. After abnormal display behaviour was observed, we replaced that approach with a **uniform, full-panel breathing sequence in green, blue, and red, with successively reduced intensity**. During each frame all 256 pixels share the same colour and intensity. The panel finally becomes black before the R4 sends `FADE_DONE`, which the Tower ESP32 uses to coordinate the reset.

**Engineering the restart:** DemoCycle v3 uses round and boot identifiers to distinguish old packets from new ones. Tower ESP32 stores a small amount of round metadata through `Preferences`. The ordinary activity totals on the Tokens and R4 are still mainly RAM-based; a software restart clears the current demonstration's activity data. Online devices process the reset plan and execute software resets. Devices that have lost power or wireless contact cannot be guaranteed to restart at exactly the same time.

**Important:** If the left lane fills before the right-side simulated player has completed all 10 entries, the left-lane closing process takes priority. The system does not wait for the simulation to finish.

## 12. Stage Twelve: Latest physical issues and diagnosis (as of 25 October 2026)

### 12.1 Random colours across the entire Time Tower

- **Observed symptom:** After accumulating three or four units, the entire 8×32 matrix—including the two divider columns that should have remained black—displayed random colours.
- **Comparative test:** After R4 global brightness was reduced from `70` to `20`, the user reported that the fault no longer appeared.
- **Reasonable interpretation:** Display load, 5V supply voltage drop, common-ground quality, or limited WS2812B data-signal margin may be contributing factors. Reducing brightness reduces load, making this a strong clue, **but it does not establish one measured root cause**.
- **Firmware decision:** The packaged R4 sketch keeps the five-second green/blue/red breathing sequence with `BRIGHTNESS = 20`.
- **Unresolved hardware work:** The reported electrical arrangement powers the matrix via the R4's round DC power input and its 5V output path. This should not be assumed to be a reliable supply arrangement for a large LED array. An independent, suitably rated 5V supply, appropriate power-injection points, and D6 signal integrity should be evaluated. **This rewiring is not claimed as completed in the package.**

### 12.2 Exercise Token blacking out near the end of full-screen progress

- **Observed symptom:** The Exercise Token sometimes went black as the sixth to eighth rows of its progress matrix lit up, although later transfer feedback still worked. The Gaming Token did not show the same problem.
- **Comparative test:** The user later reported that the Exercise Token **did not black out when used alone with a 5V power bank connected to the ESP32 through USB-C**.
- **Current interpretation:** It is not an error that necessarily occurs whenever a particular row is written. Stability with several devices operating together, or under different power conditions, is still unproven. The root cause has not been isolated among the supply, cabling, MAX7219 module, and 3.3V-to-5V logic-signal margin.
- **Firmware decision:** Keep the intended **full 64-LED progress effect** rather than reducing the number of lit dots based on an unconfirmed diagnosis. MAX7219 stays at its lowest intensity setting, `0`. Further comparisons of power sources and display modules are required.

**General rule:** “The display worked after lowering brightness” does not mean “the hardware is fully fixed.” Likewise, “the Exercise Token worked once on a power bank” does not mean “it passed testing under all operating conditions.”

## 13. Final firmware structure and parameter reference

| Device | Firmware directory (inside the ZIP) | Main configuration |
|---|---|---|
| Exercise Token | `Firmware/Token_Exercise_Final180/` | `TOKEN_ID=1`, `EXERCISE=2` |
| Study Token | `Firmware/Token_Study_Final180/` | `TOKEN_ID=2`, `STUDY=1` |
| Gaming Token | `Firmware/Token_Gaming_Final180/` | `TOKEN_ID=3`, `GAMING=3` |
| Tower ESP32 | `Firmware/Tower_Bridge_DemoCycle_v3/` | ESP-NOW channel 6; UART GPIO7/8; 9600 baud |
| UNO R4 | `Firmware/R4_Tower_Breath5s_B20/` | WS2812B on D6; `BRIGHTNESS=20`; 5-second breathing sequence |

| Parameter | Current setting | Purpose |
|---|---:|---|
| `DURATION` | 8000 ms | Demonstration: eight seconds represents one hour. |
| `CONFIRM_MS` | 250 ms | Stable-orientation confirmation window. |
| `RETURN_SWAP_MS` | 800 ms | Alternation between the success checkmark and return-flip arrow. |
| `FINAL_ACK_TIMEOUT_MS` | 35000 ms | If the final acknowledgement does not arrive, show an “unknown” state. |
| `SIM_INTERVAL_MS` | 10000 ms | Start one simulated Drop on the right every 10 seconds. |
| `DROP_DELAY` | 70 ms | Interval between falling animation steps. |
| `MAX_UNITS` | 10 | Maximum of 10 units per lane. |
| `FULL_HOLD_MS` | 10000 ms | Keep the full left-lane scene visible for 10 seconds. |
| `FADE_MS` | 5000 ms | Total duration of the green/blue/red breathing sequence. |
| `BRIGHTNESS` (R4) | 20 | Lower brightness observed to be temporarily stable during physical tests. |
| `BRIGHTNESS` (Token) | 0 | Lowest MAX7219 intensity setting. |

For implementation details in the complete sketches, see functions including `showProgress()`, `readOrientationSample()`, `updateSession()`, `pollTransferAck()`, `startNext()`, `handleR4()`, `drawTimeUnit()`, `stepFalls()`, and `updateRound()`. Together these cover LED patterns, orientation, timing, acknowledgements, UART routing, falling animations, and closing/reset transitions.

## 14. Verification summary: What is supported, and what is not?

| Item | Current evidence and status |
|---|---|
| ESP-NOW data received by Tower from a Token | Observed in earlier physical serial-monitor tests. |
| R4 completes Drop → UART reply → Tower matches result | Earlier screenshot shows matching `DROP_DONE` and `R4 DROP CONFIRMED`. |
| Tower final acknowledgement → Token success checkmark | The user previously reported the integrated version behaving normally; complete retesting of the new round-coordinated version remains necessary. |
| Three Tokens assigned separate IDs and activities | Configured in the source; three-device concurrent reliability has not been quantified. |
| Real left-side records and fixed simulated right-side sequence | Implemented in the latest demonstration firmware; the script does not represent a second real user. |
| Hold / breathing / software restart after 10 left-side units | Implemented in the latest firmware; full physical round-to-round coordination and power conditions still need retesting. |
| Time Tower at brightness 20 | User reported that the previous random-colour fault no longer appeared; long-term electrical reliability is unknown. |
| Exercise Token's 64-LED progress | Worked in a standalone power-bank test; occasional blackouts in other contexts remain unexplained. |
| Real one-hour use or long-term habit change | **Not verified**; eight seconds is an accelerated demonstration. |
| Natural interaction between two real users or reduction of addiction / improvement in efficiency | **No causal evidence available**. These cannot be presented as demonstrated project outcomes. |

### Evidence worth keeping for further testing

1. **Interaction video:** Upright → flip → eight rows filling → full acknowledgement → checkmark/return arrow → READY again.
2. **Communication logs:** Token's ID, activity, round, and message number together with matching R4 completion and the Tower's final ACK.
3. **Two-column demonstration video:** Three real Token categories on the left; fixed simulated sequence and 10-second intervals on the right, explicitly labelled as simulation.
4. **End-of-round video:** Tenth left-side Drop → 10-second hold → five-second green/blue/red sequence → black screen → a new round begins with empty lanes.
5. **Power and stability notes:** Number of brightness-20 R4 tests, actual power path, comparison between standalone and simultaneous Exercise Token operation, and all failures as well as successes.
6. **Evaluation records:** P01/P02 transcript evidence and themes, clearly identifying statements whose speaker is uncertain. Any later testing with two real users should be recorded as new evidence.

## 15. Personal involvement and reflection (for the portfolio)

Based on the available project conversations and team responsibility records, the student's involvement spans **tangible prototyping, Token–Tower integration, communication-protocol iteration, test planning, and evaluation analysis**. Activities discussed include hardware wiring and component tests, LED and orientation feedback changes, incremental UART/wireless acknowledgement checks, translation and cleaning of evaluation materials, qualitative coding, and theme synthesis. A personal contribution statement should use the student's own GitHub commits, photographs, recordings, and meeting notes to distinguish individual implementation, work completed collaboratively, and team decisions.

A simple reflection that can be adapted using personal evidence is:

> Initially, I focused on getting the device to count time and show progress. Physical testing made me realise that knowing how to start, understanding whether an action worked, and trusting that the record really reached the shared Tower mattered more than an attractive lighting animation alone. I therefore kept improving the system from something that could demonstrate a function into something that could provide clearer, more trustworthy interaction feedback. At the same time, making personal activity records visible to others could create comparison pressure, while requiring another flip every hour could interrupt studying. These tensions reminded me that completing a technical feature is not the same as demonstrating that the design goal has been achieved.

This is **draft reflection material** to refine using the student's actual contribution evidence; it must not be treated as an additional user research finding.

---

## 16. Sources and cautions

This process account is based on the existing project records and corresponding `.ino` firmware versions. The main traceable sources are:

- `DECO3500_Project_Development_Notebook.pdf`: Domain research, 14 team interviews, seven story clusters, refined Stories 1/5, concept comparison, and physical-form iterations (especially pp. 2–21).
- `DECO3500_Evaluation_Analysis_P01_P02.md`: Formative evaluation, T1–T6, and numbered qualitative evidence. It distinguishes staged role-play from genuine use by two people.
- `TIME_TOKEN_CONCEPT_SPEC.md`, `TIME_TOKEN_FUNCTIONAL_SPEC_T2`, `TIME_TOKEN_HARDWARE_PLAN_T3.md`: Earlier **concept, functional, and hardware plans**; they do not supersede the actual firmware.
- Firmware development sequence: `TimeToken_FlipFix` → `ReceiverACK` → `ReturnLink_Test` → `DropDone` → `FinalACK` → `DemoCycle v3` → `FullScreen Progress 180°` → `Tower Breath5s / Brightness20`.
- Serial screenshots and physical feedback in the project discussions, particularly `R4 DROP CONFIRMED`, full-panel random colours, improvement after reducing brightness, and standalone Exercise Token power-bank testing.

**The most important research boundary:** We have developed a prototype for deliberate time recording and sharing that raises useful questions about social interaction. We have **not** established that it reduces gaming addiction, improves productivity, or changes long-term behaviour.
