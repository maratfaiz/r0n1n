# R0N1N UX/UI concept

Given constraints (details in `HARDWARE.md`): a 128×64 monochrome screen, a
5-way D-pad + Back, no touch, single-digit-to-low-tens of KB of free RAM.
Any "swipe" metaphor is implemented through buttons — D-pad directions need
to become predictable "gestures," not a metaphor for its own sake.

## Home — a "dashboard," not a launcher

The stock approach across every fork is "home menu = app list." R0N1N
changes the role of the home screen itself: it shows the **device's
state**, not a list of things you could launch.

Large **time**, **date** below it, **battery** in the corner, a thin strip
of system indicators (BLE, USB, SD, active radio, current profile). A
dolphin/animation is optional, via asset packs.
Minimal by design: the essentials are visible immediately, no menu diving
required.

## Navigation model — "pseudo-swipes"

A single, consistent law of motion from Home, identical across every
section:

| Action | Result |
|---|---|
| **OK** | Open the App Launcher (grid/list with categories) |
| **Left / Right** | Switch "desktops" (Radio ⇄ Cards ⇄ IR ⇄ USB/HID ⇄ Dev/GPIO); the set and order depend on the active profile |
| **Down** | Control Center: quick toggles (BLE, backlight, sound/vibration, silent mode, USB mode, TX-lock, active external module), sliders, profile switcher |
| **Up** | Quick Actions / Favorites: a customizable grid of favorite actions, hold-OK to move slots, hold-Back to pick an action; also shows the "last capture" |
| **Hold Back** | Global search (apps, capture files, actions, settings), typed via the system keyboard, incremental |
| **Long-press OK on Home** | Recent — last-used apps/files |

It feels like flipping through phone home screens, but it's implemented
entirely with buttons — no imitation of gestures that would require touch.

**As implemented** (see `ROADMAP.md`, Stages 1–2): OK opens the R0N1N
Applications menu (categories, then Files, Captures, Search, Hub, all apps,
Settings); Left/Right open the active profile's sections as a carousel
(Right starts at the first, Left at the last); hold Back opens Search on
release, because holding it for 5 s is still the stock power-off menu. The
stock favorites stay on hold Left/Right.

## Visual language ("v2")

Checked as real-resolution mockups before it was built:

- one readable font for all text (FontSecondary, now with Cyrillic);
- an inverted 10 px header bar: title left, an "n/N" position or the state
  on the right; clock and battery only on Home and in Control Center;
- selection is always a filled rounded shape with inverted content;
- tiles carry icons only, the selected tile's full name is in a caption
  (or, in Control Center, in the header together with its state);
- 13 px list rows, four visible, with a scrollbar;
- no hint rows: OK opens, Back returns, arrows move — on every screen.

## Profiles/modes — the core of the UX concept

The same build serves a mixed audience through a profile, not through a
forked firmware:

- **Everyday** — large elements, step-by-step wizards, "safe" tools (IR
  remote, U2F, TOTP, reading your own cards).
- **Pentest** — dense lists, the full RF/NFC/USB toolset, keybinds,
  Capture Timeline front and center.
- **Dev** — GPIO/UART/SPI/I2C/SWD, a logic analyzer, a console/CLI, the JS
  runner.
- **CTF** — notes, a timer, "cheat sheets," exporting findings to the
  companion.

A profile changes: the set and order of "desktops," interface density, the
content of Quick Actions/Control Center, and the level of hints. Switching
profiles takes 2 presses from the Control Center.

## Cross-cutting system services

- **Capture Timeline** — a single chronological feed of every artifact
  (.sub/.nfc/.rfid/.ir), tagged by type/frequency/time; replay, export, and
  send-to-companion from one place instead of scattered across each app's
  own folder.
- **Global Search** — search across apps, files, actions, and settings;
  the index lives on SD (see the RAM constraints in `HARDWARE.md`), not in
  memory.
- **Favorites/Recent** — quick access to frequently used tools.
- **Contextual actions** — a long OK-press on any object opens a single
  menu (emulate/save/rename/export/delete) — the same behavior everywhere,
  instead of each app reinventing it.
- **Progressive hints** — a hint line is visible in Everyday, hidden in
  Pentest/Dev.
- **Notifications via RGB/vibration/sound** — a configurable layer,
  including a "silent mode" (LED only).
- **Consistency** — one system keyboard, one file picker, one confirmation
  dialog for "sharp" operations, shared across the whole firmware.

## Where each piece comes from technically

Everything here is written for R0N1N on top of the official Flipper Zero
firmware (see `FIRMWARE_LANDSCAPE.md`), reusing its existing pieces where
they fit (Control Center is the stock lock menu today, JS modules come
from the official JS engine); ideas from other firmwares are
reimplemented, not copied. The Home dashboard, the
pseudo-swipe navigation model, profiles, Global Search, and Capture
Timeline are R0N1N's own contribution, implemented as new system services
on top of Furi (see `ARCHITECTURE.md`, "R0N1N layer" section).

## Readiness criterion for a UX decision

Every new screen or interaction is checked against the list in
`VISION.md` (path to the goal, adapts to profile, honest about the
capability's source) before it's greenlit for implementation.
