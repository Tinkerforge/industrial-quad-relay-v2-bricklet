/* industrial-quad-relay-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * relay.h: Relay handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef RELAY_H
#define RELAY_H

#include <stdint.h>
#include <stdbool.h>

#include "communication.h"
#include "bricklib2/utility/led_flicker.h"

#include "xmc_gpio.h"

#define RELAY_NUM 4

typedef struct {
	uint8_t pin;
	uint8_t config;
	XMC_GPIO_PORT_t *port_base;
	LEDFlickerState channel_led_flicker_state;
} CHANNEL_LED_CONFIG_t;

typedef struct {
	uint32_t monoflop_time[RELAY_NUM];
	uint32_t monoflop_time_start[RELAY_NUM];
	bool monoflop_done[RELAY_NUM];
	bool monoflop_callback[RELAY_NUM];
	CHANNEL_LED_CONFIG_t channel_leds[RELAY_NUM];
} Relay;

bool relay_get(const uint8_t pin);
void relay_set(const uint8_t pin, const bool value);

void relay_init(void);
void relay_tick(void);

#endif
