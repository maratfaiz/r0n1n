# Основной набор возможностей (core feature set)

Наследуется от базы Unleashed, с UX-слоем, переработанным по образцу
Momentum (см. `FIRMWARE_LANDSCAPE.md`). Это описание того, что должно
работать «из коробки» — без учёта уникальных для R0N1N функций
(см. `UNIQUE_FEATURES.md`) и security-инструментария
(см. `SECURITY_TOOLKIT.md`).

## Sub-GHz

Приём/сохранение/повтор сигналов, частотный и спектр-анализатор (в рамках
возможностей CC1101 — см. ограничение «не SDR» в `HARDWARE.md`), брутфорс
fixed-code протоколов (только лабораторно, см. `SECURITY_TOOLKIT.md`),
расширенный диапазон и протоколы из Unleashed, Subdriving (привязка
GPS-координат к сигналу), плейлисты сигналов. Внешний CC1101 —
first-class модуль, а не «второй сорт».

## NFC (13.56 МГц)

Чтение/сохранение/эмуляция, словарные атаки, mfkey32/nested/card-only на
MIFARE Classic, MIFARE Plus SL3 (AES) из Unleashed, NFC Magic, NFC Maker
(NDEF/vcard).

## RFID 125 кГц

Чтение/запись/эмуляция (EM4100/HID Prox/Indala/Cyfral), RFID Fuzzer
(лабораторно).

## Инфракрасный порт

Универсальный пульт на базе IRDB, обучение новым командам, «выключить всё».

## iButton / 1-Wire

Чтение/запись/эмуляция.

## BadUSB / BadKB

DuckyScript + расширения, USB и BLE HID, подмена VID/PID/имени/MAC,
редактор/раннер с шаблонами, JS-BadUSB (условия, циклы, GUI, экспорт в
виртуальный disk-образ).

## U2F/FIDO + TOTP

Нативный U2F по USB (из OFW/Unleashed); дальнейшее расширение до
FIDO2/passkey и TOTP/HOTP — предмет отдельной оценки на Этапе, когда до неё
дойдёт очередь в roadmap (нужна проверка совместимых community-библиотек
перед обещанием в roadmap).

## GPIO / Dev

USB-UART/SPI/I2C-мост, SWD/JTAG-отладка (DAP Link), i2c-сканер, датчики
через GPIO/I2C/1-Wire, базовый логический анализатор, GPS NMEA.

## Системные возможности (UX-слой)

Control Center, кастомизируемый Home/Desktop, Asset Packs, keybind-система,
продвинутый файловый менеджер с виртуальным монтированием disk-образов,
JS-движок как основа для сценариев (см. `UNIQUE_FEATURES.md`).

## Явно вне core feature set (see `HARDWARE.md`)

Wi-Fi-атаки, wardriving, nRF24-инструменты, video-out, AI — не входят в
core, так как требуют внешнего железа или companion. Они описаны отдельно
как модульные/companion-функции, чтобы не создавать ложных ожиданий от
базовой прошивки.
