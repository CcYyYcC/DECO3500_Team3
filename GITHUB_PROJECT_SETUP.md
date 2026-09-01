# 任务看板搭建指南 · Task Board Setup Guide

用 GitHub Projects（新版，Projects v2）来实现你说的五阶段流程。这是账号/组织级别的看板功能，需要在网页上点几下设置，不是靠提交文件生成的——下面是具体步骤。

This uses GitHub's newer Projects (Projects v2) feature to implement your 5-stage workflow. It's configured via the GitHub web UI, not committed as a file — here's exactly how.

---

## 第一步：创建看板 · Step 1: Create the Project

1. 进入你们的 repo → 点顶部 **Projects** 标签 → **New project** → 选择 **Board** 模板
2. 命名，比如 `GG Design Project Tracker`
3. Go to your repo → **Projects** tab → **New project** → **Board** template → name it

## 第二步：设置五个状态列 · Step 2: Set the 5 status columns

默认看板会带 Todo / In Progress / Done 三列，把它改成你要的五列：

进入看板 → 点 **Status** 字段旁的 **⋯** → **Edit field** → 依次改成：

1. 📥 **Backlog** — 以后可能要做，还没排期
2. 📋 **Todo** — 已确定要做，分配到具体成员，还没开始
3. 🔨 **In Progress** — 正在做
4. 👀 **Review** — 做完了，等队友检查
5. ✅ **Done** — 完成

Open the board → click **⋯** next to the **Status** field → **Edit field** → rename/add columns to match the above five stages.

## 第三步：加两个自定义字段 · Step 3: Add two custom fields

除了默认的 Assignee，建议再加：

- **Week**（Single select）：Week 6, Week 7 … Week 13 — 方便按周筛选任务，直接对应 `WEEKLY_PLAN.md` 里的周次
- **Category**（Single select）：Research / Design / Prototype / Docs / Ethics — 方便看某一类工作的整体进度

添加方式：看板右上角 **+** → **New field** → 选 Single select，填选项。

Add via the **+** button top-right of the board → **New field** → Single select, then fill in the options above.

## 第四步：把任务变成 issue · Step 4: Turn tasks into issues

每条具体任务建一个 GitHub Issue（不是直接在看板卡片里打字，issue 有讨论区、可以关联PR，更适合团队协作留痕）。用下面的模板：`.github/ISSUE_TEMPLATE/task.yml`（已经帮你写好，提交到 repo 后，点 **New issue** 就会看到这个表单）。

Create one GitHub Issue per task (rather than typing directly on a card — issues support discussion threads and PR linking, which is better for team traceability). Use the template below: commit `.github/ISSUE_TEMPLATE/task.yml` to your repo, and it'll show up as a form when you click **New issue**.

新建的 issue 会自动进 Backlog；确定要做了就拖到 Todo，同时在 Assignee 里指定负责人。

New issues land in Backlog by default; once scheduled, drag to Todo and set the Assignee.

## 第五步（可选）：自动化 · Step 5 (optional): Automation

看板设置里的 **Workflows** 可以设置：
- Issue 被 assign → 自动移到 Todo
- 关联的 PR 打开 → 自动移到 In Progress
- 关联的 PR 被 review request → 自动移到 Review
- Issue 被 close → 自动移到 Done

不是必须，但能省去手动拖卡片的功夫。

In the board's **Workflows** settings, you can automate the moves above (assigned → Todo, PR opened → In Progress, review requested → Review, issue closed → Done). Optional, but saves manual dragging.

---

## 每周怎么用 · Weekly Routine

1. 每周初：把 `WEEKLY_PLAN.md` 当周的 checklist 拆成 issue，扔进 Backlog/Todo，按人分配
2. 每天/每次开会：更新自己任务的状态（拖卡片）
3. 每次内部 stand-up 后：把 Done 的任务截图或列表贴进 WIKI 的 stand-up 记录页（这也是作业要求的一部分）

1. Start of each week: break that week's checklist from `WEEKLY_PLAN.md` into issues, assign owners
2. Daily/at each meeting: drag cards to reflect real status
3. After each internal stand-up: paste the Done list/screenshot into that week's WIKI stand-up record (this is required documentation per the brief)
