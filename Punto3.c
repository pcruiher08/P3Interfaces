#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

HANDLE *serial;
unsigned char TxDBuffer[10];
unsigned char RxDBuffer[10];
int aux;
int acum=0;

DCB config;
COMMTIMEOUTS touts;


#define TRANS_COUNT 9
#define EDO_COUNT 15


char chr = 0;
float acumDig1 = 0;
float acumDig2 = 0;
float multiplicador = 1.0;
float resultado = 0.0;
float valFracc = 0.0;
int valEntero = 0;
int tempEntero = 0;
int edo = 0;
int edoAnt = 0;
int trans = 0;
int decimalDig1 = 0;
int decimalDig2 = 0;
int decimal = 0;
char result[10];

enum Oper { Suma, Resta, Mult, Div };
enum Oper oper;

//0-->Inválida
//6-->Digito
//7-->Operador
int chrTrans[TRANS_COUNT] =
                   { 0,'(',')','=',  8, 27, 6 ,  7, '.'};
int mtzTrans[EDO_COUNT][TRANS_COUNT] = {
                    { 0, 1 , 0 , 0 , 0 , 0 , 0 , 0 ,  0},
                    { 1, 1 , 1 , 1 , 99, 99, 2 , 1 ,  1},
                    { 2, 2 , 2 , 2 , 99, 99, 3 , 4 ,  9},
                    { 3, 2 , 2 , 2 , 99, 99, 2 , 2 ,  2}, 
                    { 4, 4 , 4 , 4 , 99, 99, 5 , 4 ,  4},
                    { 5, 5 , 7 , 5 , 99, 99, 6 , 5 , 12},
                    { 6, 5 , 5 , 5 , 99, 99, 5 , 5 ,  5},
                    { 7, 7 , 7 , 8 , 99, 99, 7 , 7 ,  7},
                    { 8, 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  0},
                    { 9, 9 , 9 , 9 , 99, 99, 10, 9 ,  9},
                    {10, 10, 10, 10, 99, 99, 11, 4 , 10},
                    {11, 10, 10, 10, 99, 99, 10, 10, 10},
                    {12, 12, 12, 12, 99, 99, 13, 12, 12},
                    {13, 13, 7 , 13, 99, 99, 14, 13, 13},
                    {14, 13, 13, 13, 99, 99, 13, 13, 13}};



int calcTrans(char chr) {
    int trans = 0;
    if ((chr >= '0') && (chr <= '9')) { //Digito
        return(6);
    }else if (chr == '.') {
        return(8);
    }

    switch (chr) {
    case'+':
    case'-':
    case'*':
    case'/':
        return(7);
    }
    for (trans = 5; trans > 0; trans--) {
        if (chr == chrTrans[trans]) {
            break;
        }
    }
    return(trans);
}

int sigEdo(int edo, int trans) {
    return(mtzTrans[edo][trans]);
}

