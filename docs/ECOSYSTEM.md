# App ecosystem

## GitHub ecosystem — context

The Flipper ecosystem is huge and fragmented across catalogs: the official
Flipper Application Catalog (`flipperdevices/flipper-application-catalog`,
surfaced through Lab.Flipper.net), fork-specific satellite catalogs
(`Momentum-Apps`, RogueMaster's app collection), independent platforms, and
community-maintained awesome-lists. Project classes relevant to R0N1N:

- **Sub-GHz:** fixed-code brute-forcers, spectrum analyzers, POCSAG/TPMS/
  weather decoders, .sub editors.
- **NFC/RFID:** MIFARE attacks, NFC Magic, RFID Fuzzer, NFC Maker.
- **IR:** IRDB extensions, remote converters.
- **GPIO/hardware:** ESP flashers, UART/SPI/I2C bridges, DAP Link/SWD.
- **USB/BadUSB:** DuckyScript playbook libraries, JS-BadUSB.
- **Bluetooth/2.4 GHz:** BLE tools, nRF24 tools — modular.
- **Dev tools:** `ufbt` (fast .fap builds), GitHub Actions for CI-building
  firmwares and apps, SDKs for several languages.

This is a map of what's possible, not a list of things R0N1N is obligated
to pull into core — the overwhelming majority stays as optional .fap apps
on SD (see `ARCHITECTURE.md`).

## The community's main pain point: "API mismatch"

A .fap was built against a different version/fork — the major API version
doesn't match — the app fails to load. Cause: every fork has its own
`api_symbols.csv` and API version; the App Loader checks the major version
before launching. This isn't a hypothetical problem — it surfaces reliably
the moment a user tries to move an app between forks.

## R0N1N's strategy: a systemic fix, not patches

1. **R0N1N Hub (on-device catalog)** — search, categories, favorites, an
   "installed/update available" status, and an **API-compatibility
   filter**: by default show only .fap apps compatible with the current
   R0N1N API version, and warn explicitly about incompatible ones instead
   of failing silently.
2. **Companion catalog sync** — the PC/mobile companion picks the correct
   .fap build for the installed R0N1N version (similar to Flipper
   Lab/the mobile app, but targeting a custom API) — transport details in
   `COMPANION.md`.
3. **CI build pipeline** — an official R0N1N target in `ufbt` + GitHub
   Actions, so building for R0N1N is one command, not a manual patch to
   someone else's `.fap`.
4. **A stable API contract** — follow `api_symbols.csv` version semantics
   (major = breaking change), minimize breaking changes between releases,
   publish a compatibility table.
5. **A predictable microSD layout** — clear folders (`apps/Sub-GHz`,
   `apps/NFC`, `apps/GPIO`, `apps/Scripts`, `asset_packs`, `update`) with
   automatic migration of user files on version upgrades (following how
   Momentum migrates files when an SD card is inserted).

## What belongs in the MVP vs. later

The compatibility filter in the Hub and a predictable SD layout are part of
the MVP (Stage 2–3 in `ROADMAP.md`), since without them any app ecosystem
built on top of R0N1N would just reproduce the same pain we're trying to
solve. The CI target and companion catalog sync are Stage 3–4, once the
R0N1N layer's own API has stabilized.
