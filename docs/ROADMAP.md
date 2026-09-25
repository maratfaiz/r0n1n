# Roadmap

## Current status

**Stages 0 and 1 are done** — see their sections below for exactly what
shipped and what was simplified. `firmware/` is the official Flipper Zero
firmware, release 1.4.3 (`git subtree`, full history), with R0N1N's Home
dashboard, Control Center, Quick Actions, and Recent wired in and verified
against actual `./fbt` builds, not just planned. Stage 2 (Profiles, Global Search,
Capture Timeline) is next and hasn't started.

Stage-duration estimates are rough, carried over from the original
research without independent verification; real planning should
recompute them now that Stages 0–1 give an actual sense of pace on this
codebase.

## Stage 0 — Fork & Foundation — done

Goal: assemble R0N1N as a fork of the official Flipper Zero firmware, set
up `ufbt`/CI, release channels, branding, a reproducible build.
Dependencies: an official release tag of
`flipperdevices/flipperzero-firmware` (see `FIRMWARE_LANDSCAPE.md`), the
toolchain.
Risk: drifting from upstream — mitigated by regular syncing with official
releases.

**Progress:** official release `1.4.3` is merged into `firmware/` via
`git subtree` (full history preserved), all 12 submodules resolve and
check out cleanly, and a stock `f7-firmware-D` build succeeds end to end
(toolchain fetch → compile → link → `.bin`/`.dfu`) — see `HARDWARE.md` for
the measured flash numbers this produced. `FIRMWARE_ORIGIN` is `R0N1N`
(reported in version info) and About opens with an R0N1N screen crediting
the official firmware. CI (`.github/workflows/build-firmware.yml`) builds
on push/PR. Cyrillic font feasibility is resolved — see `HARDWARE.md`,
it's cheap (~1-5 KB against ~245 KB free), using fonts already vendored in
`lib/u8g2`, no sourcing/hand-drawing needed. Still open: release
channels/versioning scheme, and an R0N1N boot-splash asset (the stock
animations are brand-neutral).
Outcome: an R0N1N build == official firmware + R0N1N UX layer, installable
via qFlipper ("Install from file") or an SD-card update package.

## Stage 1 — MVP: Home + navigation + Control Center — done

Goal: the new Desktop dashboard (time/date/battery/indicators),
pseudo-swipes, Control Center, basic favorites/recent.
Dependencies: GUI/`ViewDispatcher`, notification (see `ARCHITECTURE.md`).
Risk: Desktop RAM/performance — mitigated by profiling and lazy `View`s.
Outcome: the device feels like a cohesive shell rather than an app list —
a demonstrable MVP prototype.

**Shipped**, all inside `firmware/applications/services/desktop/` unless
noted, each verified with a real `./fbt` build:

- **Home dashboard**: `desktop_view_main.c` gained a big clock, date, and
  a profile-name label, drawn via `locale_format_time`/`locale_format_date`
  (already in the codebase — respects the existing 12h/24h and date-format
  settings) on a 1 Hz timer that only runs while Home is on screen. It's a
  *separate* draw-only `View` layered above the dolphin animation in
  `main_view_stack`, not the same `View` main_view's input handling uses —
  `ViewStack` ties draw order and input priority to the same array (reverse
  order, first-consumer-wins), and main_view's input callback always
  returns `true`, so simply reordering it would have made it swallow input
  before the dolphin's own view-level "poke" interaction (right button
  short) ever saw it. See the `dashboard_view` comment in
  `desktop_view_main.c`.
- **Control Center (Down)**: no new screen — the official firmware's
  lock menu (Lock, dummy mode) is reused as-is, just reached from Down
  instead of Up. Quick toggles (BT, sound, brightness/volume) and the
  USB-mode and TX-lock toggles from the original `UX_DESIGN.md` list
  aren't wired in yet — the official lock menu has no such pages, and
  they need their own design pass rather than a stub.
