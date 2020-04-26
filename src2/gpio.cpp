#include "gpio.h"

#include <bcm_host.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/mman.h>
#include <unistd.h>

#define GPIO_OFFSET 0x00200000
#define GPFSEL0_OFFSET 0x0000
#define GPSET0_OFFSET 0x001C
#define GPCLR0_OFFSET 0x0028
#define GPLEV0_OFFSET 0x0034

#define DEVICE_FILE "/dev/mem"

const int Gpio::PERIPHERAL_ADDRESS = bcm_host_get_peripheral_address();

Gpio::Gpio(){
  if ((m_fd = open(DEVICE_FILE, O_RDWR | O_SYNC)) < 0) {
    perror("failed to open()\n");
  }

  m_addr = (unsigned int)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, PERIPHERAL_ADDRESS + GPIO_OFFSET);
}

Gpio::~Gpio(){
  munmap((void*)m_addr, PAGE_SIZE);
  close(m_fd);
 }

 int Gpio::set_fsel(int pin, Gpio::FunctionSelect fsel) {
   //１つしかできないからなんとかしろ
 
  (*((volatile unsigned int*)((m_addr + GPFSEL0_OFFSET)))) = ((int)fsel << 3*(pin % 10));
   return 0;
 }

 int Gpio::set_pin(int pin){
  (*((volatile unsigned int*)((m_addr + GPSET0_OFFSET)))) = (1 << pin);
   return 0;
 }
	 
 int Gpio::clear_pin(int pin){
  (*((volatile unsigned int*)((m_addr + GPCLR0_OFFSET)))) = (1 << pin);
   return 0;
 }
