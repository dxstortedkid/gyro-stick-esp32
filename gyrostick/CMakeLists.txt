# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(gyrostick)

target_include_directories(app PRIVATE
    src
    src/modules/mpu6050
)

target_sources(app PRIVATE
        src/main.cpp
        src/modules/mpu6050/mpu6050.cpp
)
