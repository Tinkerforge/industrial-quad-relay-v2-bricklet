// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_industrial_quad_relay_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_IndustrialQuadRelayV2 iqr;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_industrial_quad_relay_v2_create(&iqr, NULL, hal), "create device object");

	// Turn relays alternating on/off 10 times with 100 ms delay
	int i;
	for (i = 0; i < 10; ++i) {
		tf_hal_sleep_us(hal, 100 * 1000);
		bool value[4] = {true, false, false, false};
		check(tf_industrial_quad_relay_v2_set_value(&iqr, value), "call set_value");
		tf_hal_sleep_us(hal, 100 * 1000);
		bool value_1[4] = {false, true, false, false};
		check(tf_industrial_quad_relay_v2_set_value(&iqr, value_1), "call set_value");
		tf_hal_sleep_us(hal, 100 * 1000);
		bool value_2[4] = {false, false, true, false};
		check(tf_industrial_quad_relay_v2_set_value(&iqr, value_2), "call set_value");
		tf_hal_sleep_us(hal, 100 * 1000);
		bool value_3[4] = {false, false, false, true};
		check(tf_industrial_quad_relay_v2_set_value(&iqr, value_3), "call set_value");
	}
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
