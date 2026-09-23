# Landscape of existing firmwares and R0N1N's base strategy

## Method

Below is a reconciliation of two independent research passes (our detailed
breakdown plus a supporting pass from another model), cross-checked against
GitHub repositories (September 2026). Version numbers and release dates
didn't match between the two sources and in places resembled hallucinated
specifics — so only verified facts are listed here (repositories, active
development status, general architectural differences), not invented build
numbers. Before starting Stage 0 (fork), re-check the current `HEAD` of
each repository — links below.

## Official Firmware (OFW)

- Repository: `flipperdevices/flipperzero-firmware`.
- FreeRTOS + Furi, layered architecture (HAL → services → applications).
- Maximum stability, signed OTA updates, official App Catalog.
- Regional TX restrictions on by default, doesn't store captured rolling
  codes, minimal UI customization.
- As of 2026, Flipper Devices has resumed active support for external
  contributions with stricter review (including specific attention to
  AI-generated code touching low-level libraries) — worth factoring into
  R0N1N's upstream-compatibility planning, though it doesn't block forking
  from Unleashed.

## Unleashed (DarkFlippers) — recommended base for R0N1N

- Repository: `DarkFlippers/unleashed-firmware`, actively maintained
  (releases were still landing through September 2026).
- "Official but unlocked": extended Sub-GHz range, regional restrictions
  removed, additional protocols, saving and replaying captured signals,
  external CC1101 support over SPI, security options (Lock on Boot, reset
  on wrong PIN).
- The most widely used stable base, and the foundation for both Momentum
  and RogueMaster — i.e. maximum compatibility with community apps from
  both directions.
- Weak spot — UI stays close to stock, no deep interface rework.

**Conclusion:** Unleashed remains the right choice for the fork base:
stability + unlock + broad API compatibility, while its UI layer is barely
reworked — meaning it won't fight the R0N1N UX layer.

## Momentum (Next-Flip) — source of UX ideas

- Repository: `Next-Flip/Momentum-Firmware` (plus the `Momentum-Apps` and
  `Asset-Packs` satellite repos); a direct continuation of Xtreme
  Firmware, built by the same team (Xtreme officially stopped development
  in late 2024, and its developers moved to Momentum).
- The most polished UX among the forks: multiple home-menu styles, a
  Control Center with quick toggles, an advanced file manager, Asset
  Packs, a keybind system (button remapping, press/hold), JS modules
  (Storage, GUI, BLE, SubGHz, USB Disk), Bad-KB (USB+BLE), FindMy, BLE
  Spam, GPS Subdriving.
- **For R0N1N, Momentum is the main source of UX ideas and the best
  technical reference for the interface**, but not the fork base: we port
  and rethink specific components (Control Center, file manager, keybinds,
  JS modules) on top of Unleashed rather than forking Momentum wholesale —
  this keeps a narrow, predictable delta from upstream.

## RogueMaster (RogueMaster/The-Flipper-Files) — catalog map, not a base

- Repository: `RogueMaster/flipperzero-firmware-wPlugins`, based on
  Unleashed.
- "Kitchen sink": maximum apps, games, plugins, animations; releases ship
  frequently (weekly / with each OFW update).
- Upside — breadth of community app coverage; downside — the least
  predictable stability and the "heaviest" build; the project's own notes
  about needing to clear `/ext/apps` before updating are a sign of fragile
  migrations.
- **Role for R0N1N:** a source of catalog candidates (see `ECOSYSTEM.md`),
  filtered hard for stability and memory footprint — not forked directly.

## Xtreme Firmware — historical note

- Was once the stability/feature-completeness flagship among the forks;
  officially discontinued (Flipper-XFW) in late 2024.
- The team and its work moved into Momentum — meaning every current Xtreme
  idea worth having is already available through Momentum; there's no need
  to treat Xtreme as a separate source for R0N1N.

## Synthesis: R0N1N's base strategy

1. **Fork Unleashed** — the foundation (Stage 0): API/app compatibility,
   the unlock, external-module support, active upstream maintenance.
2. **Port the UX layer from Momentum**, reworked into R0N1N's own
   information architecture (Home dashboard, pseudo-swipes, profiles — see
   `UX_DESIGN.md`), not a blind copy of Momentum's menus.
3. **RogueMaster and other community forks are a source of catalog
   candidates** (`ECOSYSTEM.md`), not code to fork.
4. **Disciplined, continuous rebasing onto Unleashed** — the main risk of
   this strategy is drifting from upstream (see `ROADMAP.md`, risks
   section).

## Sources to re-verify before Stage 0

- `https://github.com/DarkFlippers/unleashed-firmware`
- `https://github.com/Next-Flip/Momentum-Firmware`
- `https://github.com/RogueMaster/flipperzero-firmware-wPlugins`
- `https://github.com/flipperdevices/flipperzero-firmware`
- `https://docs.flipper.net/zero/development/hardware/tech-specs`
