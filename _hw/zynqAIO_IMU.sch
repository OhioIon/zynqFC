EESchema Schematic File Version 4
LIBS:zynqAIO-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 14
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:C C701
U 1 1 5B607D10
P 3000 4300
F 0 "C701" H 3025 4400 50  0000 L CNN
F 1 "100nF" H 3025 4200 50  0000 L CNN
F 2 "" H 3038 4150 50  0001 C CNN
F 3 "" H 3000 4300 50  0001 C CNN
	1    3000 4300
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR082
U 1 1 5B607DEE
P 3600 2550
F 0 "#PWR082" H 3600 2400 50  0001 C CNN
F 1 "+3V3" H 3600 2690 50  0000 C CNN
F 2 "" H 3600 2550 50  0001 C CNN
F 3 "" H 3600 2550 50  0001 C CNN
	1    3600 2550
	1    0    0    -1  
$EndComp
Text GLabel 3300 3500 0    60   Input ~ 0
IMU_SPI_MOSI
Text GLabel 3300 3400 0    60   Input ~ 0
IMU_SPI_MISO
Text GLabel 3300 3200 0    60   Input ~ 0
IMU_SPI_SCK
Text GLabel 3300 3300 0    60   Input ~ 0
IMU_SPI_nCS1
$Comp
L Device:R R701
U 1 1 5B608ACD
P 3600 2850
F 0 "R701" V 3680 2850 50  0000 C CNN
F 1 "4.7k" V 3600 2850 50  0000 C CNN
F 2 "" V 3530 2850 50  0001 C CNN
F 3 "" H 3600 2850 50  0001 C CNN
	1    3600 2850
	1    0    0    -1  
$EndComp
$Comp
L zynqAIO:BMX160 U701
U 1 1 5B60B055
P 4350 3050
F 0 "U701" H 4350 3050 60  0000 C CNN
F 1 "BMX160" H 4350 3150 60  0000 C CNN
F 2 "" H 4350 3050 60  0001 C CNN
F 3 "" H 4350 3050 60  0001 C CNN
	1    4350 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3200 3850 3200
Wire Wire Line
	3300 3300 3600 3300
Wire Wire Line
	3300 3400 3850 3400
Wire Wire Line
	3300 3500 3850 3500
Wire Wire Line
	3600 3300 3600 3000
Connection ~ 3600 3300
Wire Wire Line
	3600 2700 3600 2550
$Comp
L Device:C C702
U 1 1 5B60B332
P 3350 4300
F 0 "C702" H 3375 4400 50  0000 L CNN
F 1 "100nF" H 3375 4200 50  0000 L CNN
F 2 "" H 3388 4150 50  0001 C CNN
F 3 "" H 3350 4300 50  0001 C CNN
	1    3350 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 4150 3000 4100
Wire Wire Line
	2800 4100 3000 4100
Connection ~ 3000 4100
Wire Wire Line
	3350 4150 3350 4100
Connection ~ 3350 4100
Wire Wire Line
	3850 4450 3800 4450
Wire Wire Line
	3800 4450 3800 4100
Connection ~ 3800 4100
Wire Wire Line
	3850 4200 3750 4200
Wire Wire Line
	3750 4200 3750 4550
Wire Wire Line
	2800 4550 3000 4550
Connection ~ 3750 4550
Wire Wire Line
	3350 4450 3350 4550
Connection ~ 3350 4550
Wire Wire Line
	3000 4450 3000 4550
Connection ~ 3000 4550
$Comp
L power:+3V3 #PWR083
U 1 1 5B60B4E1
P 2800 4100
F 0 "#PWR083" H 2800 3950 50  0001 C CNN
F 1 "+3V3" H 2800 4240 50  0000 C CNN
F 2 "" H 2800 4100 50  0001 C CNN
F 3 "" H 2800 4100 50  0001 C CNN
	1    2800 4100
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR084
U 1 1 5B60B51E
P 2800 4550
F 0 "#PWR084" H 2800 4300 50  0001 C CNN
F 1 "GND" H 2800 4400 50  0000 C CNN
F 2 "" H 2800 4550 50  0001 C CNN
F 3 "" H 2800 4550 50  0001 C CNN
	1    2800 4550
	0    1    1    0   
$EndComp
Text GLabel 3300 3750 0    60   Input ~ 0
IMU_INT1
Text GLabel 3300 3850 0    60   Input ~ 0
IMU_INT2
Wire Wire Line
	3300 3750 3850 3750
Wire Wire Line
	3850 3850 3300 3850
Text Notes 2050 3600 0    60   ~ 0
to Zynq\nPS bank\n501
$Comp
L sensors:MPU-6000 U?
U 1 1 5BCF68EA
P 8500 3750
F 0 "U?" H 8050 4300 50  0000 C CNN
F 1 "MPU-6000" H 8800 3200 50  0000 C CNN
F 2 "" H 8500 2950 50  0001 C CNN
F 3 "" H 8500 3600 50  0001 C CNN
	1    8500 3750
	1    0    0    -1  
