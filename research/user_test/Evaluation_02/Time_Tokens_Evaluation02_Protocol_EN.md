# Time Tokens & Time Tower
## Round 2 — Revised Formative Evaluation Protocol

**Course:** DECO3500 Social & Mobile Computing  
**Team:** Team 3  
**Source version:** Revised draft, 17 October 2026  
**Edition:** English-only working version, with facilitator scripts and recording sheets  
**Status:** Planned procedures. Complete the equipment and version checks before use.

> **Edition note.** This document consolidates the Chinese and English sections of the supplied `Time_Tokens_Evaluation_Protocol_FIXED.docx`. It retains the original test conditions, questions, task IDs and evidence boundaries. Test B and the real-hour check are proposed additions; their presence in this protocol does not establish that they were completed. The companion [Evaluation Analysis](Time_Tokens_Evaluation02_Analysis_EN.md) reports only what is supported by the supplied P01/P02 transcripts.

| Test | Configuration and purpose |
|---|---|
| **Test A — Simulated shared records** | One real participant P1 and programmed housemate B_sim. Examine display interpretation, awareness of shared records, location and activity transitions, and immediate understanding of the hourly checkpoint. |
| **Test B — Real paired interaction** | Two real participants, P1 and P2, in a shared setting. Examine initiation and response, turn-taking, joint viewing, avoidance and social pressure. |
| **Optional — Genuine one-hour checkpoint** | Normal-duration activity followed by a real hour boundary. Examine context-specific continuation and interruption, not long-term habit change. |

> **Core design decision.** Retain the one-hour intentional checkpoint. Evaluate the opportunities for reflection and the interaction burden it may create; do not assume that it improves time management.

### Current prototype assumptions

- Activity Tokens are **Study, Exercise and Gaming**. Flipping starts a session; LEDs and completion feedback communicate state; the Time Tower displays activity records.
- In demo mode, **eight seconds represent one simulated hour**. Completed records transfer automatically, as specified in the original protocol. No placement on top of the Tower is required.
- Test A includes programmed B_sim records. In Test B, distinguish the real P2 from any simulated data.
- Verify power, attribution cues, acknowledgement, and the actual restart gesture. Do not describe unimplemented functions as validated.

**Suggested scale:** Test A: 2–3 individual participants, approximately 35–45 minutes per session. Test B: 2–3 pairs, approximately 40–50 minutes per pair. The real-hour check is optional. These numbers and timings are project recommendations, not requirements specified by the lecture.

## Part 1 — Research questions and evidence boundaries

### 1. Evaluation purpose and methods

This is a **formative evaluation**: investigate what people actually do, how they interpret shared records, how interaction unfolds between people, and what should change next. Product liking, task completion and satisfaction are not the main outcomes. Do not substitute SUS/TAM scores for social or contextual analysis.

Use unprompted first-use observation, contextual tasks, simulated-record observation in Test A, real paired observation in Test B, separate interviews, short joint discussion and evidence coding. The approach follows the supplied Week 8 Evaluation material, particularly pages 3–8 and 11–17.

### 2. Evaluation question → Evidence → Decision

| ID / design objective | Evaluation question | Evidence and supported interpretation |
|---|---|---|
| **EQ1 — Social awareness** | When do users independently notice, ignore or interpret a new shared record? | Gaze, approaching, questions, spontaneous words, misreading and no response. Distinguish before/after prompts. Test A supports reactions to simulated records. |
| **EQ2 — Social interaction** | With two real users, who initiates, responds or avoids interaction? How do records enter the conversation? | Test B: shared attention, turn-taking, helping, negotiation, silence, withdrawal and both users' words. |
| **EQ3 — Social boundaries** | What interpretations, comparisons and privacy concerns accompany public records? | Compare sharing behaviour with separate interviews. Reluctance to record or be seen is valid evidence. |
| **EQ4 — Mobile/contextual use** | How do Study, Exercise and Gaming transitions, and the Tower's position, affect use? | Interruptions, cables, position, distance, viewing angle, passing behaviour, and holding or placing Tokens. |
| **EQ5 — Hourly checkpoint** | How do users understand the need to actively decide whether to begin again each hour? When is it meaningful or disruptive? | Independent restart, activity switching, no restart and reasons. An eight-second demo only reveals immediate reactions. |
| **EQ6 — Legibility and trust** | Are progress, completion and actual transfer understood? | Independent actions, help, faults, interpretations and the actual display. Only evaluate personal attribution when the display provides it. |

