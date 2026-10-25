# A2 Poster — Copy & Image Brief (v1)

**For:** whoever lays out the poster
**Format:** A2 portrait, 420 × 594 mm, read at close range
**Body word count:** ~690 words (excluding title, captions, footer)
**Status:** draft for team review — three items need a decision before print (see "Open decisions" at the end)

> 给排版的人：下面每个区块标了 **[ZONE n]**，括号里是建议占版面的比例。英文是直接上版的最终文案，中文是给你的说明，不要排进去。图片部分每张都给了拍摄/制作要求，按编号对应。

---

## [ZONE 1] Header — 顶部通栏（约 15%）

### Title

**Time Tokens**

### Subtitle

Making time visible, together

### One-line concept

> Three physical activity tokens record time spent on study, entertainment and rest. Completed sessions transfer to a shared Time Tower, where the day accumulates as a visible record everyone in the space can see.

**图片：** IMG-01（主图，整个 header 的视觉锚点）

---

## [ZONE 2] Problem & Opportunity — 上部左栏（约 15%）

### The Problem

We began with the domain of combatting computer game addiction. Our interviews pointed somewhere else: the issue was rarely the total hours played. It was that a game could take priority over a real-life commitment at the wrong moment — and that people often could not see this pattern until afterwards.

So we stopped designing a restriction, and started designing for visibility.

### Design question

> **How might we make everyday time use visible and support reflection — without restricting what people choose to do?**

**图片：** IMG-02

---

## [ZONE 3] Research & Development — 上部右栏（约 15%）

### How we got here

**Initial domain** → **Interviews & analysis** → **Physical prototype** → **User evaluation**

Interviews reframed the problem from *how much* people play to *when* play competes with something else. That reframing led to a physical, low-friction object rather than an app: starting a session had to take one gesture and no screen.

Four physical iterations followed, each driven by a constraint we hit in the last — component fit, cable clearance, LED retention, and finally the assembled 60 mm enclosure. Evaluation then surfaced a tension we had not designed for: the system asks for an hourly gesture, but an hourly gesture interrupts the very activity it is recording.

**图片：** IMG-03

### Where the idea came from

**In Time (2011).** In the film, time is currency: everyone carries a glowing countdown on their forearm, and every transaction is visible on the body. What stayed with us was not the dystopia but the premise — that time becomes a different thing once you can *see* it. We took the visibility and deliberately left out the scarcity: our tokens show where time went, they never take it away or run out.

**Daily Stack (2010), Anders Hojmose & Sebastian Rønde Thielke, CIID.** A set of wooden bricks, each standing for a task type and a span of time, physically stacked on a base as you work. It showed us that stacking is legible on its own — a pile has a height, a colour mix and a shape you can read across a room, with no screen and no numbers. Daily Stack kept the record personal, on one desk. Our Time Tower moves that stack into a space other people share, which is where our project departs from it.

> 两个灵感正好是一组对照：*In Time* 给了"可见"，Daily Stack 给了"可堆叠"，我们加的是"共享"。这句话可以作为这一栏的小标题或图注。

**图片：** IMG-10（可选）

---

## [ZONE 4] How It Works — 中部通栏（约 22%，图片为主）

> 这一栏是海报的视觉主体。四张照片横向排列，编号 ①–④，每张配标题和一句说明。

**① Choose & Start**
Pick the token for what you are about to do. Place it in the start orientation and the session begins.

**② Follow the feedback**
The LED matrix animates while the session runs, and plays a completion animation when the time unit is done.

**③ Transfer your time**
Completed units are carried to the Time Tower and added to the shared record.

**④ View & reflect**
The Tower stacks the day as accumulated blocks — one block, one hour.

*Exhibition note (小字，放在这一栏角落):* Demonstration timing is accelerated. One block represents one hour.

**图片：** IMG-04 / IMG-05 / IMG-06 / IMG-07

---

## [ZONE 5] What We Learned — 下部左栏（约 18%）

### Evaluation

Three participants, two study sessions each, with one hour compressed to eight seconds. Only the study token was tested.

| What we found | What we are exploring |
|---|---|
| **Flipping did not always register.** Participants could not tell a wrong gesture from an unresponsive device. | Make a valid flip register consistently, with immediate acknowledgement. |
| **The stack did not explain itself.** Participants read the Tower as "completed activity" but could not reliably say whether it showed one hour or two. | Make the unit and the cumulative total legible without a verbal explanation. |
| **An hourly gesture competes with the activity.** Returning to flip the token every hour was described as impractical. | A start/end interaction that can hold a session longer than one hour. |
| **A shared display changes what people record.** One participant said they would show study and conceal gaming. | Separated user columns, and private/shared modes, before claiming any social effect. |

> **Understanding an action is not the same as wanting to repeat it every hour.**

*小字:* The multi-user scenario was hypothetical — the current prototype does not yet demonstrate it.

**图片：** IMG-08

---

## [ZONE 6] Social Value, Limits & Next Steps — 下部右栏（约 15%）

### Why shared, not personal

**Shared traces, different moments.** Records contributed at different times accumulate into one visible trace. Nobody has to be present at the same moment for the Tower to become a common reference — for noticing a pattern, or for starting a conversation about it.

**Personal recording, everyday settings.** Tokens are designed to sit where the activity happens — a desk, a sofa — while the Tower stays in the shared space. Recording is individual; reading it is collective.

> A shared record for reflection — not a productivity leaderboard.

### Where we are

