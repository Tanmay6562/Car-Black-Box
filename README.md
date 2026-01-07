# 🚗 Car Black Box System (Embedded)

## 📌 Overview
This project implements an **automotive car black box system** that records critical vehicle parameters and events.  
It is designed to help in **accident analysis, driver behavior monitoring, and vehicle diagnostics**.

---

## ⚙️ Hardware Components
- PIC16F877A Microcontroller
- 16x2 Character LCD (CLCD)
- External EEPROM
- RTC Module
- Matrix Keypad
- ADC-based Speed Sensor
- Power Supply Unit

---

## 💻 Software Tools
- Embedded C
- MPLAB X IDE
- XC8 Compiler
- Picsimlab

---

## 🔑 Features
- Real-time vehicle parameter display
- Event logging with timestamp
- EEPROM-based data storage
- Password-protected menu system
- Log viewing and management
- User navigation via keypad

---

## 🧠 Working Principle
1. System initializes LCD, EEPROM, RTC, ADC, and keypad
2. Vehicle parameters are continuously monitored
3. Events are logged into EEPROM with date & time
4. Secure menu allows viewing logs and system settings

---

## 📂 Folder Structure
car-black-box-embedded/
│
├── README.md
│
├── src/              # ALL .c files
│   ├── main.c
│   ├── lcd.c
│   ├── keypad.c
│   ├── eeprom.c
│   ├── rtc.c
│
├── inc/              # ALL .h files
│   ├── lcd.h
│   ├── keypad.h
│   ├── eeprom.h
│   ├── rtc.h
