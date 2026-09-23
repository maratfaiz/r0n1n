# Firmware architecture

## Base decision

A fork of **Unleashed** (see `FIRMWARE_LANDSCAPE.md` for why this base was
chosen); UX components are ported and reworked from **Momentum**. We're not
writing an OS from scratch — that would break compatibility with the app
catalog and throw away years of community work on radio-stack stability,
drivers, and the HAL.

## Layers (inherited from the official firmware's model)

1. **Hardware / `furi_hal`** — nothing below the HAL is touched; we work
   only through the public `furi_hal`, never directly against the STM32
   HAL — a precondition for cross-compatibility with upstream and future
   Unleashed updates.
2. **FreeRTOS + Furi (Furi OS)** — the scheduler, threads, records
   (`record_open`/`record_close`), services. Not modified.
3. **Services** — GUI/`ViewDispatcher`, storage (LittleFS + SD), input,
   notification, RPC (control over USB/BLE — the companion transport, see
   `COMPANION.md`), BLE.
4. **Core apps** — Sub-GHz, NFC, RFID, IR, GPIO, iButton, BadUSB, U2F —
   inherited from Unleashed with almost no logic changes, only integration
   points with the new UX layer (see below).
5. **R0N1N layer** — the project's main technical contribution,
   implemented mostly as system apps/services on top of Furi so the
   monolith doesn't grow:
   - the new **Desktop/Home** (dashboard + pseudo-swipes),
   - the **navigation manager** (desktops),
   - **Profile Manager**,
   - **Global Search** (index on SD),
   - the **Capture Timeline** service (hooks into core apps' save paths),
   - **R0N1N Hub** (catalog with a compatibility filter),
   - the **Workflow/JS runner** (built on mJS).
6. **FAP apps on SD** — all further extensibility, without growing the
   monolith (see `ECOSYSTEM.md`).

## Memory management (critical — see `HARDWARE.md`)

Real limits need to be respected, not designed around "future hardware
that might exist":
- One user app active at a time.
- Frugal `View`s, no heavy static buffers in new services.
- Profile with the firmware's own `top`/`free` CLI on every significant
  change to the R0N1N layer, not only "at the end."
- Minimal `stack_size` for new threads, so they don't waste heap.
- R0N1N services must be "lazy": data (the search index, Capture Timeline)
  lives on SD, with only the current screen's working set in RAM.
- **Design goal:** the R0N1N layer must not make free heap worse than
  stock Unleashed on the same API version — measurable, verified during
  the prototype stage (`ROADMAP.md`, Stage 1).

## Firmware updates

The OFW/Unleashed mechanism is kept as-is: the update package is written to
SD at `/ext/update/`, applied offline on reboot by a small bootloader,
without touching user data; DFU via qFlipper serves as emergency recovery.
Signed releases, a public changelog.

## Stability and compatibility

- Mandatory integration tests before every release (mirroring current
  upstream OFW practice).
- Regional TX-lock as a configurable option (see `SECURITY_TOOLKIT.md`).
- "Sharp" security features gated behind explicit UI confirmation.
- Separate dev/release build channels.
- Compatibility with the Flipper ecosystem through the public Furi API and
  `api_symbols.csv` versioning (see `ECOSYSTEM.md`).

## Companion channel

RPC over USB/BLE (already present in the firmware as a service) — the only
transport for the companion and the AI bridge. The device stays a thin
client; heavy computation stays on the companion (see `COMPANION.md`).
This is an architectural principle, not an implementation detail: no
feature critical to the device's base, standalone operation may depend on
the companion.

## Open question

The exact boundary between "R0N1N layer as a system service" and "R0N1N
layer as a FAP app on top of the standard SDK" needs prototyping in
Roadmap Stage 1: some services (Global Search, Capture Timeline) may turn
out to be simpler and safer for upstream compatibility as privileged FAPs
rather than core patches. That decision follows from profiling, not from
deciding it up front.
