#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>  // Add this include for close()

int main() {
    int i2c_bus;
    int n;
    // open the bus
    i2c_bus = open("/dev/i2c-3", O_RDWR); // open I2C bus 3, in read write mode. File handle will be i2c_bus.

    if (i2c_bus < 0) {
        perror("Error opening i2c_bus");
        return 1;
    }

    // set slave address
    if (ioctl(i2c_bus, I2C_SLAVE, 0x70) < 0) {
        perror("Error setting slave address");
        close(i2c_bus); // Use close() instead of pclose()
        return 1;
    }

    // access the bus and write
    uint8_t data[1] = {0x58};    //reg
    n = write(i2c_bus, data, sizeof(data));
    if (n != sizeof(data)) {
        perror("Error writing to i2c bus");
        close(i2c_bus);
        return 1;
    }

    // Read value back
    uint8_t read_data[1];
    n = read(i2c_bus, read_data, sizeof(read_data));
    if (n != sizeof(read_data)) {
        perror("Error reading from i2c bus");
        close(i2c_bus);
        return 1;
    }
    
    printf("\nReading ..\n");
    printf("%d\n", read_data[0]);

    close(i2c_bus);
    return 0;
}

