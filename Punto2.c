#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

HANDLE *serial;

unsigned char chr;
int aux;

DCB config;
COMMTIMEOUTS touts;

void main() {
	printf("Iniciando comunicaci?n ...\n");
	serial=CreateFile("COM4",GENERIC_WRITE|GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (serial!=INVALID_HANDLE_VALUE) {
		printf("... vamos bien! ... \n");

		GetCommState(serial,&config);
		config.BaudRate=CBR_9600;
		config.fParity=0;
		config.fBinary=1;
		config.StopBits=ONESTOPBIT;
		config.ByteSize=8;
		SetCommState(serial,&config);

		touts.ReadTotalTimeoutConstant=0;
		touts.ReadIntervalTimeout=0;
		touts.ReadTotalTimeoutMultiplier=0;
		SetCommTimeouts(serial,&touts);

		aux=0;
		chr=0;
		while (chr!='.') {
			ReadFile(serial,&chr,1,&aux,NULL);
			if ((chr>='a')&&(chr<='z')) {
				chr-=32;
				WriteFile(serial,&chr,1,&aux,NULL);
				WriteFile(serial,"\n\r",2,&aux,NULL);
			} else {
				if ((chr>='0')&&(chr<='9')) {
					switch (chr)
					{
					case '0':
						WriteFile(serial,"cero\n\r",6,&aux,NULL);
						break;
					case '1':
						WriteFile(serial,"uno\n\r",5,&aux,NULL);
						break;
					case '2':
						WriteFile(serial,"dos\n\r",5,&aux,NULL);
						break;
					case '3':
						WriteFile(serial,"tres\n\r",6,&aux,NULL);
						break;
					case '4':
						WriteFile(serial,"cuatro\n\r",8,&aux,NULL);
						break;
					case '5':
						WriteFile(serial,"cinco\n\r",7,&aux,NULL);
						break;
					case '6':
						WriteFile(serial,"seis\n\r",6,&aux,NULL);
						break;
					case '7':
						WriteFile(serial,"siete\n\r",7,&aux,NULL);
						break;
					case '8':
						WriteFile(serial,"ocho\n\r",6,&aux,NULL);
						break;
					case '9':
						WriteFile(serial,"nueve\n\r",7,&aux,NULL);
						break;
					default:
						break;
					}
					
				}
			}
		}
		CloseHandle(serial);
	} else {
		printf("Error: COM4 inaccesible\n");
	}
}