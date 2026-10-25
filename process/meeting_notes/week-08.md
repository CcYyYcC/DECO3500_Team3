# 2026-09-15 — Week 8 (studio, Tue + Thu)

**Present:** Yiyang Qin · Bo Wang · Yucheng Chen · Zhaoguo Huang · Yuming Zhong

**State:** Requirements finalised. Functional build started. Round 1 testing being scheduled.

**Discussed / decided:**
- Concept reduced to **Time Tokens & Time Tower**: separate tokens, each turning a chosen hour
  into a tangible unit, and a Tower that collects completed units so the day reads as a
  mixture of activities rather than one productivity score.
- Core cycle fixed as **Flip → Track → Transfer → Reflect & Restart**.
- **Cylinder rejected after building it.** The minimal build used three LEDs and a ball tilt
  switch. It exposed three limits: a cylinder rolls, its resting direction is ambiguous, and
  three LEDs can only show a small set of states. Replaced with a **rounded cuboid with two
  stable faces**, an **8×8 LED dot matrix** and a **tri-axis orientation sensor**.
- A capsule twist mechanism was examined and rejected: more custom mechanical design, more
  failure points, and attention pulled away from the flip interaction the concept exists to
  test.
- **No punitive lock-out.** The system notifies and invites reconsideration; it never prevents
  play. Activity meaning must not rely on colour alone, so the matrix carries an icon too.
- R1–R5 agreed and documented.

**Blockers:**
- Enclosure iterations still in progress; first shell was larger than it needed to be and the
  wiring had nowhere to sit.

**Next steps:**
| Member | Action | By |
|---|---|---|
| Yiyang Qin | Enclosure iterations — reduce to 60 mm, add cable clearance, redesign front retention | Week 9 |
| Bo Wang | Timing cycle and display firmware; get one Token working end to end | Week 9 |
| Zhaoguo Huang | Round 1 test plan, tasks and observation sheets | Week 9 |
| Yuming Zhong | Recruit Round 1 participants | Week 9 |
| Yucheng Chen | Complete the Ethical Disclaimer canvas; build the wiki structure | Week 9 |

---

[← Week 7 (studio, Tue + Thu)](week-07.md) · [All meetings](README.md) · [Next →](week-09.md)
