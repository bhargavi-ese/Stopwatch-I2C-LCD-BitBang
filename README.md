
# ⏱️ STM32F411RE Stopwatch using I2C LCD & Bit-Banging I2C with 2 Switches

This project implements a fully functional stopwatch on the STM32F411RE microcontroller using:
- A 16x2 LCD connected via I2C (with bit-banging)
- Two tactile switches for Start/Stop and Reset control

It uses **manual I2C (bit-banging)**—no HAL or I2C libraries—and is written entirely in **bare-metal Embedded C**.

---

## 🚀 Features

- 🕒 Stopwatch counts time in `HH:MM:SS` format
- 🟢 **Start / Pause toggle** via Switch 1 (PA5)
- 🔄 **Reset** stopwatch via Switch 2 (PA6)
- ✅ Displays current status (`Running`, `Paused`) on the second line
- 💡 LCD controlled over I2C using manual bit-banging (PB8: SCL, PB9: SDA)

---

## 🧰 Hardware Used

| Component               | Description                           |
|------------------------|---------------------------------------|
| STM32F411RE            | Nucleo Development Board              |
| 16x2 I2C LCD Module    | Based on PCF8574 I2C I/O Expander     |
| 2x Tactile Switches    | For Start/Pause and Reset             |
| Breadboard + Jumpers   | For wiring the circuit                |
| Resistors (10kΩ pull-up)| (Optional) for cleaner switch logic  |

---

## 📌 Pin Connections

| STM32 Pin | Function        | Connection        |
|-----------|-----------------|-------------------|
| PB8       | I2C SCL         | LCD SCL           |
| PB9       | I2C SDA         | LCD SDA           |
| PA5       | Start/Pause     | Switch 1 → GND    |
| PA6       | Reset           | Switch 2 → GND    |
| 3.3V      | Power           | LCD VCC, Switch   |
| GND       | Ground          | LCD GND, Switch   |

**Note:** Switches are active-low and connected with internal pull-up.

---

## 📷 Output Preview

### ▶️ Running State
![Running State](images/running.jpg)

### ⏸️ Paused State
![Paused State](images/paused.jpg)

### 🔄 Reset State
![Reset State](images/reset.jpg)

---

## 🛠️ How It Works

- LCD communication uses bit-banged I2C protocol (manual toggling of SDA/SCL).
- SysTick timer used for 1-second delay.
- PA5 toggles stopwatch state (start or pause).
- PA6 resets stopwatch counters.
- The LCD displays time and status messages based on user interaction.

---

## 🧪 Build & Flash Instructions

### ✅ Requirements

- STM32CubeIDE (for building and flashing)
- ST-Link debugger (built into Nucleo)
- Proper USB cable

### 🧱 Build Steps

1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/STM32-Stopwatch-I2C-LCD-BitBang.git
   ```
2. Open the `.project` in **STM32CubeIDE**.
3. Connect your Nucleo board.
4. Click **Build → Debug → Run**.

---

## 📂 Project Structure

```
Core/
├── Src/
│   └── main.c            # Main logic and bit-banging I2C
├── Inc/
│   └── ...               # (If any header files used)
Drivers/
├── ...                   # CMSIS & STM32 headers
```

---

## 📄 License

This project is open-source and free to use under the [MIT License](LICENSE).

---