### 3. Keep actual events and simulation separate

| Evidence type | Treatment in this protocol |
|---|---|
| **REAL — Actual event** | Actions, speech, reciprocal responses, actual device states and faults involving real participants. |
| **SIM — Simulation** | Eight-second hours, programmed B_sim records and operator-preloaded data. These are not genuine accumulated activity hours. |
| **PROMPTED — After intervention** | Behaviour following requests such as “look at the Tower” or “try again”. Do not report it as spontaneous. |
| **HYPOTHETICAL — Expected use** | Answers about future housemates or long-term use describe expectations, not demonstrated future behaviour. |

**Scope boundary:** One real user with a virtual housemate is not evidence of completed reciprocal social interaction testing. Test B is intended to address this gap. When it is not conducted, report real paired interaction as unverified.

## Part 2 — Preparation and standardisation

### 4. Participants, setting and team roles

| Element | Execution standard |
|---|---|
| **Participants** | Test A: 2–3 students individually. Test B: 2–3 relevant pairs. Record familiarity and shared-space experience; do not treat strangers and housemates as interchangeable. |
| **Team roles** | One facilitator handles scripts and safety; one observer logs the timeline, device states and speech. An additional technical monitor may assist when available. |
| **Context** | Provide personal activity and shared Tower locations, with real or simulated study, light movement and gaming. Record layout, distance, noise, sightlines, sockets and cable length. |
| **Data and consent** | Observation notes are required for the study. Obtain separate explicit consent for participation, notes, audio, photographs and video. Use participant IDs; avoid private laptop content or messages. |
| **Fairness and safety** | Use safe, light physical activity. Do not morally judge gaming time. Participants may decline recording or disclosure, skip questions or stop. |

### 5. Equipment and version checklist

- [ ] Record firmware/build, date, and available Study, Exercise and Gaming Tokens.
- [ ] Check start, finish and restart for each Token. Write down the actual restart gesture and LED/vibration meanings.
- [ ] Confirm that one completed demo pattern means one **simulated** hour, not an actual hour of study.
- [ ] Verify automatic transfer, the absence of a deposit-on-Tower action, and the actual indication of failure or confirmation.
- [ ] Check whether personal identity is really shown. Activity categories alone do not identify P1 versus B_sim/P2. Omit identity-accuracy scoring when unsupported.
- [ ] Test A: standardise the B_sim script and update timing where possible; otherwise log the actual sequence, timing and display.
- [ ] Test B: disable B_sim or isolate and label it clearly. Reset the display and log the order of genuine contributions.
- [ ] Record USB, power and cable constraints. Ensure movement cannot pull connections loose; prepare fault recovery.
- [ ] Confirm separate media permissions. Label debugging changes and manually added records.

**Pause and recovery:** On unstable power, failed reset, transfer failure or inconsistent records, first capture the immediate user response, time and LED state. Then pause to troubleshoot. Mark **FAULT**, **RESET** or **MANUAL SIM** in the log. If a key condition changes, record a new version and analyse it separately.

## Part 3 — Test A: One real participant and B_sim

**Purpose:** Explore personal interaction clarity, awareness of simulated shared records, spatial/activity transitions and immediate checkpoint reactions. B_sim is stimulus material, not another real participant.

