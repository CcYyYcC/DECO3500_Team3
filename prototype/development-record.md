# Prototype Development

## 1. Prototype Overview

Our prototype explores how everyday time use can be made more tangible and visible through physical interaction.

The current system consists of two main parts:

1. **Personal Time Tokens** — physical devices used to represent and track different activity types, including Work/Study, Entertainment, and Rest.
2. **Shared Time Tower** — a public physical display intended to receive completed activity time from individual Tokens and visualise accumulated time in a shared form.

The prototype is designed to support reflection rather than restrict particular activities. Instead of telling users whether an activity is "good" or "bad", it makes the distribution of time more visible so that users can notice patterns in how they spend their day.

---

# 2. Overall Interaction Concept

The intended interaction flow is:

**Select an activity**  
↓  
**Activate the corresponding Time Token**  
↓  
**The Token begins tracking time**  
↓  
**LED feedback visualises the passage of time**  
↓  
**A completed time unit is recorded**  
↓  
**Completed time is transferred to the shared Time Tower**  
↓  
**The Tower adds the activity to a collective time representation**

The individual Token therefore focuses on **personal awareness**, while the Time Tower introduces the **social and shared dimension** of the experience.

---

# 3. Development of the Personal Time Token

## 3.1 Initial Concept

The initial prototype concept treated time as a physical object rather than information displayed only on a phone or computer screen.

Different activities were represented through separate physical Tokens. The intention was to make starting and tracking an activity a simple physical action while allowing users to see time gradually accumulate.

The early concept focused on three activity categories:

- Work / Study
- Entertainment
- Rest

Each category represents a different type of everyday time use rather than a performance score.

---

## 3.2 Controller Development

The prototype was initially explored using larger development hardware. During later development, the controller was changed to a **Waveshare ESP32-S3-Zero**.

The smaller controller was selected because it is more suitable for integration into the physical Token and allows the prototype to move from a breadboard-style technical test towards a compact physical artefact.

Current controller:

- Waveshare ESP32-S3-Zero

The ESP32-S3-Zero is currently used as the main controller for both the Time Token prototype and the Time Tower prototype.

---

## 3.3 LED Display Development

An early version of the Token explored an I²C-based 8×8 LED matrix.

During development, this display was replaced with a **MAX7219 8×8 LED matrix**.

The current MAX7219 modules are fixed single-colour matrices. Separate red, green, and blue modules can therefore be used to differentiate activity categories physically.

The LED matrix is used as the main visual feedback mechanism for the Token.

Its intended functions include:

- indicating that a Token has started;
- showing that time is currently being tracked;
- visualising the progress of a time unit;
- showing when a time unit has been completed;
- providing simple transition or completion animations.

The current development has focused on testing reliable LED output and basic animation behaviour before integrating the complete interaction sequence.

---

# 4. Time Tracking Logic

The Time Token is designed around a one-hour time unit.

When a Token is activated, the system begins tracking the selected activity. During the tracking period, the LED matrix provides visual feedback so that the user can see that time is progressing without repeatedly checking a phone or numerical timer.

The intended sequence is:

**Inactive**  
↓  
**Activity started**  
↓  
**Timer running**  
↓  
**LED progress feedback**  
↓  
**One time unit completed**  
↓  
**Completed unit stored**  
↓  
**Ready for transfer**

A variable representing accumulated completed units is used so that completed time can be retained before it is transferred to the Time Tower.

This separates:

- time currently being tracked; and
- time that has already been completed and is ready to contribute to the shared display.

---

# 5. Physical Interaction Development

The prototype has gradually moved away from conventional screen-based interaction.

The aim is for the Token itself to communicate its current state through:

- its physical form;
- activity-specific identity;
- LED behaviour;
- simple physical interaction.

Earlier concepts considered additional controls such as external buttons. The current direction simplifies the Token and reduces unnecessary interface elements so that the interaction can remain focused on the physical object itself.

The prototype is still being refined to determine the clearest interaction for starting, completing, and transferring activity time.

---

# 6. Shared Time Tower

## 6.1 Purpose

The Time Tower is the shared component of the prototype.

While the individual Token records personal activity time, the Time Tower is intended to transform multiple individual contributions into a collective representation.

Its purpose is not to rank users or identify who has performed "better". Instead, it makes different patterns of time use socially visible.

