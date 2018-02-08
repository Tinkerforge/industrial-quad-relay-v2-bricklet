/* industrial-quad-relay-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * relay.c: Relay handling
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

#include "relay.h"

#include "bricklib2/hal/system_timer/system_timer.h"

#include "xmc_gpio.h"

#include "configs/config_relay.h"

const uint8_t relay_pins[RELAY_NUM] = {RELAY0_PIN_NUM, RELAY1_PIN_NUM, RELAY2_PIN_NUM, RELAY3_PIN_NUM};

Relay relay;

bool relay_get(const uint8_t pin) {
	return !XMC_GPIO_GetInput(RELAY_PORT, relay_pins[pin]);
}

void relay_set(const uint8_t pin, const bool value) {
	XMC_GPIO_SetOutputLevel(RELAY_PORT, relay_pins[pin], value ? XMC_GPIO_OUTPUT_LEVEL_LOW : XMC_GPIO_OUTPUT_LEVEL_HIGH);
}

void relay_init(void) {
	memset(&relay, 0, sizeof(Relay));

	XMC_GPIO_CONFIG_t button_pin_config = {
		.mode         = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
	};

	for(uint8_t i = 0; i < RELAY_NUM; i++) {
		XMC_GPIO_Init(RELAY_PORT, relay_pins[i], &button_pin_config);
		relay.monoflop_done[i] = true;
	}
}

void relay_tick(void) {
	for(uint8_t i = 0; i < RELAY_NUM; i++) {
		if(!relay.monoflop_done[i]) {
			if(system_timer_is_time_elapsed_ms(relay.monoflop_time_start[i], relay.monoflop_time[i])) {
				relay_set(i, !relay.monoflop_value[i]);
				relay.monoflop_callback[i] = true;
				relay.monoflop_done[i]     = true;
			}
		}
	}
}