| Stage | Suggested time | Main evidence |
|---|---|---|
| **A0 — Consent and context** | 5 minutes | Permissions, habits and activity settings. |
| **A1 — Unprompted first use** | 4 minutes | Discoverability and points requiring help. |
| **A2 — Completion/checkpoint** | 6–8 minutes | Unprompted end-state response and restart. |
| **A3 — Activity changes/B_sim** | 8–10 minutes | Noticing or ignoring updates; contextual friction. |
| **A4 — Passing/reading Tower** | 5–6 minutes | Independent looking and prompted interpretation. |
| **A5 — Interview/debrief** | 7–10 minutes | Explanations of meaning, sharing and burden. |

### A0 — Welcome, disclosure and context

**Facilitator says:**

> “Thanks for helping. We are testing the prototype, not judging your productivity. This demo fast-forwards one hour into eight seconds, and completed records are automatically transferred to the shared Tower. It will also contain simulated records for a virtual housemate. You may choose not to start a record, skip questions or stop. May we take notes? Audio and video require separate permission.”

**Ask:**

> “Where do you normally study, exercise or game? How do you decide when to change activities? Do you share those spaces with anyone?”

Record baseline answers and whether others' activities influence the participant's arrangements. Do not yet teach the flip gesture, disclose the exact timing of B_sim updates or suggest that P1 should compare productivity.

### A1 — First use without instruction: Study Token

**Task:**

> “Imagine you are about to start studying. Please try using these objects to record that activity in the way that makes sense to you.”

**Observe:** Token selection, first attempts, orientation, LED interpretation, confusion between completion and upload, and words spoken before help.

**Help ladder:** Remain silent for approximately 60–90 seconds. If stuck, ask “Would you like a hint?” Give specific help only with agreement: “Turn the top of the token to the bottom, hold it steady and try again.” Log no help, general hint and specific instruction separately.

After the first attempt, explain once: “For this demo, a one-hour session is accelerated to eight seconds. Completed time is automatically sent to the Tower.” Use this wording only if the hardware behaves accordingly; otherwise stop and amend the script first.

### A2 — End-of-session intentional checkpoint

**Facilitator action:** At the end of the first simulated hour, do not immediately ask for another Study hour. Allow **45–60 seconds without prompting**, while P1 continues the original activity.

**Observe:** Notice of completion; touching, returning or flipping the Token; independent restart, switching, continuing the activity without recording, no action and spontaneous reasons. Separate help from faults.

**Neutral follow-up after observation:**

> “What did you think the token was doing when it finished? What were you thinking of doing next, and why?”

**Controlled second Study attempt, only if needed:**

> “If you decided to record another Study session now, how would you do that?”

Label this attempt **PROMPTED**, not spontaneous restarting.

**Interpretive boundary:** Eight seconds can reveal immediate state understanding and decision-making, not the reflection benefit or interruption cost of a genuine hourly checkpoint. Retaining the hourly action is a team decision, not a predefined successful result.

### A3 — Activity contexts and B_sim updates

**Exercise task:**

> “Please do a brief safe stretch or walk around, and record it if you would like. Then return to your own activity for a moment.”

**Gaming task:**

> “Now try a short gaming activity. You can record it with the Gaming token if you wish, then carry on as you normally would.”

Observe starting and switching Tokens, placement, movement, sightlines and power/cable limits. Alternate Exercise/Gaming order across sessions only when controllable; otherwise keep the order fixed and note why.

Around B_sim updates, leave **30–60 seconds of quiet observation**. Record looking, approaching, asking, comparing, spontaneous talk or no reaction. Identify whether every update came from P1 or B_sim, and whether it coincided with P1's action. Do not mistake a B_sim update for confirmation of P1's recording. Mark manually restored displays **SIM**.

Where appropriate, allow a few minutes of actual reading, short gameplay or rest. These periods can reveal contextual friction but do not represent a complete normal-hour trial.

### A4 — Passing the Tower, interpretation and next choice

**Movement task:**

> “You can take a short break and get some water from the common area, then come back.”

Before asking about the Tower, record looking, stopping, turning, approaching, route, viewing distance/angle and cable restrictions. Do not direct attention to the display.

**Open interpretation after returning:**

> “What information could you see on the Tower? What do you think the patterns mean? What is unclear?”

**Attribution, only when the display identifies contributors:**

