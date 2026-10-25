# Communication

How a completed time unit gets from a Token to the Tower.

| Folder | What belongs here |
|---|---|
| — | *Nothing committed yet* |

## Design

ESP-NOW between the Token's ESP32-S3-Zero and the Tower's receiver, with the receiver
forwarding over UART to the Arduino UNO R4 Minima that drives the display.

A Token accumulates completed units locally and sends a batch on transfer rather than
sending each unit as it finishes. The two counts are kept separate: a running total that
never decreases, and a pending count that clears only when the Tower confirms it has
processed the batch — not when the radio reports a successful send. Without that
distinction a lost acknowledgement either drops time or counts it twice.

## Status

**Designed, not integrated.** The hardware is in place and space is reserved on both
boards, but the path has not been verified end to end, so the two devices have been
developed and tested separately. The full loop has never run in front of a user.

Round 2 found that transfer feedback is already a trust problem at the interaction level
(theme T5), independently of whether the radio works:
[`/research/user_test/round_2/results/`](../../research/user_test/round_2/results/Time_Tokens_Evaluation02_Analysis_EN.md).

<!-- TODO: commit the protocol definition — message format, batch and acknowledgement
     numbering, and what happens when an acknowledgement is lost. -->

---

[← Prototype](../README.md)
