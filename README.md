# R0N1N

**R0N1N** — концепция кастомной прошивки Flipper Zero как UX-first
операционной оболочки, а не очередного «форка с кучей приложений».

> **Статус: концептуальная стадия.** Разработка (форк, сборка, код) ещё
> не начата. Этот репозиторий сейчас содержит только проработанную
> документацию — vision, техническое обоснование и план. См.
> [`docs/ROADMAP.md`](docs/ROADMAP.md), раздел «Статус на данный момент».

## Идея в двух словах

Форки Flipper Zero (Official, Unleashed, Momentum, RogueMaster) к 2026 году
функционально сошлись — они различаются стабильностью и полировкой, а не
набором возможностей. R0N1N исходит из того, что незанятый дефицит рынка —
не функции, а **связность и удобство**: единый Home-дашборд вместо списка
приложений, предсказуемая навигация «псевдо-свайпами» на D-pad, сквозные
профили режимов (Everyday / Pentest / Dev / CTF), единая лента захватов и
глобальный поиск. Подробно — [`docs/VISION.md`](docs/VISION.md).

Технически проект строится как форк стабильной базы Unleashed с UX-слоем,
портированным и переосмысленным из Momentum — не переписывание ядра с нуля.
Всё, что физически не помещается в STM32WB55 (Wi-Fi-атаки, AI, video-out,
SDR), честно вынесено на уровень опциональных внешних модулей или
companion-приложения, а не обещано как «встроенное».

## Документация

| Документ | О чём |
|---|---|
| [`docs/VISION.md`](docs/VISION.md) | Видение проекта, три принципа, границы (white-hat/легальность) |
| [`docs/HARDWARE.md`](docs/HARDWARE.md) | Возможности и жёсткие ограничения железа Flipper Zero |
| [`docs/FIRMWARE_LANDSCAPE.md`](docs/FIRMWARE_LANDSCAPE.md) | Анализ существующих прошивок и стратегия выбора базы |
| [`docs/UX_DESIGN.md`](docs/UX_DESIGN.md) | Home-дашборд, навигация «псевдо-свайпы», профили, сквозные сервисы |
| [`docs/FEATURES.md`](docs/FEATURES.md) | Базовый набор возможностей, наследуемый от Unleashed/Momentum |
| [`docs/UNIQUE_FEATURES.md`](docs/UNIQUE_FEATURES.md) | Что отличает R0N1N от остальных форков |
| [`docs/SECURITY_TOOLKIT.md`](docs/SECURITY_TOOLKIT.md) | White-hat security-инструментарий и его рамки применения |
| [`docs/ECOSYSTEM.md`](docs/ECOSYSTEM.md) | Экосистема приложений и решение проблемы «API mismatch» |
| [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) | Слоистая архитектура прошивки, управление памятью |
| [`docs/COMPANION.md`](docs/COMPANION.md) | PC/mobile companion и опциональный AI-мост |
| [`docs/ROADMAP.md`](docs/ROADMAP.md) | Этапы разработки, MVP vs. полная версия, риски |

## Легальность

R0N1N — легальный open-source проект кастомизации прошивки коммерчески
доступного устройства, в духе Unleashed/Momentum/RogueMaster. Все
security-функции предназначены только для собственных устройств,
лабораторных стендов, CTF и авторизованного пентеста. Подробнее —
[`docs/VISION.md`](docs/VISION.md) («Границы проекта») и
[`docs/SECURITY_TOOLKIT.md`](docs/SECURITY_TOOLKIT.md).

## Участие

См. [`CONTRIBUTING.md`](CONTRIBUTING.md) — на текущей стадии полезнее всего
ревью и уточнение документации, а не код.

## Лицензия

[GPL-3.0](LICENSE) — как и прошивки, на которых основан R0N1N.