> “Which records are yours and which are from the simulated housemate? How can you tell?”

**When contributor identity is not displayed, ask instead:**

> “Does this display allow you to tell who contributed which records? Why or why not?”

If exactly four base trials are completed without extra or missing records, the expected count is **Study 2 / Exercise 1 / Gaming 1**, totalling four simulated hours. Use the actual display snapshot as ground truth; do not force an expected count when extra actions or faults occurred.

**Choice task:**

> “What would you like to do next? You can record again or leave it for now; it is up to you.”

Mark the resulting choice as following a facilitator invitation, not as fully spontaneous behaviour.

### A5 — Post-use interview and debrief

1. “Which moment felt most uncertain? What did you think was happening?”
2. “Did the simulated housemate's records catch your attention? If not, why might that be?”
3. “Did the Tower influence what you chose to do next? Which information mattered?”
4. “What can these patterns tell you about the other person's past activity? What can they not tell you about the present?”
5. “At the checkpoint, did you think more about the next device action or about whether to continue the activity? When could this interrupt you?”
6. “Which kinds of activity records would you be comfortable sharing with housemates or visitors? Which would you keep private, and why?”
7. “How did moving and switching activities affect use? How would power or location constraints matter in another room?”
8. “If you could change one thing, what happened today that makes you choose it? Did knowing B was simulated affect your reaction?”

Thank the participant, reiterate the simulated time and B_sim data, and record requests to retract optional media. Code future housemate or long-term-use predictions as **HYP**, separately from observed events.

## Part 4 — Test B: Two real users in a shared setting

**Purpose:** Address Test A's inability to demonstrate reciprocal interaction. Observe joint viewing, handovers, help, negotiation, avoidance, privacy boundaries and the influence of physical layout.

**Suggested scale:** 2–3 pairs; 40–50 minutes per pair. P1 and P2 are real participants. Disable B_sim where possible, or isolate and label every simulated record. Do not assume that a two-column display identifies two real contributors.

### B0 — Setup and separate consent: About 5 minutes

Record familiarity, shared-space experience, available devices, positions and actual identity-display capability. Obtain individual agreement to participation, media capture and shared visibility. Allow a participant to skip an activity they do not want displayed.

**Facilitator says:**

> “You will both use the activity tokens and shared Tower in the same space. Please act as you normally would. You do not have to talk about the display or record every activity. In this demo, completed records are transferred automatically to the shared Tower, so you may choose not to start a record you do not want visible. You can stop the session at any point. We are not comparing productivity.”

### B1 — Independent activities and optional recording: About 10 minutes

**Task:**

> “Please each work on a brief everyday activity, such as reading, planning or taking a short break. Use the tokens if you find them relevant; doing nothing is also okay.”

Observe independent use, competition or waiting for Tokens, helping, turn-taking, state uncertainty, one person acting for the other, and unresolved errors. If hardware does not support parallel same-activity recording, use turns and label the limitation.

### B2 — Natural encounter in the shared area: 8–10 minutes

**Context transition:**

> “You may finish the task and spend a few minutes in the shared area. What happens next is up to you.”

Do not ask participants to discuss the Tower. Leave at least **2–3 minutes for spontaneous interaction** without continuous think-aloud. Record who looks, approaches or speaks first, responses, pointing, joint attention, silence, avoidance, humour, discomfort and continued individual activity.

### B3 — Prompted joint interpretation: About 6 minutes

**Task:**

> “Now please look at the Tower together and discuss what you think the records show, and what information might be missing. It is fine to disagree.”

Observe negotiated interpretation, turn-taking, disagreement, help and correction. Label all B3 talk **PROMPTED**; it is not B2's unprompted conversation. When contributor identity is absent from the interface, do not score personal attribution as correct or incorrect.

### B4 — Separate interviews, then joint reflection: 12–15 minutes

**Ask each participant privately:**

> “When did you notice the other person or their activity? Did you feel watched or influenced? Was there anything you preferred not to share or explain? Can you identify a specific moment?”

**Ask together:**

