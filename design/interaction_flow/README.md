# Interaction Flow

How the final system is used, shown on the built prototype.

| File | What it shows |
|---|---|
| `interaction-flip-track-transfer.jpg` | Flip prompt, timing, and transfer confirmation on the built Token |
| `current-build-tower-and-tokens.jpg` | Latest build — three Tokens, the Tower's accumulated traces, and the hook-and-loop planning strips |

Both are sourced from the final A2 poster
([`../poster/TimeTokens_A2_Poster.pdf`](../poster/TimeTokens_A2_Poster.pdf)) at print
resolution. Replace them with the original camera files when those are added — keep the
filenames so nothing linking here breaks.

## The flow

1. **Choose and start** — pick the Token for the activity and place it in the start
   orientation.
2. **Track** — the LED matrix animates while the session runs, and plays a completion
   animation when the unit is done.
3. **Transfer** — the completed unit goes to the Tower and joins the shared record.
4. **Read** — the Tower stacks the day as accumulated blocks, one block to an hour.

Round 2 found that state and transfer feedback are still not clear enough to trust (theme
T5): see
[`/research/user_test/round_2/results/`](../../research/user_test/round_2/results/Time_Tokens_Evaluation02_Analysis_EN.md).

## Referencing these from the wiki

The wiki is a separate repository and cannot use relative paths into this one:

```html
<img src="https://raw.githubusercontent.com/CcYyYcC/DECO3500_Team3/main/design/interaction_flow/current-build-tower-and-tokens.jpg" width="420">
```

---

[← Design](../README.md)
