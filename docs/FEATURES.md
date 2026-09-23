# Core feature set

Inherited from the Unleashed base, with a UX layer reworked in the style of
Momentum (see `FIRMWARE_LANDSCAPE.md`). This describes what should work
"out of the box" — excluding R0N1N's unique differentiators (see
`UNIQUE_FEATURES.md`) and the security toolkit (see `SECURITY_TOOLKIT.md`).

## Sub-GHz

Receive/save/replay signals, a frequency and spectrum analyzer (within
what CC1101 allows — see the "not an SDR" constraint in `HARDWARE.md`),
fixed-code brute force (lab use only, see `SECURITY_TOOLKIT.md`), the
extended range and protocols from Unleashed, Subdriving (tagging a signal
with GPS coordinates), signal playlists. External CC1101 is a first-class
module, not a second-class citizen.

## NFC (13.56 MHz)

Read/save/emulate, dictionary attacks, mfkey32/nested/card-only attacks on
MIFARE Classic, MIFARE Plus SL3 (AES) from Unleashed, NFC Magic, NFC Maker
(NDEF/vcard).

## 125 kHz RFID

Read/write/emulate (EM4100/HID Prox/Indala/Cyfral), RFID Fuzzer (lab use).

## Infrared

A universal remote built on the IRDB, learning new commands, "turn
everything off."

## iButton / 1-Wire

Read/write/emulate.

## BadUSB / BadKB

DuckyScript plus extensions, USB and BLE HID, VID/PID/name/MAC spoofing, an
editor/runner with templates, JS-BadUSB (conditionals, loops, GUI, export
to a virtual disk image).

## U2F/FIDO + TOTP

Native U2F over USB (from OFW/Unleashed); extending this to FIDO2/passkey
and TOTP/HOTP is a separate evaluation once its turn comes up in the
roadmap (compatible community libraries need to be checked before this
gets promised in the roadmap).

## GPIO / Dev

USB-UART/SPI/I2C bridge, SWD/JTAG debugging (DAP Link), an I2C scanner,
sensors over GPIO/I2C/1-Wire, a basic logic analyzer, GPS NMEA.

## System capabilities (UX layer)

Control Center, a customizable Home/Desktop, Asset Packs, the keybind
system, an advanced file manager with virtual disk-image mounting, the JS
engine as a foundation for scripting (see `UNIQUE_FEATURES.md`).

## Explicitly outside the core feature set (see `HARDWARE.md`)

Wi-Fi attacks, wardriving, nRF24 tooling, video-out, AI — none of these are
part of the core, since they require external hardware or a companion.
They're described separately as modular/companion features so the base
firmware never sets false expectations.