> “What could you learn from the shared record? Did any interaction arise naturally, or mainly because of the task? In what situations would you want this visible—or not visible?”

Do not require private activity details. Watch for unequal participation or one person answering for another. Mark each injected record if B_sim cannot be disabled; never count it as a real contribution.

### B5 — Adaptations for hardware constraints

| Constraint | Permitted adaptation | Evidence boundary |
|---|---|---|
| Only one three-Token set | P1/P2 take turns; observe handover, waiting and help. | Does not test concurrent independent timing. |
| Tower shows activities only | Observe joint interpretation; manually log who contributed what. | Does not establish successful contributor identification. |
| Movement limited by cables | Use the actual powered area; log distances and obstacles. | Does not validate free portability or cross-room operation. |
| B_sim cannot be disabled | Mark the simulated area/data and keep an injection log. | Not every display change belongs to a real participant. |

## Part 5 — Optional genuine one-hour checkpoint

Only conduct this additional check when normal-duration firmware is stable and the team needs evidence about interruption after an actual hour. Label it separately from all accelerated tests.

| Stage | Procedure |
|---|---|
| **Natural activity: 55–65 minutes** | Let the participant study or undertake a chosen real activity without continuous think-aloud. Record device placement, checking and interruptions from other events. The trial must reach the actual completion event. |
| **After completion: 3–5 minutes** | Do not immediately prompt. Record stopping, looking, flipping again, switching activity or continuing the original task. |
| **Brief recall: 5–8 minutes** | Ask why they acted as they did, when the action felt meaningful or burdensome, and whether it arose from the device or their own activity plans. |
| **Reporting boundary** | Report context-specific observations, not improved long-term time management. When not conducted, mark the real-hour effect as unverified. |

## Part 6 — Observation and analysis principles

### Event labels: Record behaviour before interpretation

| Code | Meaning |
|---|---|
| **SPONT** | Unprompted event, such as looking up at the Tower after an update without a facilitator reminder. |
| **TASK** | Movement or action caused by a contextual task, without an instruction to discuss the display. |
| **PROMPT** | Pointing or speech after a direct request to inspect or discuss the Tower. |
| **HELP** | General hint or specific instruction from the facilitator. |
| **FAULT** | Misrecognition, transfer failure, reset or power interruption. Record the response before troubleshooting. |
| **SIM** | B_sim data, accelerated demo units or manual records. |
| **HYP** | An expectation about future use, rather than observed behaviour. |

### Rapid analysis after each session

The facilitator and observer should spend **10–15 minutes** reviewing the session. Identify the factual event, the participant's explanation, the design uncertainty or tension, whether to keep/change/retest, and the supporting sheet or recording timecode.

Group findings by EQ1–EQ6. Include not noticing, no conversation, missing uploads and reluctance to disclose. A conversation between two people is one interaction event, not two independent successes. Do not reduce the analysis to task-success counts.

| Reporting statement | Evidence required |
|---|---|
| “In context X, a participant responded to simulated records…” | Test A observation, timing, display evidence and words. |
| “Two real users did/did not interact around a shared pattern…” | Actual Test B paired records. |
| “We retained the active one-hour interaction because…” | A design decision/rationale, including its potential burden; not a proven benefit. |
| “The design aims to support reflection/conversation…” | Clearly marked design intention. |
| “The system improves long-term time management/attention…” | Not supported by the short prototype tests. Do not report it as a result. |

## Part 7 — Print-and-use recording sheets

Print one set per session. Duplicate the Event Log as needed. Write observed facts before interpretations, and capture participants' words as accurately as possible.

### Sheet 1 — Session setup

