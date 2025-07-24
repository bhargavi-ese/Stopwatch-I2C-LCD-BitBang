# Stopwatch Using I2C LCD and Button with Bit-Banging I2C on STM32F411RE

This project implements a simple stopwatch on the STM32F411RE microcontroller using:
- A 16x2 I2C LCD display (controlled via manual bit-banging I2C)
- A push-button for Start/Stop functionality
- Accurate millisecond counter using `SysTick` interrupt

---

## 🚀 Features

- Stopwatch counting up to 99 minutes and 59 seconds
- Start/Stop toggle using a single button
- I2C LCD interface built from scratch (no HAL/LL libraries)
- Manual I2C (bit-banging) protocol implementation using GPIO
- Accurate timing using the STM32 SysTick timer
- Fully developed in **Embedded C** without any third-party libraries

---

## 🔧 Hardware Used

| Component         | Details                                |
|------------------|----------------------------------------|
| Microcontroller   | STM32F411RE (Nucleo-64 Board)          |
| Display           | 16x2 LCD with I2C backpack (PCF8574)   |
| Button            | Tactile switch for Start/Stop          |
| Resistor          | 10kΩ pull-down resistor for button     |
| Breadboard & Wires| Standard prototyping setup             |

---

## 🔌 Pin Connections

| LCD Pin      | STM32F411RE GPIO |
|--------------|------------------|
| SDA          | PB9              |
| SCL          | PB8              |

| Button Pin   | STM32F411RE GPIO |
|--------------|------------------|
| One Button   | PA5              |

---

## 🧠 How It Works

- The stopwatch time is stored in `minutes` and `seconds` variables.
- SysTick fires every 10 ms. A counter accumulates to 1000 ms (1 sec), then seconds increment.
- Pressing the button toggles `start_flag`, enabling/disabling counting.
- Display is updated via I2C LCD every second.

---

## 📁 Project Structure

Project_Folder/
│
├── src/
│ ├── main.c
│ ├── i2c_bitbang.c
│ ├── lcd_i2c.c
│ └── delay.c
│
├── inc/
│ ├── i2c_bitbang.h
│ ├── lcd_i2c.h
│ └── delay.h
│
├── README.md
└── .project, .cproject (for STM32CubeIDE)

---

## 🧠 Concepts Learned

- Bit-banging I2C protocol from scratch
- Writing LCD commands using PCF8574 I/O expander
- Handling GPIO button debounce
- Using `SysTick` timer for precise timing
- Embedded systems project structure & documentation

---

## 🖼️ Demo (Optional)

> If you have a short demo video or image, insert it here.

---

## 📜 License

This project is open-source and available under the [MIT License](LICENSE).

---

## 🙋‍♀️ Author

**Bhargavi Dande**  
Embedded Systems Enthusiast | STM32 Developer  
GitHub: [@bhargavi-ese](https://github.com/bhargavi-ese)  
LinkedIn: [View Profile](https://linkedin.com/in/bhargavi-dande)  