int ejecutaEdo(int edo) {
    static int i = 0;
    static int negativoFlag = 0;
    static int puntodecimal = 0;
    static int digitosCont = 0;
    valFracc = 0.0;
    valEntero = 0;
    tempEntero = 0;
    resultado = 0;
    decimal = 0;

    switch (edo) {
        case 0:
            break;
        case 1:
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            decimalDig1 = 0;
            acumDig1 = 0;
            decimalDig2 = 0;
            acumDig2 = 0; 
            break;
        case 2:
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            acumDig1 *= 10;
            acumDig1 += (chr - '0');
            break;
        case 3:
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            acumDig1 *= 10;
            acumDig1 += (chr - '0');
            return(2);
        case 4:
            switch (chr) {
            case'+':
                oper = Suma;
                break;
            case'-':
                oper = Resta;
                break;
            case'*':
                oper = Mult;
                break;
            case'/':
                oper = Div;
                break;
            }
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);          
            break;
        case 5:
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            acumDig2 *= (float)10.0;
            acumDig2 = (chr - '0');
            break;
        case 6:
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            acumDig2 *= (float)10.0;
            acumDig2 += (chr - '0');
            return(5);
        case 7: 
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            break;
        case 8:
            switch (oper) {
            case Suma:
                resultado = acumDig1 + acumDig2;
				WriteFile(serial,"=",1,&aux,NULL);
				sprintf(result,"%f",resultado);
				WriteFile(serial,&result,8,&aux,NULL);
				WriteFile(serial,"\n",1,&aux,NULL);
				WriteFile(serial,"\r",1,&aux,NULL);
				printf("= %f \n",resultado);
                break;
            case Resta:
                resultado = acumDig1 - acumDig2;
				WriteFile(serial,"=",1,&aux,NULL);
				sprintf(result,"%f",resultado);
				WriteFile(serial,&result,8,&aux,NULL);
				WriteFile(serial,"\n",1,&aux,NULL);
				WriteFile(serial,"\r",1,&aux,NULL);
				printf("= %f \n",resultado);
                break;
            case Mult:
                decimalDig1 += decimalDig2;
                resultado = acumDig1 * acumDig2;
				WriteFile(serial,"=",1,&aux,NULL);
				sprintf(result,"%f",resultado);
				WriteFile(serial,&result,8,&aux,NULL);
				WriteFile(serial,"\n",1,&aux,NULL);
				WriteFile(serial,"\r",1,&aux,NULL);
				printf("= %f \n",resultado);
                break;
            case Div:
                if (acumDig2) {
                    decimalDig1 = 6;
                    resultado = acumDig1 / acumDig2;
					
					WriteFile(serial,"=",1,&aux,NULL);
					sprintf(result,"%f",resultado);
					WriteFile(serial,&result,8,&aux,NULL);
					WriteFile(serial,"\n",1,&aux,NULL);
					WriteFile(serial,"\r",1,&aux,NULL);
					printf("= %f \n",resultado);
                }else{
                    resultado = -1;
					WriteFile(serial,"=",1,&aux,NULL);
					sprintf(result,"%f",resultado);
					WriteFile(serial,&result,8,&aux,NULL);
					WriteFile(serial,"\n",1,&aux,NULL);
					WriteFile(serial,"\r",1,&aux,NULL);
					printf("= %f\n",resultado);
				}
                break;
            }

            if (decimalDig1 < decimalDig2) {
                decimal = decimalDig2;
            }else{
                decimal = decimalDig1;
            }

            for (i = 1; i <= decimal; i++) {
                valFracc *= (float)10.0;
                tempEntero = valFracc;
                valFracc -= (float)tempEntero;

            }
            return(0);

        case 9:
            multiplicador = 1.0;
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            break;
        case 10:
            decimalDig1++;
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            multiplicador /= 10.0;
            acumDig1 += ((chr - (char)'0') * multiplicador);
            break;
        case 11:
            decimalDig1++;
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            multiplicador /= 10.0;
            acumDig1 += ((chr - (char)'0') * multiplicador);
            return(10);

        case 12:
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            multiplicador = 1.0;
            break;
        case 13:
            decimalDig2++;
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            multiplicador /= 10.0;
            acumDig2 += ((chr - (char)'0') * multiplicador);
            break;
        case 14:
            decimalDig2++;
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("%c",chr);
            multiplicador /= 10.0;
            acumDig2 += ((chr - (char)'0') * multiplicador);
            return(13);
        case 99:
			WriteFile(serial,&chr,1,&aux,NULL);
            printf("\n<<<Captura cancelada>>>\n");
            return(0);
    }
    return(edo);
}

void main() {
	printf("Iniciando comunicación ...\n");
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
		while (chr!='q') {
			ReadFile(serial,&chr,1,&aux,NULL);
			if (chr!='q') {
				trans=calcTrans(chr);
				if (trans) {
					edoAnt=edo;
					edo=sigEdo(edoAnt,trans);
					if (edoAnt!=edo)
						edo=ejecutaEdo(edo);
				}
			}
		}
	}
	
	CloseHandle(serial);
	printf("\nFin!!!\n");
}