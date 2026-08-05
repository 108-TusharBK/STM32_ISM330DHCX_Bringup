# STM32 ISM330DHCX Bring-up

Professional bring-up of the **ST ISM330DHCX 6-axis IMU** on the **STM32 Nucleo-F446RE** using the **official ST sensor driver**.

This project focuses on clean firmware architecture, modular driver design, and production-style embedded development rather than a tutorial implementation.

---

# Hardware

- STM32 Nucleo-F446RE
- ISM330DHCX 6-Axis IMU
- STM32CubeIDE
- ST HAL Driver
- Official ST ISM330DHCX Driver

---

# Images

## Hardware Setup

![Hardware Setup](docs/hardware_setup.jpeg)

## UART Output

![UART Output](docs/uart_output.png)

---

# Features

- ✅ I2C Communication
- ✅ WHO_AM_I Device Verification
- ✅ Register Read
- ✅ Register Write
- ✅ Accelerometer Configuration
- ✅ Accelerometer Data Acquisition
- ✅ Gyroscope Configuration
- ✅ Gyroscope Data Acquisition
- ✅ Temperature Sensor Reading
- ✅ Physical Unit Conversion (g, dps, °C)
- ✅ Modular Driver Architecture
- ✅ Error Handling
- ✅ Git Version Control

---

# Project Architecture

```
Application (main.c)
        │
        ▼
IMU Driver (imu.c / imu.h)
        │
        ▼
Official ST ISM330DHCX Driver
        │
        ▼
STM32 HAL I2C
        │
        ▼
ISM330DHCX Sensor
```

---

# Folder Structure

```
Core/
│
├── Inc/
│   ├── imu.h
│   └── ...
│
├── Src/
│   ├── imu.c
│   ├── main.c
│   └── ...
│
Drivers/
│
├── ISM330DHCX/
│
└── STM32F4xx_HAL_Driver/
```

---

# Hardware Connections

| ISM330DHCX | STM32F446RE |
|------------|-------------|
| SDA | PB9 (I2C1 SDA) |
| SCL | PB8 (I2C1 SCL) |
| 3V3 | 3.3V |
| GND | GND |

Communication Interface:

- I2C
- 7-bit Address: 0x6B

---

# Driver API

```c
int32_t IMU_Init(I2C_HandleTypeDef *hi2c);

int32_t IMU_ReadAccel(float *ax,
                      float *ay,
                      float *az);

int32_t IMU_ReadGyro(float *gx,
                     float *gy,
                     float *gz);

int32_t IMU_ReadTemperature(float *temperature);
```

---

# Example UART Output

```
Acc: X=-0.003  Y=-0.034  Z=1.012 g

Gyr: X=0.07  Y=-0.46  Z=0.29 dps

Temperature: 24.84 °C
```

---

# Development Process

This project was developed incrementally using small, testable milestones:

- Project Setup
- UART Communication
- I2C Configuration
- WHO_AM_I Verification
- Register Read/Write
- Accelerometer Driver
- Gyroscope Driver
- Temperature Driver
- IMU Bring-up Completion

Each milestone was committed independently using Git.

---

# Design Goals

- Clean Embedded C
- Modular Driver Design
- No modification of STM32 HAL
- No modification of Official ST Driver
- Separation of Application and Driver Layers
- Production-style Firmware Architecture

---

# Future Improvements

Planned improvements include:

- OLED Dashboard
- Timer-based Sampling
- Ring Buffer
- Binary Data Logger
- SD Card Storage
- UART Command Line Interface
- DMA Support
- Interrupt-driven Data Acquisition
- Sensor Fusion
- Python Visualization Tool

---

# Learning Objectives

This project was built to gain practical experience with:

- Embedded C
- STM32 HAL
- STM32CubeIDE
- I2C Communication
- MEMS Sensors
- Driver Development
- Firmware Architecture
- Git & GitHub
- Embedded Debugging

---

# Repository Status

Current Version:

**IMU Bring-up Complete ✅**

Next milestone:

**Embedded Flight Recorder Firmware Platform**

---

# License

This project is provided for educational and portfolio purposes.
