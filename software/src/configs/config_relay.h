/* industrial-quad-relay-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_relay.h: Relay configuration
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

#ifndef CONFIG_RELAY_H
#define CONFIG_RELAY_H

#include "xmc_gpio.h"

#define RELAY0_PORT     XMC_GPIO_PORT1
#define RELAY0_PIN_NUM 0
#define RELAY1_PORT     XMC_GPIO_PORT0
#define RELAY1_PIN_NUM 0
#define RELAY2_PORT     XMC_GPIO_PORT0
#define RELAY2_PIN_NUM 5
#define RELAY3_PORT     XMC_GPIO_PORT0
#define RELAY3_PIN_NUM 6

#endif