Orientation sensing and the LED display are working. The token-to-tower transfer is designed and the hardware is in place, but not yet integrated. Haptic feedback is specified and not yet functioning.

### Next

Reliable transfer, a session that can run longer than an hour, and visibly separated records per person — then an evaluation with more than one person in the room.

---

## [FOOTER] 底部信息栏（约 5%）

DECO3500 Social & Mobile Computing · Team 3
Yucheng Chen · Yiyang Qin · Bo Wang · Zhaoguo Huang · Yuming Zhong

**[QR code]** → github.com/CcYyYcC/DECO3500_Team3

References (设计时按实际引用补,保持格式一致):
Frommel, J. & Mandryk, R. L. (2022). Daily Quests or Daily Pests? *Proc. ACM Hum.-Comput. Interact.* 6, CHI PLAY, Article 226.
Baumeister, R. F. & Leary, M. R. (1995). The Need to Belong. *Psychological Bulletin*, 117(3), 497–529.
Hojmose, A. & Rønde Thielke, S. (2010). *Daily Stack.* CIID, Copenhagen. hojmo.se/work/daily-stack
Niccol, A. (dir.) (2011). *In Time.* Regency Enterprises.

---

# 图片清单

> 优先级 ★★★ = 没有就没法排版；★★ = 重要；★ = 有则更好。
> 所有原型照片必须拍**现场实际展出的那一版**，不要用早期版本混用。

| 编号 | 内容 | 要求 | 优先级 |
|---|---|---|---|
| **IMG-01** | 主图：一个 Token 和 Time Tower 同框 | 浅色干净背景，略俯视，让两者的尺寸关系看得出来（Token 约 60 mm，Tower 约 400 mm）。Tower 上要有已点亮的方块，不要全暗。横构图，分辨率够放 A2 通栏（建议长边 ≥ 4000 px） | ★★★ |
| **IMG-02** | 问题场景 | 桌面上同时存在"要做的事"和"想做的事"——例如摊开的课本旁边亮着的屏幕。不要拍人脸 | ★★ |
| **IMG-03** | 迭代过程 | 四代外壳并排，或早期草图 + 当前成品的对比。能直接用 `prototype/figures/` 里的现成图 | ★★ |
| **IMG-04** | 步骤① 选择与开始 | 手持 Token、正在放置到起始朝向的瞬间。三个 Token 的盖面图标要能看清 | ★★★ |
| **IMG-05** | 步骤② 计时反馈 | Token 正面特写，LED 点阵显示计时动画中。暗一点的环境拍，让 LED 清楚 | ★★★ |
| **IMG-06** | 步骤③ 传输 | **按现场真正实现的方式拍**。如果展出时传输仍未打通，这张改为"Token 完成动画 + Tower"的并置图，文案同步改（见下方 Open decisions） | ★★★ |
| **IMG-07** | 步骤④ 查看 | Tower 上新增一块之后的状态，最好能看出堆叠关系。可以拍成 before/after 两格 | ★★★ |
| **IMG-08** | 测试现场 | 参与者正在操作 Token 的照片，脸部不入镜或打码。没有合规照片就用观察记录表的局部特写代替 | ★★ |
| **IMG-10** | 灵感对照（可选） | 两格并置的小图：左边一个发光的倒计时/手臂意象（**不要用电影剧照，有版权**，自己画一个抽象的发光数字条即可），右边 Daily Stack 的实物参考图（**用他们官网的图必须标注来源**，或者自己搭一组木块拍一张）。整体做成窄条，放在 ZONE 3 底部 | ★ |
| **IMG-09** | Tower 图例 | 不是照片，是矢量图：颜色/形状 ↔ 活动类型 ↔ 一块 = 一小时。**必须和展出版本一致** | ★★★ |

---

# 排版建议

```
┌─────────────────────────────────────┐
│  [ZONE 1] Title + concept + IMG-01  │  通栏
├──────────────────┬──────────────────┤
│ [ZONE 2] Problem │ [ZONE 3] Research│  双栏
├──────────────────┴──────────────────┤
│  [ZONE 4] How It Works  ① ② ③ ④     │  通栏，视觉主体
├──────────────────┬──────────────────┤
│ [ZONE 5] Learned │ [ZONE 6] Social  │  双栏
├──────────────────┴──────────────────┤
│  [FOOTER] 团队 · QR · References     │
└─────────────────────────────────────┘
```

- 正文字号按 A2 近距离阅读设，正文不小于 18 pt，图注不小于 14 pt
- IMG-09 图例建议放在 ZONE 4 的右下角或 ZONE 6 开头
- 三句加粗引用句（设计问题、"Understanding an action…"、"A shared record…"）是全海报的记忆点，排版上要拉开层级

---

# Open decisions — 排版前必须定

1. **传输方式**：展出时 Token → Tower 的传输是真跑通，还是手动模拟？决定 IMG-06 怎么拍、ZONE 4 ③ 的文案怎么写。目前文案写的是中性的"carried to the Time Tower"，如果打通了应改成具体动作。
2. **计时规则**：沿用"一小时结束后再翻一次开始下一段"，还是改成连续计时？ZONE 4 ①② 和 ZONE 5 第三条都依赖这个。
3. **Tower 是否区分个人**：如果展出版本不区分，ZONE 6 的"shared traces"要避免让人以为能看出谁是谁。

> 这三项定下来之前，文案里所有关于传输和多人的表述都保持"设计意图"的口吻，不要改成已实现。
