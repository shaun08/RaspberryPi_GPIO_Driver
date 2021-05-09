/*
 * \brief:  Testing the custom GPIO memory device driver for Raspberry Pi 4
 * \author: Shaun Prabhu
 * \date:   May 08, 2021
 */

#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define BLOCK_SIZE (4*1024) // 4KB - page size

static volatile unsigned int GPIO_BASE = 0x00200000;
static volatile unsigned int *gpio = NULL;

int main()
{
    int fd = open("/dev/RPI_GPIO_3", O_RDWR | O_SYNC | O_CLOEXEC);
    
    if(fd < 0)
    {
        printf("ERROR opening RPI_GPIO device\n");
        return -1;
    }
    
    gpio =(uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ|PROT_WRITE,
                           MAP_SHARED, fd, GPIO_BASE);
    
    if(gpio == MAP_FAILED)
    {
        printf("ERROR mmaping into RPI_GPIO device\n");
        return -2;
    }
    
    close(fd);
    
    return 0;
}
