# Contributing to R0N1N

## Current stage of the project

R0N1N is currently in a **conceptual/pre-development stage**: the whole
repository is documentation under `docs/`, describing the vision, the
technical foundation, and the development plan. Forking the firmware,
building it, and writing code haven't started yet (see `docs/ROADMAP.md`,
"Current status").

That means the most useful contributions right now are:

- reviewing and refining the documents in `docs/` — factual inaccuracies,
  stale references to upstream releases, unrealistic timeline estimates;
- discussing architectural decisions before they're locked into code;
- expanding `docs/HARDWARE.md` and `docs/ECOSYSTEM.md` with verified facts
  (cited to a source) wherever something is flagged as "needs
  confirmation."

Pull requests with firmware/app code won't be merged before Stage 0 (see
`docs/ROADMAP.md`) is complete — the forked base needs to exist first.

## How to propose documentation changes

1. One PR per document, or a tightly related group of documents. Don't mix
   edits to `docs/HARDWARE.md` with edits to `docs/ROADMAP.md` in the same
   PR unless one directly follows from the other.
2. Any factual claim about Flipper Zero hardware or another firmware
   (versions, specs, licenses) should cite a source in the PR description.
3. Use "this proposes X" rather than "we decided X" phrasing — reserve the
   latter for things that are genuinely already settled in existing
   documents.

## White-hat / legality

Any contribution that extends `docs/SECURITY_TOOLKIT.md` or future
security functionality must stay within the boundaries described in
`docs/VISION.md` ("Project boundaries") — the user's own devices, labs,
CTF, and authorized penetration testing only. Proposals for DoS tooling,
mass targeting, evading detection in genuinely malicious scenarios, or
supply-chain compromise are declined without discussion.

## License

The project is distributed under GPL-3.0 (see `LICENSE`) — like Unleashed,
Momentum, and RogueMaster, whose codebase will become R0N1N's foundation.
Any code added after development begins must be compatible with this
license.
