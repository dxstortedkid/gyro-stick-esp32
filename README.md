GyroStick is a single-axis (Y-axis pitch) balancing system powered by an ESP32-C3 Super Mini running Zephyr RTOS. Attitude stabilization is achieved using an MPU-6050 IMU and dual 5V brushed motors with propellers driven by an MX1508 motor driver. MATLAB/Simulink is used exclusively as an offline tool for control system design, filter formulation, and generating optimized, standalone C code (PID and digital filters) for native on-chip execution.

## Hardware

* **ESP32-C3** — main microcontroller.
* **MPU6050** — 6-axis IMU:

  * 3-axis accelerometer
  * 3-axis gyroscope
* **MX1508** — 2-channel DC motor driver.
* **2× DC motors 716** — 3.7 V, coreless, high-speed.
* **I²C** — communication between ESP32-C3 and MPU6050.
* **INT** — interrupt signal from MPU6050.
* **3.3 V / motor supply** — power for electronics and motors.

### Data

* Accelerometer: `X/Y/Z` — `m/s²`
* Gyroscope: `X/Y/Z` — `rad/s`
* Motors: `2× DC`, controlled via MX1508.


Pinout
                 ┌───────────────┐
  MX1508 IN1 ────┤ GPIO0    PWM  │
  MX1508 IN2 ────┤ GPIO1    PWM  │  
  MX1508 IN3 ────┤ GPIO2    PWM  │  
  MX1508 IN4 ────┤ GPIO3    PWM  │
                 │ GPIO4         │  
                 │               │
 MPU6050 INT ────┤ GPIO5    IRQ  │
 MPU6050 SDA ────┤ GPIO6    I2C  │  
 MPU6050 SCL ────┤ GPIO7    I2C  │  
                 └───────────────┘


[ MPU6050 INT (GPIO5) ]
            │
            ▼  k_sem_give()
┌────────────────────────────────────────────────────────┐
│ THREAD 1: fast_control_thread (High Priority)          │
│ 1. k_sem_take() — wake up when IMU data is ready       │
│ 2. Read data via I2C + calculate tilt angle            │
│ 3. Run the controller (local PID or MATLAB setpoint)   │
│ 4. Update motor PWM output immediately (< 1 µs)        │
│ 5. k_msgq_put(telemetry) ──┐                           │
└────────────────────────────┼───────────────────────────┘
                             │
                    k_msgq (Telemetry)
                             │
                             ▼
┌────────────────────────────────────────────────────────┐
│ THREAD 2: comm_thread (Low Priority)                   │
│ 1. Transmit telemetry via USB/UART (slow I/O)          │
│ 2. Receive control commands / gains from MATLAB        │
│ 3. k_msgq_put(command) ──> pass setpoints to Thread 1  │
└────────────────────────────────────────────────────────┘
               