$EndComp
Text GLabel 7300 3650 0    60   Input ~ 0
IMU_SPI_SCK
Wire Wire Line
	7300 3650 7800 3650
$Comp
L Device:R R?
U 1 1 5BCFCB04
P 7500 3150
F 0 "R?" V 7580 3150 50  0000 C CNN
F 1 "4.7k" V 7500 3150 50  0000 C CNN
F 2 "" V 7430 3150 50  0001 C CNN
F 3 "" H 7500 3150 50  0001 C CNN
	1    7500 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 3750 7500 3750
Wire Wire Line
	7500 3750 7500 3300
Connection ~ 7500 3750
Text GLabel 7300 3750 0    60   Input ~ 0
IMU_SPI_nCS0
$Comp
L Device:C C?
U 1 1 5BCFCC82
P 9000 2850
F 0 "C?" H 9025 2950 50  0000 L CNN
F 1 "100nF" H 9025 2750 50  0000 L CNN
F 2 "" H 9038 2700 50  0001 C CNN
F 3 "" H 9000 2850 50  0001 C CNN
	1    9000 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5BCFCD94
P 9400 2850
F 0 "C?" H 9425 2950 50  0000 L CNN
F 1 "100nF" H 9425 2750 50  0000 L CNN
F 2 "" H 9438 2700 50  0001 C CNN
F 3 "" H 9400 2850 50  0001 C CNN
	1    9400 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 2550 8500 2650
Wire Wire Line
	8500 2650 9000 2650
Wire Wire Line
	9400 2650 9400 2700
Wire Wire Line
	9000 2700 9000 2650
Connection ~ 9000 2650
Connection ~ 8500 2650
$Comp
L power:+3V3 #PWR?
U 1 1 5BCFCE9C
P 8500 2550
F 0 "#PWR?" H 8500 2400 50  0001 C CNN
F 1 "+3V3" H 8500 2690 50  0000 C CNN
F 2 "" H 8500 2550 50  0001 C CNN
F 3 "" H 8500 2550 50  0001 C CNN
	1    8500 2550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BCFCF57
P 9200 3100
F 0 "#PWR?" H 9200 2850 50  0001 C CNN
F 1 "GND" H 9200 2950 50  0000 C CNN
F 2 "" H 9200 3100 50  0001 C CNN
F 3 "" H 9200 3100 50  0001 C CNN
	1    9200 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 3100 9200 3050
Wire Wire Line
	9000 3050 9200 3050
Wire Wire Line
	9000 3050 9000 3000
Wire Wire Line
	9400 3050 9400 3000
Connection ~ 9200 3050
$Comp
L power:+3V3 #PWR?
U 1 1 5BCFD092
P 7500 2900
F 0 "#PWR?" H 7500 2750 50  0001 C CNN
F 1 "+3V3" H 7500 3040 50  0000 C CNN
F 2 "" H 7500 2900 50  0001 C CNN
F 3 "" H 7500 2900 50  0001 C CNN
	1    7500 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 3000 7500 2900
Text GLabel 7300 3450 0    60   Input ~ 0
IMU_SPI_MOSI
Wire Wire Line
	7300 3450 7800 3450
Text GLabel 7300 3550 0    60   Input ~ 0
IMU_SPI_MISO
Wire Wire Line
	7300 3550 7800 3550
$Comp
L power:GND #PWR?
U 1 1 5BCFD274
P 8500 4500
F 0 "#PWR?" H 8500 4250 50  0001 C CNN
F 1 "GND" H 8500 4350 50  0000 C CNN
F 2 "" H 8500 4500 50  0001 C CNN
F 3 "" H 8500 4500 50  0001 C CNN
	1    8500 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 4500 8500 4450
Text Notes 7550 4850 0    60   ~ 0
TODO: MPU-6000 noch nicht fertig angeschlossen!
Text Notes 6100 3750 0    60   ~ 0
to Zynq\nPS bank\n501
Wire Wire Line
	3600 3300 3850 3300
Wire Wire Line
	3000 4100 3350 4100
Wire Wire Line
	3350 4100 3800 4100
Wire Wire Line
	3800 4100 3850 4100
Wire Wire Line
	3750 4550 3850 4550
Wire Wire Line
	3350 4550 3750 4550
Wire Wire Line
	3000 4550 3350 4550
Wire Wire Line
	7500 3750 7800 3750
Wire Wire Line
	9000 2650 9400 2650
Wire Wire Line
	8500 2650 8500 3050
Wire Wire Line
	9200 3050 9400 3050
Text GLabel 9500 3450 2    60   Input ~ 0
IMU_INT3
Wire Wire Line
	9200 3450 9500 3450
$EndSCHEMATC
