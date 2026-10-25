# Evaluation Analysis — Both Rounds

What two rounds of formative testing established, what they contradict, and what neither
could answer.

| | Round 1 | Round 2 |
|---|---|---|
| **When** | After the first build | 18 October 2026 |
| **Prototype** | Study Token only, single-column Tower | Three Tokens, two-column Tower |
| **Participants** | 3, individually | 2, individually |
| **Social layer** | Described as a hypothetical | Enacted against a programmed housemate (B_sim) |
| **Findings** | E1–E6 | T1–T6 |

Evidence: [`round_1/`](round_1/README.md) · [`round_2/`](round_2/README.md)

> Participant labels do not carry across rounds. Round 1's P01–P03 and Round 2's P01–P02 are
> different people. Cite the round alongside any segment ID.

---

## What the second round confirmed rather than resolved

Round 1 produced planned responses to three problems. Round 2 tested the build those plans
produced, and found the same problems still present.

**Feedback and trust.** Round 1 found that a valid flip did not reliably register, leaving a
wrong gesture and an unresponsive device indistinguishable (E1), and that start, timing and
completion were not distinguishable at a glance (E3). Round 2's T5 reaches the same place
from the other direction: unclear state and transfer feedback weaken trust in recorded time.
A participant who cannot tell whether their hour was captured has no reason to believe the
Tower.

**The unit.** Round 1 found the stack did not explain its own worth (E2). Round 2's T3 puts
attribution, activity symbols and display comfort together as a single communication problem
— the Tower is now carrying more information, across two columns and three activity types,
without having solved the legibility of the one block it started with.

That three design responses were planned and are still outstanding is the honest summary of
where the prototype stands. It is not a failure of the plans; it is the gap between deciding
to fix something and having fixed it.

## What changed between rounds

**The two-column Tower exists because of E4.** Round 1 found that a shared display changes
what people are willing to record — one participant said they would show study and conceal
gaming. Separating records per person was the response, and Round 2 could then test a
display where two people's time sat side by side.

The result was not simple reassurance. T2 found that shared records do invite social
interpretation, which is the mechanism the whole concept depends on, and that they can also
invite unwanted evaluation, which is the risk the concept has to survive. Both were present
in the same sessions. The design question is no longer whether comparison happens but
whether a person can choose what to expose to it.

## Where the rounds disagree

**On the hourly unit.** Round 1's E5 is unambiguous: the hourly gesture competes with the
activity it records, and participants who understood the flip perfectly well still said they
would not come back to repeat it. Round 2's T4 reads the same checkpoint as a deliberate
design choice, with the objection moved to unit *size* — an hour leaves short activity
unrepresented.

These are not the same claim, and the difference is worth keeping rather than smoothing over.
Round 1 tested a Token that could only record study, where returning every hour bought the
participant nothing new. Round 2's participants had three activities to distinguish between,
which gives the checkpoint a job it did not previously have. Whether the hourly return is
burden or structure appears to depend on how much the system is being asked to represent.

Unresolved either way: a session that runs longer than an hour still needs a start/end
interaction that does not exist.

## What neither round tested

**Real multi-user use.** Round 1 could not touch the social layer — every question about
sharing concerned a Tower described to the participant. Round 2 enacted it against a
simulation: one real participant, with a programmed housemate holding the second column.

Test B — two real users in a shared setting — is specified in the Round 2 protocol and
**was not conducted**. The social claim is therefore the part of the concept with the least
evidence behind it, which is awkward, because it is also the part the concept rests on.
Nothing in either round should be read as evidence of genuine multi-user behaviour.

**Sustained use.** Both rounds compressed an hour into eight seconds. That shows what happens
when a session *ends*; it cannot show what an hourly prompt feels like during four hours of
actual work, which is exactly where E5 and T4 disagree.

**Behaviour change.** E6 states it directly: awareness is not the same as behaviour change.
Both rounds measured whether people understood and how they felt about it. Neither measured
whether anybody's time use changed, and no claim of that kind is supportable from this
evidence.

## Mobility

Round 2's T6 found that mobility depends on the activity setting and the hardware, not
merely on how small the Token is. The Token runs on external USB power and has no battery in
this version, so the contextual claim — recording where the activity happens — currently
holds in the places a cable reaches.

## What the next round would need to establish

1. **Test B, run properly.** Two real people, a shared space, both contributing to the same
   Tower. Observe before prompting: whether they look at it, point at it, explain it to each
   other or ignore it is itself the finding, and a task that instructs them to discuss the
   display manufactures the behaviour being observed.
2. **Whether the feedback fixes worked.** E1, E3 and T5 describe the same trust problem
   across two rounds. The test is whether a new user can tell, unaided, that their time was
   captured.
3. **Legibility of the two-column display.** T3's attribution question, tested on someone who
   has not had it explained.
4. **What people choose not to submit, and why.** E4 and T2 both point here, and neither
   round asked it of a person with something they actually wanted to keep private.

---

Round 2's own analysis, with the full coding matrix and sixteen codes:
[`round_2/results/Time_Tokens_Evaluation02_Analysis_EN.md`](round_2/results/Time_Tokens_Evaluation02_Analysis_EN.md)

Narrative write-up of both rounds:
[User Evaluation](https://github.com/CcYyYcC/DECO3500_Team3/wiki/User-Evaluation)
