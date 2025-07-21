# 🔧 STM32 FreeRTOS UART Command-Based Task System

This project is a FreeRTOS-based firmware for STM32F4 microcontrollers that implements a menu-driven command interface over UART. The user can navigate menus, control LEDs, and (in future versions) configure the RTC through interactive UART input. The system is designed around multitasking with inter-task communication using FreeRTOS queues and task notifications.

---

## 📁 Project Overview

### ✅ Key Features

- **FreeRTOS Multitasking**  
  - `menu_task`: Displays main menu and routes commands  
  - `led_task`: Displays LED options and handles user input  
  - `rtc_task`: Placeholder for RTC interaction (to be implemented)  
  - `print_task`: Intended to send strings to UART (to be completed)  
  - `cmd_handler_task`: Receives and decodes user commands via UART

- **UART Communication (USART2)**  
  - Non-blocking receive using `HAL_UART_Receive_IT`
  - Characters pushed into a queue (`inputqueue`) for parsing
  - Command processed when newline `\n` is received

- **Queues Used**  
  - `inputqueue`: Stores incoming characters from UART  
  - `printqueue`: Stores strings/messages to be printed (for future use)

- **Task Notification**  
  - Tasks are notified when a complete command is available  
  - Notifications are used to pass command pointer (`cmd_format`) to respective task

---

## 🔌 Hardware & Setup

- **MCU**: STM32F4 series (e.g. STM32F446RE)
- **UART**: USART2 (PA2 - TX, PA3 - RX)
- **LED Pins**: PA5, PA6, PA7, PB6
- **Baud Rate**: `115200`, 8N1

---

## 💻 How It Works

### Main Flow

1. **System Initialization**:
   - Clocks, UART, GPIOs, RTC, and FreeRTOS tasks are initialized in `main.c`
   - UART receive interrupt is enabled

2. **User Input Handling**:
   - Each character received via UART is pushed into `inputqueue`
   - On newline (`\n`), the input is processed and command is sent to appropriate task using `xTaskNotify`

3. **Task Behavior**:
   - `menu_task` presents options like LED or RTC
   - `led_task` presents LED effects menu (LED functionality yet to be implemented)
   - `cmd_handler_task` parses commands and routes them

---

## 🧠 Code Highlights

### `main.c`
- Initializes hardware peripherals and creates tasks
- Starts FreeRTOS scheduler

### `taskhandler.c`
- Implements all task logic
- Uses a state machine (`smainmenu`, `sLEDMenu`, etc.)
- `cmd_handler_task` processes received characters into `cmd_format`

### `stm32f4xx_it.c`
- UART and TIM interrupt handlers call HAL drivers
- `HAL_UART_RxCpltCallback()` queues characters and triggers command processing