| Field | Entry |
|---|---|
| Session ID / condition | __________  [ ] Test A: P1 + B_sim  [ ] Test B: P1 + P2  [ ] Real-hour |
| Date / location | ________________________________________________ |
| Participants / relationship | P1: ______  P2, if real: ______  Relationship: __________ |
| Facilitator / observer | ________________________ / ________________________ |
| Consent | Participation [ ]  Notes [ ]  Audio: P1 [ ] P2 [ ]  Images/video: P1 [ ] P2 [ ] |
| Firmware / timing | Version: __________  [ ] 8-second demo  [ ] Real one-hour mode |
| Actual restart gesture | ________________________________________________ |
| Transfer / acknowledgement | [ ] Automatic transfer verified  [ ] Visible confirmation  Other: ______ |
| Personal attribution | [ ] Explicit contributor IDs  [ ] Activity categories only  [ ] Unsure |
| Simulation | [ ] B_sim OFF  Script ID: ______  [ ] Uncontrolled timing |
| Power / setting | USB/adapter: ______  Tower position: ______  Distance: ______  Noise: ______ |
| Task order | Study-1 → checkpoint → Study-2 → Exercise/Gaming; specify order: ______ |
| Known constraints | ________________________________________________ |

### Sheet 2 — Preflight equipment check

| Check | Status | Notes |
|---|---|---|
| Three activity Tokens respond | [ ] OK [ ] Fail | ____________________ |
| Start/finish/restart match script | [ ] OK [ ] Fail | ____________________ |
| Eight-second demo creates a simulated unit | [ ] OK [ ] Fail | ____________________ |
| Actual transfer and display confirmed | [ ] OK [ ] Fail | ____________________ |
| B_sim scripted or disabled correctly | [ ] OK [ ] Fail | ____________________ |
| Reset, capacity and overflow inspected | [ ] OK [ ] Fail | ____________________ |
| Cables, safe movement and camera scope | [ ] OK [ ] Fail | ____________________ |

### Sheet 3 — Event log: Duplicate as needed

**Codes:** SPONT / TASK / PROMPT / HELP / FAULT / SIM / HYP.

| Time / timecode | Actor and context | Observed action / exact words | Code | Display / source |
|---|---|---|---|---|
| ______ | __________ | ________________________ | ______ | __________ |
| ______ | __________ | ________________________ | ______ | __________ |
| ______ | __________ | ________________________ | ______ | __________ |
| ______ | __________ | ________________________ | ______ | __________ |
| ______ | __________ | ________________________ | ______ | __________ |
| ______ | __________ | ________________________ | ______ | __________ |
| ______ | __________ | ________________________ | ______ | __________ |
| ______ | __________ | ________________________ | ______ | __________ |
| ______ | __________ | ________________________ | ______ | __________ |
| ______ | __________ | ________________________ | ______ | __________ |

Do not put researcher assumptions in the observed-action field. On a fault, record the visible response first and the technical cause separately.

**Prompt and assistance audit**

| Prompt time | Facilitator words/action | Behaviour after prompt | Help level |
|---|---|---|---|
| ______ | __________________ | __________________ | [ ] None [ ] General [ ] Specific |
| ______ | __________________ | __________________ | [ ] None [ ] General [ ] Specific |
| ______ | __________________ | __________________ | [ ] None [ ] General [ ] Specific |

### Sheet 4 — Display, checkpoint and context

| Observation | What actually happened |
|---|---|
| Study attempt 1 | [ ] Independent [ ] General hint [ ] Specific hint [ ] Fault. Flip/LED interpretation: __________________ |
| First checkpoint | [ ] Self-restart [ ] Switch [ ] No action [ ] Misunderstood. Action during 45–60 seconds: __________________ |
| Study attempt 2 | [ ] Spontaneous [ ] Prompted [ ] Helped [ ] Failed. Reason: __________________ |
| B_sim / other update | Source/time: ______  [ ] Noticed [ ] Ignored. Gaze/words/actions: __________________ |
| Shared Tower viewing | [ ] Spontaneous glance [ ] Stopped [ ] Approached [ ] Prompted only. Position/angle/distance: __________________ |
| Pattern interpretation | Actual Study/Exercise/Gaming: ______  P1's answer: ______  Actual B_sim/P2 contribution: ______  Interpretation: ______ |
| Social response: Test B | Initiator: ______  Respondent: ______  Duration: ______  Pointing/help/negotiation/avoidance: __________________ |
| Context constraints | Movement: ______  Power: ______  Competing attention: ______  Unwanted interruption: __________________ |
| Sharing boundaries | Observed non-sharing / expressed concern: __________________ |

