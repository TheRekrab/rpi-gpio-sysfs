#ifndef RPI_GPIO_SYSFS
#define RPI_GPIO_SYSFS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

#define PIN_HIGH 1
#define PIN_LOW 0

#define PIN_IN 0
#define PIN_OUT 1

struct gpio_pin {
	int pin_id;
	bool initialized;
	char mode; // A char because we don't need all four bytes of a standard integer.
};

int pin_init(struct gpio_pin* pin, int pin_id) {

	if (true == pin->initialized) {
		printf("pin_init: pin already initialized\n");
		return -1;
	}
	
	int fd = open("/sys/class/gpio/export", O_WRONLY);

	if (0 > fd) {
		perror("pin_init: open");
		return -1;
	}

	int write_res = dprintf(fd, "%d", pin_id);
	
	close(fd);

	if (0 > write_res) {
		perror("pin_init: dprintf");
		return -1;
	}

	pin->initialized = true;
	pin->pin_id = pin_id;

	return 0;
}

int pin_mode(struct gpio_pin* pin, char mode) {
	if (true != pin->initialized) {
		printf("pin_mode: pin not initialized\n");
		return -1; // Pin isn't initialized!
	}

	char fp[33];

	snprintf(fp, 32, "/sys/class/gpio/gpio%d/direction", pin->pin_id);

	int fd = open(fp, O_WRONLY);
	if (0 > fd) {
		perror("pin_mode: open");
		puts(fp);
		return -1;
	}
	
	int write_res;

	if (mode == PIN_OUT) {
		write_res = dprintf(fd, "out");
	} else {
		write_res = dprintf(fd, "in");
	}

	close(fd);

	if (0 > write_res) {
		perror("pin_mode: dprintf");
		return -1;
	}


	pin->mode = mode;

	return 0;
}

int pin_write(struct gpio_pin* pin, int value) {
	if (true != pin->initialized) {
		printf("pin_write: pin not initialized\n");
		return -1;
	}

	if (PIN_OUT != pin->mode) {
		printf("pin_write: pin not set to PIN_OUT mode\n");
		return -1;
	}

	char fp[31];

	snprintf(fp, 30, "/sys/class/gpio/gpio%d/value", pin->pin_id);

	int fd = open(fp, O_WRONLY);
	if (0 > fd) {
		perror("pin_write: open");
		puts(fp);
		return -1;
	}

	close(fd);

	int write_res = dprintf(fd, "%d", value);
	if (0 > write_res) {
		perror("pin_write: dprintf");
		return -1;
	}
	
	return 0;

}

char pin_read(struct gpio_pin* pin) {
	if (true != pin->initialized) {
		printf("pin_read: pin not initialized\n");
		return -1;
	}

	if (PIN_IN != pin->mode) {
		printf("pin_read: pin not set to PIN_IN mode\n");
		return -1;
	}

	char fp[31];
	snprintf(fp, 30, "/sys/class/gpio/gpio%d/value", pin->pin_id);

	int fd = open(fp, O_RDONLY);
	if (0 > fd) {
		perror("pin_read: open");
		puts(fp);
		return -1;
	}

	char res;
	int read_res = read(fd, &res, 1);

	close(fd);

	if (1 != read_res) {
		perror("pin_read: read");
		return -1;
	}

	return res;
}

int pin_close(struct gpio_pin* pin) {
	if (true != pin->initialized) {
		return -1;
		printf("pin_close: pin not initialized\n");
	}

	int fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (0 > fd) {
		perror("pin_close: open");
		return -1;
	}

	int write_res = dprintf(fd, "%d", pin->pin_id);
	
	close(fd);

	if (0 > write_res) {
		perror("pin_close: dprintf");
		return -1;
	}

	return 0;
}

#endif