The intended interaction is:

**Individual activity completed**  
↓  
**Time stored on Token**  
↓  
**Time transferred**  
↓  
**Time Tower receives activity information**  
↓  
**Shared display is updated**

This creates a connection between personal time awareness and social reflection.

---

## 6.2 Current Time Tower Development

The Time Tower is currently being developed as a separate physical prototype.

Current development includes:

- ESP32-S3-Zero controller setup;
- LED matrix connection and testing;
- basic LED output testing;
- testing simple display patterns;
- development of the physical structure;
- preparation for receiving data from individual Tokens.

The LED system is currently being tested independently before full integration with the complete Tower interaction.

This modular testing approach allows hardware problems to be identified before the communication and shared visualisation systems are added.

---

# 7. Prototype Architecture

The current prototype can be represented as:

```text
PERSONAL TIME TOKEN
│
├── ESP32-S3-Zero
├── Activity identity
├── Timer logic
├── 8×8 MAX7219 LED matrix
├── LED progress feedback
└── Completed time units
          │
          │ Transfer
          ▼
SHARED TIME TOWER
│
├── ESP32-S3-Zero
├── Data receiving logic
├── LED display system
└── Shared time visualisation
```

This architecture separates the prototype into two relatively independent systems.

This is useful during development because the Token and Tower can first be tested separately before being integrated into the complete experience.

---

# 8. Current Prototype Status

| Prototype Component | Current Status | Notes |
|---|---|---|
| ESP32-S3-Zero setup | Implemented | Main controller selected and configured |
| Personal Token hardware | Prototype implemented | Physical and electronic development ongoing |
| MAX7219 LED matrix | Implemented / testing | Basic LED behaviour tested |
| LED animations | In development | Used for progress and state feedback |
| One-hour timer logic | Implemented / refining | Core timing behaviour developed |
| Completed time storage | In development | Completed units retained before transfer |
| Physical Token form | In development | Being refined for the final interaction |
| Time Tower controller | Implemented | ESP32-S3-Zero used |
| Time Tower LED system | Testing | Independent hardware tests currently being conducted |
| Shared visualisation | Early prototype | Visual behaviour still being refined |
| Token → Tower communication | In development | Full integration not yet completed |
| Multi-user accumulation | Planned / early development | To be integrated with the shared Tower |

---

# 9. Current Technical Development Strategy

At the current stage, development is being carried out in separate modules rather than attempting to build the complete system at once.

The current workflow is:

### Step 1 — Test individual hardware

Each LED matrix and controller is tested independently.

### Step 2 — Implement basic Token behaviour

Timer logic and LED feedback are tested on the individual Token.

### Step 3 — Test the Time Tower display

The Tower LED system is tested separately to ensure stable output.

### Step 4 — Integrate communication

Completed time units from the Token will be transferred to the Tower.

### Step 5 — Integrate shared visualisation

The Tower will convert individual contributions into the final shared time representation.

### Step 6 — Refine through user evaluation

The interaction and visual feedback will be adjusted based on whether users understand the Token states, time progression, transfer interaction, and shared representation.

---

# 10. Current Limitations

The current prototype is a proof-of-concept rather than a finished product.

Several parts of the experience are still under development:

- communication between the Time Token and Time Tower is not yet fully integrated;
- the final shared visualisation is still being refined;
- some LED behaviour is still being tested for reliability;
- the final physical enclosure and interaction details are still evolving;
- multi-user accumulation has not yet been fully implemented.

These limitations are intentionally being addressed through iterative development. At this stage, the priority is to make the core interaction understandable and testable before adding further technical complexity.

---

# 11. Next Prototype Iteration

The next development stage will focus on connecting the currently separate prototype components into one complete interaction flow.

The main priorities are:

1. stabilise the Token LED and timer behaviour;
2. stabilise the Time Tower LED display;
3. complete Token-to-Tower data transfer;
4. define how different activity categories appear on the Tower;
5. test how multiple contributions accumulate;
6. evaluate whether users understand both the personal and shared representations;
7. refine the physical models and feedback based on evaluation findings.

The goal for the next iteration is therefore not simply to add more functionality, but to create a coherent experience from **individual time tracking → completed activity → transfer → shared reflection**.