- **Quick Actions (Up)**, `desktop_scene_favorites.c` (new): lists the four
  existing `FavoriteApp` slots (previously each reachable only by its own
  D-pad shortcut) as one list, launches on selection, plus a fixed
  **Archive** entry: in the official firmware Archive isn't a Loader app
  and was reachable only from Down-short on Home, which is now Control
  Center — without this entry it would be unreachable (the linker even
  dropped it from the image). Not the customizable 3×2 grid `UX_DESIGN.md`
  describes — it's the existing favorite slots in list form, which is what
  made this buildable without inventing a second, parallel favorites
  store.
- **Recent (hold OK)**, `desktop_scene_recent.c` (new): shows apps launched
  since boot, most-recent first, in-memory only (nothing persisted across
  reboot). Needed one small addition outside `desktop/`: `LoaderEvent`
  (`firmware/applications/services/loader/loader.h`) didn't carry the
  app's name, so `LoaderEventTypeApplicationBeforeLoad` subscribers had no
  way to know what was about to launch — confirmed by reading
  `loader_do_start_by_name`, not assumed. Added a `name` field, populated
  only at that one call site and only when the Loader isn't already
  running an app (the official Loader publishes the event before its lock
  check; the other three publish sites set it `NULL`); purely additive, every other subscriber (power, archive,
  loader_applications) only reads `.type` and is unaffected. An app is
  added to the list only once the Loader reports it stopped
  (`LoaderEventTypeApplicationStopped`), so failed launches (app not found,
  bad `.fap`) never appear; relaunching an app already listed moves it to
  the front instead of duplicating it. Entries hold the full `.fap` path
  (same 128-byte limit as the favorite slots) but the list shows just the
  file name; Quick Actions labels its slots the same way.
- **Navigation remap**: Up opens Quick Actions (was the lock menu), Down
  opens Control Center (was Archive), hold-OK opens Recent (unused on
  Home before). Archive moved into Quick Actions, as above.

**Deliberately not done** (see `UX_DESIGN.md` for the full model these are
part of):
- **Left/Right "desktop" paging** — still direct favorite-app shortcuts,
  unchanged from the official firmware. Real per-profile desktop sets need Stage 2's
  Profile Manager to define what the panes even are; building paging with
  no real content behind it now would've meant deleting a working shortcut
  for a stub.
- **Global Search (hold Back)** — explicitly Stage 2 scope, not touched.
- **Profile-aware anything** — the dashboard's profile label is a fixed
  `"Everyday"` string (`DASHBOARD_DEFAULT_PROFILE_NAME` in `desktop_i.h`);
  there is no Profile Manager to read from yet.
- **Recent files** — `UX_DESIGN.md` has hold-OK list "last-used
  apps/files"; Recent lists apps only, and relaunches them *without* the
  arguments they were first started with. Replaying args blindly isn't
  safe: they can be stale file paths or an RPC session marker from
  qFlipper/mobile, and an app opened on a file from Archive would silently
  reopen that file. Recent files belong with Stage 2's Capture Timeline,
  which will know which files are captures worth reopening.

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
Dependencies: the official JS engine and its modules, module drivers.
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

**MVP (Stages 0–2):** a fork of the official firmware + the Home dashboard,
pseudo-swipes, Control Center, profiles, Global Search, Capture Timeline.
Functionally the MVP equals the official firmware (stability and app
compatibility are already there); the added value is the UX layer on top.

**Full release / R0N1N 1.0 (Stages 3–6):** adds the R0N1N Hub with app
compatibility solved, the companion ecosystem, the workflow/JS engine,
first-class external modules, the optional AI bridge, and the
educational/CTF layer.

## Main project risks

1. **Memory/performance** — the R0N1N layer must not make free heap worse
   than the stock official firmware (verified starting at Stage 1, see
   `ARCHITECTURE.md`).
2. **Upstream drift** — disciplined, regular syncing with official releases.
3. **App fragmentation** — API versioning and the CI target (Stage 3).
4. **Staying within white-hat boundaries** — TX-lock as an option,
   confirmation for "sharp" operations, AI actions off-by-default (see
   `SECURITY_TOOLKIT.md`, `VISION.md`).

## How to read this roadmap

Each stage is a direction and a dependency order, not a rigid schedule.
Before starting any stage, re-check `HARDWARE.md` (open questions about the
actual memory budget) and `FIRMWARE_LANDSCAPE.md` (how current upstream is
at the time work starts).
