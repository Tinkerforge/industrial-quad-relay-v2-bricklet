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
		case FID_SET_OUTPUT_VALUE: return set_output_value(message);
		case FID_GET_OUTPUT_VALUE: return get_output_value(message, response);
		case FID_SET_MONOFLOP: return set_monoflop(message);
		case FID_GET_MONOFLOP: return get_monoflop(message, response);
		case FID_SET_SELECTED_OUTPUT_VALUE: return set_selected_output_value(message);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse set_output_value(const SetOutputValue *data) {
	for(uint8_t i = 0; i < RELAY_NUM; i++) {
		relay_set(i, data->value & (1 << i));
		relay.monoflop_done[i] = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_output_value(const GetOutputValue *data, GetOutputValue_Response *response) {
	response->header.length = sizeof(GetOutputValue_Response);
	response->value         = (relay_get(0) << 0) | (relay_get(1) << 1) | (relay_get(2) << 2) | (relay_get(3) << 3);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_monoflop(const SetMonoflop *data) {
	if(data->pin > RELAY_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	relay.monoflop_done[data->pin]       = false;
	relay.monoflop_time_start[data->pin] = system_timer_get_ms();
	relay.monoflop_time[data->pin]       = data->time;
	relay.monoflop_value[data->pin]      = data->value;

	relay_set(data->pin, data->value);

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_monoflop(const GetMonoflop *data, GetMonoflop_Response *response) {
	if(data->pin > RELAY_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	uint32_t diff = system_timer_get_ms() - relay.monoflop_time_start[data->pin];
	if((relay.monoflop_done[data->pin]) || (relay.monoflop_time[data->pin] == 0)) {
		diff = 0;
	}

	response->header.length  = sizeof(GetMonoflop_Response);
	response->time           = relay.monoflop_time[data->pin];
	response->time_remaining = diff > relay.monoflop_time[data->pin] ? 0 : diff;
	response->value          = relay_get(data->pin);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_selected_output_value(const SetSelectedOutputValue *data) {
	if(data->pin > RELAY_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	relay_set(data->pin, data->value);
	relay.monoflop_done[data->pin] = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}




bool handle_monoflop_done_callback(void) {
	static bool is_buffered = false;
	static MonoflopDone_Callback cb;

	if(!is_buffered) {
		uint8_t i = 0;
		for(; i < RELAY_NUM; i++) {
			if(relay.monoflop_callback[i]) {
				tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(MonoflopDone_Callback), FID_CALLBACK_MONOFLOP_DONE);
				cb.pin                     = i;
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
