gcc -o Transmit Transmit.c -lwiringPi -lwiringPiDev -lm -lpthread -lrt -lcrypt
gcc -o Receive Receive.c -lwiringPi -lwiringPiDev -lm -lpthread -lrt -lcrypt
gcc -o ReceiveWireless ReceiveWireless.c -lwiringPi -lwiringPiDev -lm -lpthread -lrt -lcrypt
