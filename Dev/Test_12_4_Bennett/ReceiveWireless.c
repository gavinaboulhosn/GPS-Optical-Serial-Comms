#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdio.h>
#include <string.h>

#define MAXCHAR 50
#define MAX_VAL 9
#define MODE_LENGTH 5
#define TOLERANCE 2000

int period;

int CheckReceivedData(char expectedPacket){
    int buffer;
    int stream;
    int tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7;

    FILE *fp;
    char full_line[MAXCHAR];
    char mode[MODE_LENGTH];
    char packetReceive[MAX_VAL];
    char *filename = "/home/odroid/Desktop/Dev/Test_12_4_Bennett/test.txt";
    
    // Need to add something to read current packet
    // Then case statement for checking when to perform certain operations
    fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Could not open file %s", filename);
        return 1;
    }
    fgets(full_line, MAXCHAR, fp);
    while(fgets(full_line, MAXCHAR, fp) != NULL) {
//        strncpy(mode, full_line, 5);
        strncpy(packetReceive, full_line + 6, 9);
//        printf("%s ", mode);
          printf("Expected Packet: 10101010  ");
          printf("Received Packet:");
          printf("%c", full_line[0]);
          printf("%c\n", full_line[1]);
//        printf("%s", packetReceive);    // End line is done for us
//        printf("%s", full_line);        // End line is done for us
        delayMicroseconds(period);
    }
    fclose(fp);
    return 0;    
}
int main(void) {
    int tmp;
    char expectedPacket;
    char packet;
    char nmea;
    int read_status;

    for(int i=0; i<8; i++){
        tmp=0;
        printf("Enter Expected Packet Bit[%d", i);
        printf("]: ");
        scanf("%d", &tmp);
        if(tmp==1) expectedPacket^=(1<<i);
        printf("0x%x", expectedPacket);
        printf("\n");
    }
    printf("Enter clock period (microseconds): ");
    scanf("%d", &period);

    while(1) {
        read_status = CheckReceivedData(expectedPacket);
    }    
    
    return 0;
}
