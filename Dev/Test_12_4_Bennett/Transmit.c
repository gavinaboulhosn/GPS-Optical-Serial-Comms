#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdio.h>

#define UNCORRECTABLE	0xFF
#define ERROR_IN_PARITY	0xFE
#define NO_ERROR		0x00

int period;

// Given a byte to transmit, this returns the parity as a nibble
char DL_HammingCalculateParity128(char value)
{
	// Exclusive OR is associative and commutative,
	// so order of operations and values does not matter.
	char parity;
	if ( ( value & 1 ) != 0 ) parity = 0x3;
	else parity = 0x0;
	if ( ( value & 2 ) != 0 ) parity ^= 0x5;
	if ( ( value & 4 ) != 0 ) parity ^= 0x6;
	if ( ( value & 8 ) != 0 ) parity ^= 0x7;
	if ( ( value & 16 ) != 0 ) parity ^= 0x9;
	if ( ( value & 32 ) != 0 ) parity ^= 0xA;
	if ( ( value & 64 ) != 0 ) parity ^= 0xB;
	if ( ( value & 128 ) != 0 ) parity ^= 0xC;
    
    //printf("\nParity is: 0x%x", parity);
	return parity;
}

// Private method
// Give a pointer to a received byte,
// and given a nibble difference in parity (parity ^ calculated parity)
// this will correct the received byte value if possible.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 3 means corrections not possible
static char DL_HammingCorrect128Syndrome(char* value, char syndrome)
{
	char _hammingCorrect128Syndrome[16] ={NO_ERROR,ERROR_IN_PARITY,ERROR_IN_PARITY,0x01,ERROR_IN_PARITY,0x02,0x04,0x08,ERROR_IN_PARITY,0x10,0x20,0x40,0x80,UNCORRECTABLE,UNCORRECTABLE,UNCORRECTABLE};
	// Using only the lower nibble (& 0x0F), look up the bit
	// to correct in a table
	char correction = _hammingCorrect128Syndrome[syndrome & 0x0F];
	if (correction != NO_ERROR)
	{
		if (correction == UNCORRECTABLE || value == NULL) return 3; // Non-recoverable error
		else
		{
			if ( correction != ERROR_IN_PARITY) *value ^= correction;
			return 1; // 1-bit recoverable error;
		}
	}
	return 0; // No errors
}

// Given a pointer to a received byte and the received parity (as a lower nibble),
// this calculates what the parity should be and fixes the recevied value if needed.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 3 means corrections not possible
char DL_HammingCorrect128(char* value, char parity)
{
	char syndrome;
	if (value == NULL) return 3; // Non-recoverable error
	syndrome = DL_HammingCalculateParity128(*value) ^ parity;
	if (syndrome != 0) return DL_HammingCorrect128Syndrome(value, syndrome);
	return 0; // No errors
}

int appendParity(char in){ 
    int inCopy=in;
    int out=0;
    char parity = DL_HammingCalculateParity128(inCopy);
    int parityCopy=parity;
    out^=inCopy;
    out^=(parityCopy<<8);
    //printf("\nComplete Packet is: 0x%x", out);
    return out;
}

void transmitLED(int in){
    int buffer;
    int stream;
    printf("\nPacket sending: ");
    //digitalWrite(0,0);
    delayMicroseconds(4*period);
    for (int i=0; i<8; i++)
    {
        buffer=in;
        stream=((buffer>>i)&1);
        printf("%d",stream);
        digitalWrite(0, stream);
        delayMicroseconds(period);
    }
    //digitalWrite(0,1);
    //delayMicroseconds(4*period);
    //digitalWrite(0,0);
    //printf("\nPacket Sent");
}

char receiveGPS(int fd){ //Return 1 byte from NMEA message, NULL if none
	char out='\0';
	while(!(serialDataAvail(fd))){
            delayMicroseconds(10);
        }
		if(serialDataAvail(fd)) out=serialGetchar(fd);
	return out;
}

void transmitSerial(int in, int fd){ //Transmit 
    int inCopy=in;
    char buf[2]={inCopy, inCopy>>8};
        serialPutchar(fd, buf[0]);
        serialPutchar(fd, buf[1]);
}

int receiveSerial(int fd){ //Return 1 byte from Serial message, NULL if none
	char in='\0';
        while(!(serialDataAvail(fd))){
            delayMicroseconds(10);
        }
		if(serialDataAvail(fd)) in=serialGetchar(fd);
	return in;
}

int main(void)
{
	//Transmit Code
	char nmea;
	int packet;
    char packetIn;
	int tmp;
	
    wiringPiSetup();
    pinMode(0, OUTPUT);
    digitalWrite(0, 0);

    int fdGPS = serialOpen("/dev/ttyUSB0", 4800);
    int fdSerial = serialOpen("/dev/ttyS1", 4800);
	
    //LED Transmit
    for(int i=0; i<8; i++){
        tmp=0;
        printf("Enter Packet Bit[%d", i);
        printf("]: ");
        scanf("%d", &tmp);
        if(tmp==1) packetIn^=(1<<i);
        printf("0x%x", packetIn);
        printf("\n");
    }
    printf("Enter clock period (microseconds): ");
    scanf("%d", &period);
    
	while(1){
	transmitLED(packetIn);
	}
	
    /*
    while(1){
	    nmea=receiveGPS(fdGPS);
        printf("%c", nmea);
    	packet=appendParity(nmea);
	    transmitSerial(packet, fdSerial);
    }
	*/
	//Receive Code
	/*
	char packetIn;
	int count=0;
    char m;
    char p;
	int errors=0;
	int start=0;

    wiringPiSetup();
    int fd = serialOpen("/dev/ttyS1", 4800);
	printf("Waiting to receive.\n");

	while(1){
		packetIn=receiveSerial(fd);
        if(packetIn=='$'){
            start=1;
            count=0;
            printf("Packet Errors: %d  Message: ", errors);
        }
        else count++;
        if(start&&count%2==0){
            m=packetIn;
            printf("%c", m);
        }
        else if(start) {
            p=packetIn;
            errors+=DL_HammingCorrect128(&m, p);
        }
	}
	*/
    return 0;
}


