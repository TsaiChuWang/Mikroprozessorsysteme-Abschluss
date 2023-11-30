# Mikroprozessorsysteme-Abschluss
Für die Unterrichtstunde Mikroprozessorsysteme und Schnittstellentechnik an der Nationaluniversität

# [Thema]


## Members

李宣瑩 , [ashelyli0213@gmail.com](mailto:ashelyli0213@gmail.com)

黃文龍 , [wayne06130613@gmail.com](mailto:wayne06130613@gmail.com)

王采筑 , [wtsaichu@gmail.com](mailto:wtsaichu@gmail.com)

## **Environment Dependency**

**Operation System** : MX-23.1_x64, Debian 6.1.55-1 (2023-09-29) x86_64 GNU/Linux

**Board** : ESP32-CAM (AI-Thinker)

**ESP-IDF Version** : v5.3

## Target of the project

For the final project of the lesson **"中正大學 National Chung Cheng University 2022 Fall COMM 4305175 微處理機系統與介面技術 Microprocessor based system and interface techniques "**

### Reqirements of the project

- Using NUC140 or eESP32
- Using AI ,even if simple algorithm with accurancy over 70 %
- The model should run on the board
- The data should comes form the sensor, e.G ADXL345
- **Do not use Tensorflow** , you must develop a algorithm by yourself

## Folder Description

├── [ReadMe.md](http://readme.md/)              Help documentation

├── Einheit Prüfung         Unit test

│   ├── Presentätionen    Power ponits

## Instructions for use

```c
cd [target folder path]
```

```c
get_idf
```

```c
idf.py set-target esp32
```

```c
idf.py menuconfig
```

```c
idf.py build
```

```c
idf.py -p /dev/ttyUSB0 flash
```

```c
idf.py -p /dev/ttyUSB0 monitor
```