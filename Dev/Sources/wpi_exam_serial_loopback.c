#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdio.h>
 
int main(int argc, char ** argv)
{
        int fd = serialOpen("/dev/ttyS1", 1500000);
        char buf[255];
 
        while (1) {
                //printf("Serial Output> ");
                while (serialDataAvail(fd))
                        putchar(serialGetchar(fd));
        }
}
