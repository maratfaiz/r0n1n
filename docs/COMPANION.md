# Companion ecosystem

## Why a companion is needed at all

A companion removes three specific pain points that a single firmware
can't solve on its own (constraints detailed in `HARDWARE.md`):

1. A tiny screen (128×64 monochrome) with input limited to a D-pad.
2. Manually moving and picking files/apps between the device and the SD
   card.
3. Fragmented app catalogs and "API mismatch" (see `ECOSYSTEM.md`).

## R0N1N Companion (PC: Windows/macOS/Linux; Mobile: Android/iOS)

- **Files and captures.** Two-way sync of the Capture Timeline (see
  `UX_DESIGN.md`), viewing/annotating `.sub`/`.nfc`/`.ir` files on a full
  screen, exporting to third-party tool formats (e.g. Wireshark) wherever
  the data format allows it.
- **App catalog.** Picking the correct `.fap` build for the installed
  R0N1N version, batch install/update over USB/BLE — the implementation of
  the strategy in `ECOSYSTEM.md`.
- **Firmware/backup.** Installing/rolling back R0N1N, a full backup of the
  device configuration.
- **Remote screen.** Mirroring the screen and controlling the buttons —
  useful for demos, teaching, and field work where staring at the real
  128×64 screen is impractical.
- **Workflow editor.** Visually building JS "recipes" (see
  `UNIQUE_FEATURES.md`, workflow engine), pushing them to the device.
- **AI bridge (optional, off by default).** Natural-language control and
  "explain this signal/card/script." The core requirement is
  approve-per-action behavior, risk tiers for actions, and a full log; a
  local-model option supports offline use. Full security requirements are
  in `UNIQUE_FEATURES.md`, item 2.

## Architectural principle: companion is a "thick client," Flipper is
"thin"

No feature critical to the device's base, standalone operation gets pushed
onto the companion. The companion amplifies capability — it's never a
crutch the firmware can't run without. This directly follows from R0N1N
remaining firmware for the Flipper Zero, not an app that merely treats the
Flipper as a peripheral.

## Transport

RPC over USB (fast, handles large transfers) and BLE (mobile/field
scenarios) — reusing the RPC service that already exists in the firmware
(see `ARCHITECTURE.md`, "Services" layer), not a new protocol built from
scratch.

## What's still unresolved and needs a prototype

- The concrete companion app stack (native per platform vs. a
  cross-platform framework) — decided at Roadmap Stage 4, once the Capture
  Timeline format and the R0N1N layer's RPC protocol have stabilized, not
  ahead of time.
- The storage/sync format for the Capture Timeline between device and
  companion — part of Stage 2 (the service itself) and Stage 4 (syncing
  it).
- Which specific LLM infrastructure powers the AI bridge — an
  implementation question for Stage 6; it would be premature to lock in a
  particular provider or third-party project as a dependency here.
