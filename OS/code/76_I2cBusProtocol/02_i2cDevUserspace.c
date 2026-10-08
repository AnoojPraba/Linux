#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Talking to a REAL I2C device from Linux user space through the i2c-dev
// interface (/dev/i2c-N, enable with `raspi-config` or `modprobe i2c-dev`):
//   1. open the adapter, 2. ioctl(I2C_SLAVE, addr), 3. write()/read() or
//   ioctl(I2C_RDWR) for combined transactions with a repeated START.
// Usage: ./02_i2cDevUserspace [bus] [addr_hex] [reg_hex]
//   e.g. ./02_i2cDevUserspace 1 0x68 0x75    (MPU-6050 WHO_AM_I register)
// Without an adapter (as on this machine) the program explains and exits 0.

// Read one register: write the register pointer, REPEATED START, read a byte.
// A single I2C_RDWR ioctl keeps both messages in one bus transaction so no other
// master can slip in between (unlike separate write() + read()).
static int read_reg(int fd, uint8_t addr, uint8_t reg, uint8_t *out)
{
    struct i2c_msg msgs[2] = {
        { .addr = addr, .flags = 0,        .len = 1, .buf = &reg },
        { .addr = addr, .flags = I2C_M_RD, .len = 1, .buf = out },
    };
    struct i2c_rdwr_ioctl_data tx = { .msgs = msgs, .nmsgs = 2 };
    return ioctl(fd, I2C_RDWR, &tx);
}

int main(int argc, char **argv)
{
    int bus = argc > 1 ? atoi(argv[1]) : 1;
    int addr = argc > 2 ? (int)strtol(argv[2], NULL, 0) : 0x68;
    int reg = argc > 3 ? (int)strtol(argv[3], NULL, 0) : 0x75;

    char path[32];
    snprintf(path, sizeof path, "/dev/i2c-%d", bus);
    int fd = open(path, O_RDWR);
    if (fd < 0)
    {
        printf("%s: %s\n", path, strerror(errno));
        printf("No I2C adapter node. On a Raspberry Pi enable I2C (raspi-config) and\n"
               "load i2c-dev; then `i2cdetect -y %d` lists device addresses.\n"
               "See 01_i2cBusSimulation.c for the protocol itself.\n", bus);
        return 0;
    }

    unsigned long funcs = 0;
    if (ioctl(fd, I2C_FUNCS, &funcs) == 0)
        printf("adapter supports: plain I2C %s, SMBus byte data %s\n",
               (funcs & I2C_FUNC_I2C) ? "yes" : "no",
               (funcs & I2C_FUNC_SMBUS_BYTE_DATA) ? "yes" : "no");

    uint8_t v;
    if (read_reg(fd, (uint8_t)addr, (uint8_t)reg, &v) < 0)
        printf("read of 0x%02X reg 0x%02X failed: %s (no device ACKed? wrong address/wiring/pull-ups?)\n",
               addr, reg, strerror(errno));
    else
        printf("device 0x%02X register 0x%02X = 0x%02X\n", addr, reg, v);
    close(fd);
    return 0;
}
