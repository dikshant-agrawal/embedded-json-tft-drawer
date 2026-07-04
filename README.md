# Embedded JSON Parser + TFT Drawer — PSoC 5LP on ErikaOS

A JSON command parser feeding a TFT drawing engine on a PSoC 5LP microcontroller: send JSON over UART, see shapes drawn on the TFT display.

## What it does
Receives JSON messages (e.g. drawing commands with shapes, coordinates, and colors) over a serial link, parses them on-device with a lightweight tokenizer, and renders the described graphics on a TFT display — demonstrating a full message-processing pipeline (receive → parse → validate → draw) on a resource-constrained target.

## Features
- Lightweight JSON tokenization based on `jsmn` with a custom parser layer (`parser.h`, `json.hts`)
- TFT driver and drawing engine (`drawer.h`, `tft/`)
- Message-queue-based decoupling of reception and rendering (`msg/`)
- Layered architecture: application (`asw/`) over board support (`bsw/`) over RTOS
- Runs on Erika Enterprise (OSEK) with tasks/events

## Tech stack
C, jsmn JSON tokenizer, Erika Enterprise OS, PSoC Creator project for PSoC 5LP (Cortex-M3), TFT display, UART.

## Build & run
1. Open `AMA_EOS_Lab_3.cydsn` in **PSoC Creator 4.x**
2. Build and flash a PSoC 5LP development board with TFT shield
3. Send JSON drawing commands over the UART terminal

## Project structure
- `source/asw/` — application entry
- `source/bsw/` — JSON parser, drawer, TFT driver, message queue, services

