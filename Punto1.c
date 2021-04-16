#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>


HANDLE *serial;
int aux, acum = 0;
#define TRANS_COUNT 17
#define EDO_COUNT 19


DCB config;
COMMTIMEOUTS touts;
char chr=0;
int dd=0, ddX=0;
int mm=0, mmX=0;
int aaaa=0, aaaaX=0;
int hh=0, hhX=0;
int nn=0, nnX=0;
int ss=0, ssX=0;

int edo=0;
int edoAnt=0;
int trans=0;

int chrTrans[TRANS_COUNT]=
					{ 0,'0','1','2','3','4','5','6','7','8','9','/',':', 13,' ', 8,27};
int mtzTrans[EDO_COUNT][TRANS_COUNT]={
					{ 0, 0 , 1 , 1 , 2 , 3 , 3 , 3 , 3 , 3 , 3 , 0 , 0 , 0 , 0 ,99,99},
					{ 1, 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 1 , 1 , 1 ,99,99},
					{ 2, 3 , 3 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 3 , 2 , 2 , 2 ,99,99},
					{ 3, 3 , 4 , 5 , 5 , 5 , 5 , 5 , 5 , 5 , 5 , 3 , 3 , 3 , 3 ,99,99},
					{ 4, 5 , 5 , 5 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 5 , 4 , 4 , 4 ,99,99},
					{ 5, 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 5 , 5 , 5 , 5 ,99,99},
					{ 6, 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 6 , 6 , 18, 6 ,99,99},
					{ 7, 8 , 8 , 8 , 8 , 8 , 8 , 8 , 8 , 8 , 8 , 7 , 7 , 18, 7 ,99,99},
					{ 8, 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 8 , 8 , 18, 8 ,99,99},
					{ 9, 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 18, 10,99,99},
					{10, 11, 11, 12, 13, 13, 13, 13, 13, 13, 13, 10, 10, 10, 10,99,99},
					{11, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 11, 13, 11, 11,99,99},
					{12, 13, 13, 13, 13, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12,99,99},
					{13, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 13, 13, 13, 13,99,99},
					{14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 15, 14, 14,99,99},
					{15, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 15, 15, 15, 15,99,99},
					{16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 16, 16, 17, 16,99,99},
					{17, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0},
					{18, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0}};


int calcTrans(char chr) {
	int trans=0;
	for (trans=TRANS_COUNT;trans>0;trans--)
		if (chr==chrTrans[trans])
			break;
	return(trans);
}

int sigEdo(int edo, int trans) {
	return(mtzTrans[edo][trans]);
}

int ejecutaEdo(int edo) {
	switch(edo) {
		case 0:
				ddX=0;
				mmX=0;
				aaaaX=0;
				hhX=hh;
				nnX=nn;
				ssX=ss;
				break;
		case 1:
		case 2:
				ddX=(chr-'0');
				printf("%c",chr);
				WriteFile(serial,&chr,1,&aux,NULL);
				break;
		case 3:
				if (chr!='/') {
					ddX*=10;
					ddX+=(chr-'0');
					printf("%c/",chr);
					WriteFile(serial,&chr,1,&aux,NULL);
					WriteFile(serial,"/",1,&aux,NULL);
				} else
					printf("/");
					WriteFile(serial,"/",1,&aux,NULL);
				break;
		case 4:
				mmX=(chr-'0');
				printf("%c",chr);
				WriteFile(serial,&chr,1,&aux,NULL);
				break;
		case 5:
				if (chr!='/') {
					mmX*=10;
					mmX+=(chr-'0');
					printf("%c/",chr);
					WriteFile(serial,&chr,1,&aux,NULL);
					WriteFile(serial,"/",1,&aux,NULL);
				} else
					printf("/");
					WriteFile(serial,"/",1,&aux,NULL);

				break;
		case 6:
				aaaaX=(chr-'0');
				printf("%c",chr);
				WriteFile(serial,&chr,1,&aux,NULL);
				break;
		case 7:
		case 8:
				aaaaX*=10;
				aaaaX+=(chr-'0');
				printf("%c",chr);
				WriteFile(serial,&chr,1,&aux,NULL);
				break;
		case 9:
				aaaaX*=10;
				aaaaX+=(chr-'0');
				printf("%c",chr);
				WriteFile(serial,&chr,1,&aux,NULL);
				break;
		case 10:
				hhX=0;
				nnX=0;
				ssX=0;
				printf(" ");
				WriteFile(serial," ",1,&aux,NULL);
				break;
		case 11:
		case 12:
				hhX=(chr-'0');
				printf("%c",chr);
				break;
		case 13:
				if (chr!=':') {
					hhX*=10;
					hhX+=(chr-'0');
					printf("%c:",chr);
					WriteFile(serial,&chr,1,&aux,NULL);
					WriteFile(serial,":",1,&aux,NULL);
				} else
					printf(":");
					WriteFile(serial,":",1,&aux,NULL);
				break;
		case 14:
				nnX=(chr-'0');
				printf("%c",chr);
				WriteFile(serial,&chr,1,&aux,NULL);
				break;
		case 15:
				if (chr!=':') {
					nnX*=10;
					nnX+=(chr-'0');
					printf("%c:",chr);
					WriteFile(serial,&chr,1,&aux,NULL);
					WriteFile(serial,":",1,&aux,NULL);
				} else
					printf(":");
					WriteFile(serial,":",1,&aux,NULL);
				break;
		case 16:
				ssX=(chr-'0');
				printf("%c",chr);
				WriteFile(serial,&chr,1,&aux,NULL);
				break;
		case 17:
				ssX*=10;
				ssX+=(chr-'0');
				printf("%c",chr);	//El estado 17 continua al estado 18 y terminan igual
				WriteFile(serial,&chr,1,&aux,NULL);
		case 18:
				dd=ddX;
				mm=mmX;
				aaaa=aaaaX;
				hh=hhX;
				nn=nnX;
				ss=ssX;
				printf("\n --- Fecha: %02d/%02d/%04d Hora: %02d:%02d:%02d ---\n",dd,mm,aaaa,hh,nn,ss);
				return(0);	//Estado aceptor, rompe la rutina y marca estado de salida
				break;
		case 99:
				printf("\n<<<Captura cancelada>>>\n");
				return(0);	//Estado aceptor, rompe la rutina y marca estado de salida
	}
	return(edo);	//Para estados no aceptores regresar el estado ejecutado
}

void main() {
	serial=CreateFile("COM4",GENERIC_WRITE|GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(serial != INVALID_HANDLE_VALUE){
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


	while (chr!='.') {		//El caracter '.' termina la ejecución del programa
		//if (kbhit()) {		//Buscando entrada del teclado
			//chr=getch();	//Tomando entrada del teclado
			ReadFile(serial,&chr,1,&aux,NULL);
			if (chr!='.') {
				trans=calcTrans(chr);	//Calcular la transición según la entrada del teclado
				if (trans) {			//Validar por transición valida (la transición 0 es inválida)
					edoAnt=edo;					//Guardar el estado anterior
					edo=sigEdo(edoAnt,trans);	//Calcular el siguiente estado
					if (edoAnt!=edo)			//Solo si hay cambio de estado hay que ...
						edo=ejecutaEdo(edo);	// ... ejecutar el nuevo estado y asignar estado de continuidad
				}
			}
		//}
	}
	printf("\nFin!!!\n");
	CloseHandle(serial);
	}
	
}