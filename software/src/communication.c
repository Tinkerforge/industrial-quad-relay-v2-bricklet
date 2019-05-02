/* industrial-quad-relay-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "configs/config_relay.h"
#include "relay.h"

extern Relay relay;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_SET_VALUE: return set_value(message);
		case FID_GET_VALUE: return get_value(message, response);
		case FID_SET_MONOFLOP: return set_monoflop(message);
		case FID_GET_MONOFLOP: return get_monoflop(message, response);
		case FID_SET_SELECTED_VALUE: return set_selected_value(message);
		case FID_SET_CHANNEL_LED_CONFIG: return set_channel_led_config(message);
		case FID_GET_CHANNEL_LED_CONFIG: return get_channel_led_config(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse set_value(const SetValue *data) {
	for(uint8_t i = 0; i < RELAY_NUM; i++) {
		if (data->value & (1 << i)) {
			relay_set(i, true);
		}
		else {
			relay_set(i, false);
		}

		relay.monoflop_done[i] = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_value(const GetValue *data, GetValue_Response *response) {
	response->header.length = sizeof(GetValue_Response);

	uint8_t bit_encoded_values = 0;

	for(uint8_t i = 0; i < RELAY_NUM; i++) {
		if(relay_get(i)) {
			bit_encoded_values |= (1 << i);
		}
		else {
			bit_encoded_values &= ~(1 << i);
		}
	}

	response->value = bit_encoded_values;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_monoflop(const SetMonoflop *data) {
	if(data->channel >= RELAY_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	relay.monoflop_done[data->channel]       = false;
	relay.monoflop_time_start[data->channel] = system_timer_get_ms();
	relay.monoflop_time[data->channel]       = data->time;
	relay.monoflop_value[data->channel]      = data->value;

	relay_set(data->channel, data->value);

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_monoflop(const GetMonoflop *data, GetMonoflop_Response *response) {
	if(data->channel >= RELAY_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	uint32_t diff = system_timer_get_ms() - relay.monoflop_time_start[data->channel];
	if((relay.monoflop_done[data->channel]) || (relay.monoflop_time[data->channel] == 0)) {
		diff = 0;
	}

	response->header.length  = sizeof(GetMonoflop_Response);
	response->time           = relay.monoflop_time[data->channel];
	response->time_remaining = diff > relay.monoflop_time[data->channel] ? 0 : diff;
	response->value          = relay_get(data->channel);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_selected_value(const SetSelectedValue *data) {
	if(data->channel >= RELAY_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	relay_set(data->channel, data->value);
	relay.monoflop_done[data->channel] = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_channel_led_config(const SetChannelLEDConfig *data) {
	if(data->led >= RELAY_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	relay.channel_leds[data->led].config = data->config;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_channel_led_config(const GetChannelLEDConfig *data, GetChannelLEDConfig_Response *response) {
	if(data->led >= RELAY_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetChannelLEDConfig_Response);
	response->config = relay.channel_leds[data->led].config;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_monoflop_done_callback(void) {
	static bool is_buffered = false;
	static MonoflopDone_Callback cb;

	if(!is_buffered) {
		uint8_t i = 0;
		for(; i < RELAY_NUM; i++) {
			if(relay.monoflop_callback[i]) {
				tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(MonoflopDone_Callback), FID_CALLBACK_MONOFLOP_DONE);
				cb.channel                 = i;
				cb.value                   = relay_get(i);
				relay.monoflop_callback[i] = false;
				break;
			}
		}
		if(i == RELAY_NUM) {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(MonoflopDone_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
