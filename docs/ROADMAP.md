# Roadmap

## Current status

**None of the below has started.** Right now this repository contains only
concept documentation (`docs/`). Stage 0 (forking the base) is the next
step, but it requires a separate decision to start — it isn't implied
automatically just because this documentation exists.

Stage-duration estimates are rough, carried over from the original
research without independent verification; real planning should
recompute them after Stage 0, once the actual pace of work on a concrete
codebase is known.

## Stage 0 — Fork & Foundation

Goal: assemble R0N1N as a fork of Unleashed, set up `ufbt`/CI, release
channels, branding, a reproducible build.
Dependencies: the current `HEAD` of `DarkFlippers/unleashed-firmware` (see
`FIRMWARE_LANDSCAPE.md`), the toolchain.
Risk: drifting from upstream — mitigated by regular rebasing.

**Progress:** Unleashed's `dev` branch is merged into `firmware/` via
`git subtree` (full history preserved), all 13 submodules resolve and
check out cleanly, and a stock `f7-firmware-D` build succeeds end to end
(toolchain fetch → compile → link → `.bin`/`.dfu`) — see `HARDWARE.md` for
the measured flash numbers this produced. Still open: a feasibility check
for a legible Cyrillic bitmap font within that measured flash budget (see
`HARDWARE.md`, `VISION.md` — "Localization as a differentiator"), branding
placeholders, release channels, and CI.
Outcome: an R0N1N build == Unleashed + branding, installable via
web/qFlipper.

## Stage 1 — MVP: Home + navigation + Control Center

Goal: the new Desktop dashboard (time/date/battery/indicators),
pseudo-swipes, Control Center (ported from Momentum), basic
favorites/recent.
Dependencies: GUI/`ViewDispatcher`, notification (see `ARCHITECTURE.md`).
Risk: Desktop RAM/performance — mitigated by profiling and lazy `View`s.
Outcome: the device feels like a cohesive shell rather than an app list —
a demonstrable MVP prototype.

## Stage 2 — Profiles + Global Search + Capture Timeline

Goal: Profile Manager (Everyday/Pentest/Dev/CTF), global search (index on
SD), a unified capture feed.
Dependencies: storage, hooks into core apps' capture-save paths.
Risk: the search index and memory — mitigated by keeping it on SD, not RAM.
Outcome: R0N1N's key UX differentiator (see `UNIQUE_FEATURES.md`, item 1)
is working.

## Stage 3 — R0N1N Hub + app compatibility

Goal: the on-device catalog with an API-compatibility filter, an official
R0N1N target in `ufbt` + GitHub Actions, microSD layout and migrations
(see `ECOSYSTEM.md`).
Dependencies: a stabilized `api_symbols.csv`, CI.
Risk: "API mismatch" — mitigated by versioning and a build matrix.
Outcome: apps install and update without friction.

## Stage 4 — Companion v1

Goal: the PC/mobile companion — capture sync, catalog/install, backup,
remote screen — built on RPC (see `COMPANION.md`).
Dependencies: RPC, a stable Capture Timeline format.
Risk: cross-platform reach/BLE stability.
Outcome: an ecosystem, not just firmware.

## Stage 5 — Workflow/JS engine + first-class external modules

Goal: JS "recipes," auto-detection and onboarding for ESP32/nRF24/
CC1101/VGM, Sweep mode (see `UNIQUE_FEATURES.md`, items 4–5).
Dependencies: JS modules (ported from Momentum), module drivers.
Risk: JS-runner memory footprint, inconsistent pinouts across modules.
Outcome: automation and modularity become first-class citizens.

## Stage 6 — AI bridge + educational/CTF layer + polish

Goal: the optional AI bridge in the companion (approve-per-action, offline
mode), built-in labs/CTF mode, a stability audit, integration tests,
documentation.
Dependencies: the companion, RPC.
Risk: AI-action safety/ethics — mitigated by risk tiers, logging,
off-by-default behavior.
Outcome: R0N1N 1.0.

## MVP vs. the full release

**MVP (Stages 0–2):** a fork of Unleashed + the Home dashboard,
pseudo-swipes, Control Center, profiles, Global Search, Capture Timeline.
Functionally the MVP equals Unleashed (compatibility and power are already
there); the added value is the UX layer on top.

**Full release / R0N1N 1.0 (Stages 3–6):** adds the R0N1N Hub with app
compatibility solved, the companion ecosystem, the workflow/JS engine,
first-class external modules, the optional AI bridge, and the
educational/CTF layer.

## Main project risks

1. **Memory/performance** — the R0N1N layer must not make free heap worse
   than stock Unleashed (verified starting at Stage 1, see
   `ARCHITECTURE.md`).
2. **Upstream drift** — disciplined, regular rebasing onto Unleashed.
3. **App fragmentation** — API versioning and the CI target (Stage 3).
4. **Staying within white-hat boundaries** — TX-lock as an option,
   confirmation for "sharp" operations, AI actions off-by-default (see
   `SECURITY_TOOLKIT.md`, `VISION.md`).

## How to read this roadmap

Each stage is a direction and a dependency order, not a rigid schedule.
Before starting any stage, re-check `HARDWARE.md` (open questions about the
actual memory budget) and `FIRMWARE_LANDSCAPE.md` (how current upstream is
at the time work starts).
