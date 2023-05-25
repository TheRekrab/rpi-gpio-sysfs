#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gpio_sysfs.h"

int main() {
	struct gpio_pin* pin4 = malloc(sizeof(struct gpio_pin));

	pin_init(pin4, 4);

	pin_mode(pin4, PIN_OUT);

	pin_write(pin4, PIN_HIGH);

	sleep(1);

	pin_write(pin4, PIN_LOW);

	pin_close(pin4);

	return EXIT_SUCCESS;
}
