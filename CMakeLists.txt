#
# Copyright (c) 2022 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

set(ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_LIST_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app)

target_sources(app PRIVATE
    cpuapp/main.c
    cpuapp/blink_thread.c
    cpuapp/led.c
    common/gpio.c
    common/ipc.c
    common/button.c
)
