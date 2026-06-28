# SmartPlant — STM32 Nucleo-F401RE RTOS based Plant & Aquarium Monitoring System

An embedded project running DHT11 (temperature/humidity), water level sensor (ADC), LCD, LED, buzzer, and button on FreeRTOS (CMSIS-RTOS v2).

## 1. Features

- Displays current temperature, humidity, and water level data on the LCD in real-time.
- Activates LED and buzzer, and outputs warning logs via UART when the water level exceeds the threshold.
- Deactivates LED and buzzer, and outputs normalised logs via UART when the water level returns to normal.
- Manually toggles the LED/buzzer using a button.
- If the water level remains above the threshold for more than 10 seconds after the alarm has been silenced via the button, the alarm resumes.

## 2. Pin Mapping 

| Pin | Function | Mode | Target Device Pin | Notes |
|---|---|---|---|---|
| PA0 | ADC1_IN0 | Analog Input | Water Level Sensor | 12bit (0~4095) |
| PA1 | GPIO | Output & Input | DHT11 Sensor | Open-Drain, Pull-up, 1-wire protocol. |
| PA2 | USART2_TX | Alternate Function  | PC Terminal(ST-Link) | 115200 baud |
| PA3 | USART2_RX | Alternate Function  | PC Terminal(ST-Link) | Unused |
| PA4 | GPIO_Output| GPIO_Output (Push-Pull) | LED | No Pull |
| PA8 | GPIO_Output | GPIO_Output (Push-Pull) | Buzzer | No Pull |
| PA10 | GPIO_EXTI10 | Input (Interrupt) | Button | Falling Edge, Pull-up |
| PB8 | I2C1_SCL | Alternate Function | LCD (16x2, I2C) | Device Address 0x27 |
| PB9 | I2C1_SDA | Alternate Function | LCD (16x2, I2C) | |

## 3. Synchronization

| Object  | Purpose |
|---|---|
| Message Queue | Sensors → LCD data transmission | 
| Event Flags | Water level sensor / Button → Alarm control signals | 
| Binary Semaphore | EXTI ISR → ButtonTask | 
| Mutex | Protects shared USART serial communication resource |

## 4. Directory

```text
Project
├── Src/
│   └── main.c
├── Inc/
│   └── main.h
├── Drivers/
├── Middlewares/
└── App/
    ├── Inc/  
    │   └── app_resources.h	Shared message types, event bits, and RTOS handles across task files
    ├── BSP/			Hardware drivers
    │   ├── dht11.c/.h
    │   ├── water_level.c/.h
    │   ├── lcd.c/.h
    │   ├── led.c/.h
    │   ├── buzzer.c/.h
    │   └── button.c/.h
    └── Tasks/			RTOS Tasks
        ├── sensor_task.c	DHT11Task, WSTask
        ├── display_task.c	LCDTask
        ├── alarm_task.c	AlarmTask, DebugTask
        └── button_task.c	ButtonTask, EXTI Callback
```

## 5. Build Environment

- Board: NUCLEO-F401RE
- Toolchain: STM32CubeIDE 2.1.1
- RTOS: FreeRTOS via CMSIS-RTOS v2