**Short post-use reflection**

| Prompt | Participant's words and circumstances |
|---|---|
| What influenced the next action? | __________________________________________ |
| What did the shared records mean? | __________________________________________ |
| When did the checkpoint help or hinder? | __________________________________________ |
| What would they change? | __________________________________________ |

### Sheet 5 — Finding → Insight → Decision

| Evidence ID / EQ | Observed fact and words | Interpretation / design tension | Decision / status |
|---|---|---|---|
| E___ / EQ___ | __________________ | __________________ | [ ] Keep [ ] Change [ ] Retest. Status: ______ |
| E___ / EQ___ | __________________ | __________________ | [ ] Keep [ ] Change [ ] Retest. Status: ______ |
| E___ / EQ___ | __________________ | __________________ | [ ] Keep [ ] Change [ ] Retest. Status: ______ |
| E___ / EQ___ | __________________ | __________________ | [ ] Keep [ ] Change [ ] Retest. Status: ______ |

**Cross-session synthesis**

| Dimension | Evidence / count with denominator | Contradictions and limitations |
|---|---|---|
| Awareness | ___ of ___ sessions noticed before a prompt | __________________ |
| Social: Real pairs only | ___ of ___ pairs initiated unprompted Tower discussion | __________________ |
| Hourly checkpoint | Spontaneous restarts: ___ / ___; eight-second demo only | __________________ |
| Contextual mobility | Power/position barriers in ___ / ___ sessions | __________________ |
| Privacy / non-use | Refusal, concern or silence; do not force scoring | __________________ |

**Illustrative reporting example — not a finding from a completed session:**

> “In this accelerated prototype session, P01 did not notice the simulated housemate update until prompted (event E03). This suggests the Tower's visibility may be limited from the study position; a situated two-user follow-up is needed.”

**Avoid:** “The system improves time management and makes users social.” The short demonstration cannot establish long-term change, and simulated B cannot establish real reciprocal interaction.

## Appendix — Sources, change log and team handover

### Changes from the original Round 2 protocol

- **Preserved:** Study/Exercise/Gaming, eight-second demonstration, automatic transfer, B_sim, natural non-response and personal/shared contexts.
- **Added:** Separate Test B with two real users; unprompted encounter and prompted joint-viewing phases.
- **Changed:** First-use attempt before flip instruction; 45–60 seconds of unprompted observation at completion instead of an immediate second Study request.
- **Added:** EQ5 for the retained one-hour checkpoint and optional real-hour observation. The active hourly cycle is not replaced with automatic continuous timing.
- **Added:** Attribution only when displayed, and controls for firmware, scripts, faults, prompting and cables.
- **Added:** Privacy questions, neutral scripts, recording sheets and evidence-to-decision limits.

### Reading basis

1. **DECO3500 Week 8 Evaluation slides, Semester 2, 2026.** Pages 3–4: formative evaluation, multiple users and situated use; pages 6–8: limits of SUS/TAM and design objective → questions → evidence; pages 11–13: adapting observation, interviews and contextual tasks; pages 16–17: social behaviour and contextual evidence.
2. **DECO3500_Paired_Prototype_Testing_Protocol.docx, Round 2.** Original Test A, eight-second timing, B_sim, four base trials, automatic upload, location task and interview domains.
3. **Time_Tokens_Evaluation_Protocol_FIXED.docx, revised 17 October 2026.** Source for this English-only consolidation. Real paired and real-hour conditions are planned extensions, not retrospective descriptions of completed tests.

### Final pre-use note

**Sign-off:** Facilitator: __________  Observer: __________  Firmware checked: __________  Recording sheets ready: __________  Date: __________

Do not silently alter task wording during a session. If capabilities differ from these assumptions, document an explicit protocol amendment and version before subsequent sessions. If Test B is not completed, state that real paired interaction remains untested.
