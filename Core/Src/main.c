#include "stm32f4xx.h"
#include <stdio.h>

// ===== I2C LCD DEFINES =====
#define LCD_ADDR 0x4E  // 0x27 << 1
#define SCL_PIN 8      // PB8
#define SDA_PIN 9      // PB9

#define START_STOP_PIN 5  // PA5
#define RESET_PIN      6  // PA6

// ======= FUNCTION PROTOTYPES =======
void delay_us(uint32_t us);
void I2C_delay(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_write(uint8_t data);
void LCD_send_cmd(uint8_t cmd);
void LCD_send_data(uint8_t data);
void LCD_send_string(char *str);
void LCD_init(void);
void LCD_set_cursor(uint8_t row, uint8_t col);
void DisplayTime(uint8_t h, uint8_t m, uint8_t s);
void GPIO_init(void);

// ======= DELAY FUNCTIONS =======
void delay_us(uint32_t us) {
    SysTick->LOAD = 16 * us - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while ((SysTick->CTRL & 0x10000) == 0);
    SysTick->CTRL = 0;
}

void I2C_delay(void) {
    delay_us(2);
}

// ======= BIT-BANGING I2C =======
void I2C_start(void) {
    GPIOB->ODR |= (1 << SDA_PIN);
    GPIOB->ODR |= (1 << SCL_PIN);
    I2C_delay();
    GPIOB->ODR &= ~(1 << SDA_PIN);
    I2C_delay();
    GPIOB->ODR &= ~(1 << SCL_PIN);
    I2C_delay();
}

void I2C_stop(void) {
    GPIOB->ODR &= ~(1 << SDA_PIN);
    GPIOB->ODR |= (1 << SCL_PIN);
    I2C_delay();
    GPIOB->ODR |= (1 << SDA_PIN);
    I2C_delay();
}

void I2C_write(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        if (data & 0x80)
            GPIOB->ODR |= (1 << SDA_PIN);
        else
            GPIOB->ODR &= ~(1 << SDA_PIN);

        GPIOB->ODR |= (1 << SCL_PIN);
        I2C_delay();
        GPIOB->ODR &= ~(1 << SCL_PIN);
        I2C_delay();

        data <<= 1;
    }

    // ACK clock
    GPIOB->ODR |= (1 << SCL_PIN);
    I2C_delay();
    GPIOB->ODR &= ~(1 << SCL_PIN);
    I2C_delay();
}

// ======= LCD FUNCTIONS =======
void LCD_send_cmd(uint8_t cmd) {
    uint8_t high = cmd & 0xF0;
    uint8_t low = (cmd << 4) & 0xF0;
    uint8_t data;

    I2C_start();
    I2C_write(LCD_ADDR);

    data = high | 0x08;
    I2C_write(data);
    I2C_write(data | 0x04);
    I2C_write(data);

    data = low | 0x08;
    I2C_write(data);
    I2C_write(data | 0x04);
    I2C_write(data);

    I2C_stop();
    delay_us(50);
}

void LCD_send_data(uint8_t dataByte) {
    uint8_t high = dataByte & 0xF0;
    uint8_t low = (dataByte << 4) & 0xF0;
    uint8_t data;

    I2C_start();
    I2C_write(LCD_ADDR);

    data = high | 0x09;
    I2C_write(data);
    I2C_write(data | 0x04);
    I2C_write(data);

    data = low | 0x09;
    I2C_write(data);
    I2C_write(data | 0x04);
    I2C_write(data);

    I2C_stop();
    delay_us(50);
}

void LCD_send_string(char *str) {
    while (*str) {
        LCD_send_data(*str++);
    }
}

void LCD_set_cursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? 0x80 + col : 0xC0 + col;
    LCD_send_cmd(addr);
}

void LCD_init(void) {
    delay_us(50000);
    LCD_send_cmd(0x30); delay_us(5000);
    LCD_send_cmd(0x30); delay_us(1000);
    LCD_send_cmd(0x30); delay_us(1000);
    LCD_send_cmd(0x20); // 4-bit mode
    LCD_send_cmd(0x28); // 2 lines
    LCD_send_cmd(0x0C); // Display ON, Cursor OFF
    LCD_send_cmd(0x06); // Entry mode
    LCD_send_cmd(0x01); // Clear
    delay_us(2000);
}

// ======= DISPLAY TIME =======
void DisplayTime(uint8_t h, uint8_t m, uint8_t s) {
    char buf[17];
    sprintf(buf, "%02d:%02d:%02d", h, m, s);
    LCD_set_cursor(1, 0);
    LCD_send_string(buf);
}

// ======= GPIO INIT =======
void GPIO_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // PB8 & PB9 as open-drain output
    GPIOB->MODER &= ~(0xF << (2 * SCL_PIN));
    GPIOB->MODER |= (0x5 << (2 * SCL_PIN));
    GPIOB->OTYPER |= (1 << SCL_PIN) | (1 << SDA_PIN);
    GPIOB->OSPEEDR |= (0xF << (2 * SCL_PIN));
    GPIOB->PUPDR &= ~(0xF << (2 * SCL_PIN));

    // PA5 & PA6 as input with pull-up
    GPIOA->MODER &= ~(0xF << (2 * START_STOP_PIN));
    GPIOA->PUPDR &= ~(0xF << (2 * START_STOP_PIN));
    GPIOA->PUPDR |=  (0x5 << (2 * START_STOP_PIN));
}

// ======= MAIN FUNCTION =======
int main(void) {
    SystemInit();
    GPIO_init();
    LCD_init();

    LCD_set_cursor(0, 0);
    LCD_send_string("STOPWATCH");

    uint8_t running = 0;
    uint8_t hrs = 0, min = 0, sec = 0;
    uint8_t last_state_pa5 = 1, last_state_pa6 = 1;

    while (1) {
        uint8_t curr_pa5 = (GPIOA->IDR & (1 << START_STOP_PIN)) ? 1 : 0;
        uint8_t curr_pa6 = (GPIOA->IDR & (1 << RESET_PIN)) ? 1 : 0;

        // Start/Stop Toggle
        if (!curr_pa5 && last_state_pa5) {
            running = !running;
            LCD_set_cursor(1, 9);
            LCD_send_string(running ? "RUNNING" : "PAUSED ");
            delay_us(200000);  // debounce
        }
        last_state_pa5 = curr_pa5;

        // Reset Button
        if (!curr_pa6 && last_state_pa6) {
            hrs = min = sec = 0;
            DisplayTime(hrs, min, sec);
            LCD_set_cursor(1, 9);
            LCD_send_string("PAUSED ");
            delay_us(200000);  // debounce
        }
        last_state_pa6 = curr_pa6;

        // Time Counting
        if (running) {
            delay_us(1000000);
            sec++;
            if (sec == 60) {
                sec = 0;
                min++;
                if (min == 60) {
                    min = 0;
                    hrs++;
                }
            }
            DisplayTime(hrs, min, sec);
        }
    }
}
