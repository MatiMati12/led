/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fatfs_sd.h"
#include "string.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */





#define N 4000
uint8_t tablicaGlowna[8][4000];
FATFS fs;
FIL fil;
FRESULT fresult;
char buffer[N];
char tekst[N];
char buffer2[100];
int ilosc_led = 0;
int szybkosc = 0;
int mode=1;
int dlugoscZnaku=0;
int kontrast=0;
////////////////////////////////////
////////////////////////////////////
///////////////////////////////////


/*int zwroc_dlugosc_pliku(){
	int counter=0;
	for(int i=0;i<N;i++){
	if(tekst[i]!=0){
		counter++;
	}
	}

	return counter-1;
}*/

void przepiszPlik(int dlugosc){
   for(int i=0;i<dlugosc;i++){
   tekst[i]=buffer[i];
   }

}
//////////////////////////////////////////////////////
void otworzPlikZTekstem(){
	  fresult = f_mount(&fs, "", 0);
	  fresult = f_open(&fil, "tekst.txt", FA_READ);
	  f_gets(buffer,4096, &fil);
	  f_close(&fil);

	 przepiszPlik(strlen(buffer));
}
//////////////////////////////////////////////////////////
bool otworzPlikZParametrami(){
	  bool control=true;
	  fresult = f_mount(&fs, "", 0);
	  fresult = f_open(&fil, "config.txt", FA_READ);
	  f_gets(buffer2,4096, &fil);
	  f_close(&fil);
	  szybkosc = (int)buffer2[14]-48;
	  if(szybkosc <1 || szybkosc>9){strcpy(tekst,"Blad pliku konfiguracyjnego, ustaw szybkosc przewijania z przedzialu <1,9>"); control=false;}
	  mode=(int)buffer2[5]-48;
	  if(mode<0 || mode>1){strcpy(tekst, "Blad pliku konfiguracyjnego, ustaw mode na 0 lub 1"); control=false;}

	  ilosc_led= ((int)buffer2[29]-48)*10+(int)buffer2[30]-48;
	  if(ilosc_led<1 ||ilosc_led>64){strcpy(tekst,"Blad pliku konfiguracyjnego, ustaw ilosc led z przedzialu (1,64>"); control=false;}

	  kontrast= ((int)buffer2[42]-48)*10+(int)buffer2[43]-48;
	  	  if(kontrast<0 ||kontrast>15){strcpy(tekst,"Blad pliku konfiguracyjnego, ustaw kontrast z przedzialu <0,15>"); control=false;}
return control;
}
//////////////////////////////////////////////////////
void wypelnijGlownaTablice(){
for(int i=0; i<N; i++){
	for(int j=0; j<8; j++){
	tablicaGlowna[j][i]= 2;
	}
}
}
/////////////////////////////////////////////////////
int dlugoscTablicyGlownej(){
	int counter=0;

	for(int i=0;i<N; i++){
		if(tablicaGlowna[7][i]!=2){counter++;}
	}
	return counter;
	}

///////////////////////////////////////////////////////
void dodajMacierzLitery(uint8_t macierz[8][dlugoscZnaku]){
int aktualnaDlugosc = dlugoscTablicyGlownej();  //dlugosc tablicy głownej przed dodaniem kolejnego znaku
for(int j=0;j<dlugoscZnaku;j++){
	for(int k=0; k<8; k++){
		int len=aktualnaDlugosc+j;
	tablicaGlowna[k][len]=macierz[k][j];
	}
}
}
///////////////////////////////////////////////////

void KonwertujTekst(int dlugosc)
{
	uint8_t literaProcent[8][7]={
			{0,1,0,0,0,0,0},
			{1,0,1,0,0,1,0},
			{0,1,0,0,1,0,0},
			{0,0,0,1,0,0,0},
			{0,0,1,0,0,0,0},
			{0,1,0,0,1,0,0},
			{1,0,0,1,0,1,0},
			{0,0,0,0,1,0,0}
	};

	uint8_t literaKropka[8][3]={
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{1,1,0},
			{1,1,0}
	};

	uint8_t literaKreskaPionowa[8][2]={
			{1,0},
			{1,0},
			{1,0},
			{1,0},
			{1,0},
			{1,0},
			{1,0},
			{1,0}
	};

	uint8_t literaWykrzyknik[8][2]={
			{1,0},
			{1,0},
			{1,0},
			{1,0},
			{1,0},
			{0,0},
			{1,0},
			{0,0}
	};

	uint8_t literaZnakZapytania[8][6]={
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{0,0,0,0,1,0},
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,0,0,0,0},
			{0,0,1,0,0,0}
	};

	uint8_t literaPrzecinek[8][3]={
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{1,0,0},
			{0,1,0},
			{0,1,0}
	};


	uint8_t literaDwukropek[8][3]={
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{1,1,0},
			{1,1,0},
			{0,0,0},
			{1,1,0},
			{1,1,0}
	};


	uint8_t literaApostrof[8][3]={
			{0,1,0},
			{1,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0}
	};

	uint8_t literaSlash[8][6]={
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{0,0,0,1,0,0},
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,1,0,0,0,0},
			{0,1,0,0,0,0},
			{1,0,0,0,0,0}
	};

	uint8_t literaBackSlash[8][6]={
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{0,1,0,0,0,0},
			{0,1,0,0,0,0},
			{0,0,1,0,0,0},
			{0,0,0,1,0,0},
			{0,0,0,1,0,0},
			{0,0,0,0,1,0}
	};

	uint8_t literaRownaSie[8][6]={
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{1,1,1,1,1,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{1,1,1,1,1,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0}
	};

	uint8_t literaPlus[8][6]={
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{1,1,1,1,1,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,0,0,0,0}
	};
	uint8_t literaMinus[8][6]={
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{1,1,1,1,1,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0}
	};
	uint8_t literaPodloga[8][6]={
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0},
			{1,1,1,1,1,0}
	};


	uint8_t literaWiekszeOd[8][6]={
			{0,0,0,0,0,0},
			{1,0,0,0,0,0},
			{0,1,1,0,0,0},
			{0,0,0,1,1,0},
			{0,1,1,0,0,0},
			{1,0,0,0,0,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0}
	};

	uint8_t literaMniejszeOd[8][6]={
			{0,0,0,0,0,0},
			{0,0,0,0,1,0},
			{0,0,1,1,0,0},
			{1,1,0,0,0,0},
			{0,0,1,1,0,0},
			{0,0,0,0,1,0},
			{0,0,0,0,0,0},
			{0,0,0,0,0,0}
	};

	uint8_t literaNawiasLewy[8][4]={
			{0,0,1,0},
			{0,1,0,0},
			{1,0,0,0},
			{1,0,0,0},
			{1,0,0,0},
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0}
	};

	uint8_t literaNawiasPrawy[8][4]={
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,1,0,0},
			{1,0,0,0}
	};

	uint8_t litera0[8][6]={
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,1,1,0},
			{1,0,1,0,1,0},
			{1,1,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,0,0}
	};

	uint8_t litera1[8][6]={
			{0,0,1,0,0,0},
			{0,1,1,0,0,0},
			{1,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0}
	};

	uint8_t litera2[8][6]={
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,1,0,0,0,0},
			{0,1,0,0,0,0},
			{1,0,0,0,0,0},
			{1,1,1,1,1,0}
	};

	uint8_t litera3[8][6]={
			{1,1,1,1,0,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{1,1,1,1,0,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{1,1,1,1,0,0}
	};

	uint8_t litera4[8][6]={
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,1,0,0,0,0},
			{1,0,0,1,0,0},
			{1,1,1,1,1,0},
			{0,0,0,1,0,0},
			{0,0,0,1,0,0},
			{0,0,0,1,0,0}
	};

	uint8_t litera5[8][6]={
			{1,1,1,1,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,1,1,1,0,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,0,0}
	};

	uint8_t litera6[8][6]={
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,0,0},
			{1,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,0,0}
	};

	uint8_t litera7[8][6]={
			{1,1,1,1,1,1},
			{1,0,0,0,0,1},
			{0,0,0,0,1,0},
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,1,0,0,0,0},
			{0,1,0,0,0,0}
	};

	uint8_t litera8[8][6]={
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,0,0}
	};

	uint8_t litera9[8][6]={
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,1,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,0,0}
	};


	uint8_t literaA[8][6]={
		{0,0,1,0,0,0},
		{0,1,0,1,0,0},
		{1,0,0,0,1,0},
		{1,0,0,0,1,0},
		{1,1,1,1,1,0},
		{1,0,0,0,1,0},
		{1,0,0,0,1,0},
		{1,0,0,0,1,0}
	};

	uint8_t literaAl[8][6]={
		{0,0,1,0,0,0},
		{0,1,0,1,0,0},
		{1,0,0,0,1,0},
		{1,1,1,1,1,0},
		{1,0,0,0,1,0},
		{1,0,0,1,1,1},
		{1,0,1,0,1,0},
		{0,0,1,0,0,0}
	};

	uint8_t literaB[8][6]={
			{1,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,1,1,1,0,0}
	};

	uint8_t literaC[8][6]={
			{0,1,1,1,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{0,1,1,1,1,0}
	};

	uint8_t literaCi[8][6]={
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,1,1,1,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{0,1,1,1,1,0}
	};

	uint8_t literaD[8][6]={
			{1,1,1,0,0,0},
			{1,0,0,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,1,0,0},
			{1,1,1,0,0,0}
	};

	uint8_t literaE[8][6]={
			{1,1,1,1,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,1,1,1,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,1,1,1,1,0}
	};

	uint8_t literaEE[8][6]={
			{1,1,1,1,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,1,1,1,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,1,0},
			{1,1,1,1,1,0},
			{0,0,1,0,0,0}
	};

	uint8_t literaF[8][6]={
			{1,1,1,1,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,1,1,1,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0}
	};

	uint8_t literaG[8][6]={
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,1,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,1,0}
	};

	uint8_t literaH[8][6]={
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,1,1,1,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0}
	};

	uint8_t literaI[8][4]={
			{1,1,1,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{1,1,1,0}
	};

	uint8_t literaJ[8][6]={
			{0,1,1,1,1,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,0,0}
	};

	uint8_t literaK[8][6]={
			{1,0,0,0,1,0},
			{1,0,0,1,0,0},
			{1,0,1,0,0,0},
			{1,1,0,0,0,0},
			{1,0,1,0,0,0},
			{1,0,0,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0}
	};

	uint8_t literaL[8][6]={
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,1,1,1,1,0}
	};

	uint8_t literaEl[8][6]={
			{0,1,0,0,0,0},
			{0,1,0,0,0,0},
			{0,1,0,0,0,0},
			{0,1,1,0,0,0},
			{0,1,0,0,0,0},
			{1,1,0,0,0,0},
			{0,1,0,0,0,0},
			{0,1,1,1,1,0}
	};

	uint8_t literaM[8][6]={
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,1,0,1,1,0},
			{1,1,0,1,1,0},
			{1,0,1,0,1,0},
			{1,0,1,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0}
	};
	uint8_t literaN[8][6]={
			{1,0,0,0,1,0},
			{1,1,0,0,1,0},
			{1,1,0,0,1,0},
			{1,0,1,0,1,0},
			{1,0,1,0,1,0},
			{1,0,0,1,1,0},
			{1,0,0,1,1,0},
			{1,0,0,0,1,0}
	};

	uint8_t literaNi[8][6]={
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,0,0,0,0,0},
			{1,0,0,0,1,0},
			{1,1,0,0,1,0},
			{1,0,1,0,1,0},
			{1,0,0,1,1,0},
			{1,0,0,0,1,0}
	};

	uint8_t literaO[8][6]={
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,0,0}
	};

	uint8_t literaOu[8][6]={
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,0,0}
	};

	uint8_t literaP[8][6]={
			{1,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,1,1,1,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0}
	};

	uint8_t literaQ[8][6]={
			{0,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,1,0,0},
			{0,1,1,1,1,0},
			{0,0,0,0,1,0}
	};

	uint8_t literaR[8][6]={
			{1,1,1,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,1,1,1,0,0},
			{1,0,1,0,0,0},
			{1,0,0,1,0,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0}
	};

	uint8_t literaS[8][6]={
			{0,1,1,1,1,0},
			{1,0,0,0,0,0},
			{1,0,0,0,0,0},
			{0,1,1,1,0,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{1,1,1,1,0,0}
	};

	uint8_t literaSi[8][6]={
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,1,1,1,0,0},
			{1,0,0,0,0,0},
			{0,1,1,1,1,0},
			{0,0,0,0,1,0},
			{0,0,0,0,1,0},
			{1,1,1,1,0,0}
	};

	uint8_t literaT[8][6]={
			{1,1,1,1,1,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0}
	};

	uint8_t literaU[8][6]={
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,1,1,0,0}
	};

	uint8_t literaV[8][6]={
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,0,1,0,0},
			{0,1,0,1,0,0},
			{0,1,0,1,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0}
	};

	uint8_t literaW[8][6]={
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{1,0,1,0,1,0},
			{1,0,1,0,1,0},
			{1,1,0,1,1,0},
			{1,1,0,1,1,0},
			{1,0,0,0,1,0}
	};

	uint8_t literaX[8][6]={
			{1,0,0,0,1,0},
			{0,1,0,1,0,0},
			{0,1,0,1,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,1,0,1,0,0},
			{0,1,0,1,0,0},
			{1,0,0,0,1,0}
	};

	uint8_t literaY[8][6]={
			{1,0,0,0,1,0},
			{1,0,0,0,1,0},
			{0,1,0,1,0,0},
			{0,1,0,1,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0}
	};

	uint8_t literaZ[8][6]={
			{1,1,1,1,1,0},
			{0,0,0,1,0,0},
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,0,1,0,0,0},
			{0,1,0,0,0,0},
			{0,1,0,0,0,0},
			{1,1,1,1,1,0}
	};
	uint8_t literaZi[8][6]={
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,1,0,0,0,0},
			{1,1,1,1,1,0},
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,1,0,0,0,0},
			{1,1,1,1,1,0}
	};

	uint8_t literaZy[8][6]={
			{0,0,0,0,0,0},
			{0,0,1,0,0,0},
			{0,0,0,0,0,0},
			{1,1,1,1,1,0},
			{0,0,0,1,0,0},
			{0,0,1,0,0,0},
			{0,1,0,0,0,0},
			{1,1,1,1,1,0}
	};

	uint8_t literaSpacja[8][4]={
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0}
	};

for(int i=0; i<dlugosc+1; i++)
{

	if(tekst[i]=='a' || tekst[i]=='A'){dlugoscZnaku=6;dodajMacierzLitery(literaA);}
	else if(tekst[i]=='b' || tekst[i]=='B'){dlugoscZnaku=6;dodajMacierzLitery(literaB);}
	else if(tekst[i]==196 && (tekst[i+1]==132 || tekst[i+1]==133 ) ){dlugoscZnaku=6;dodajMacierzLitery(literaAl);i++;}
	else if(tekst[i]==196 && (tekst[i+1]==152 || tekst[i+1]==153 ) ){dlugoscZnaku=6;dodajMacierzLitery(literaEE);i++;}
	else if(tekst[i]==197 && (tekst[i+1]==129 || tekst[i+1]==130 ) ){dlugoscZnaku=6;dodajMacierzLitery(literaEl);i++;}
	else if(tekst[i]==197 && (tekst[i+1]==131 || tekst[i+1]==132 ) ){dlugoscZnaku=6;dodajMacierzLitery(literaNi);i++;}
	else if(tekst[i]==195 && (tekst[i+1]==147 || tekst[i+1]==179 ) ){dlugoscZnaku=6;dodajMacierzLitery(literaOu);i++;}
	else if(tekst[i]==197 && (tekst[i+1]==154 || tekst[i+1]==155 ) ){dlugoscZnaku=6;dodajMacierzLitery(literaSi);i++;}
	else if(tekst[i]==197 && (tekst[i+1]==187 || tekst[i+1]==188 ) ){dlugoscZnaku=6;dodajMacierzLitery(literaZy);i++;}
	else if(tekst[i]==197 && (tekst[i+1]==185 || tekst[i+1]==186 ) ){dlugoscZnaku=6;dodajMacierzLitery(literaZi);i++;}
	else if(tekst[i]==196 && (tekst[i+1]==135 || tekst[i+1]==134 ) ){dlugoscZnaku=6;dodajMacierzLitery(literaCi);i++;}
	else if(tekst[i]=='c' || tekst[i]=='C'){dlugoscZnaku=6;dodajMacierzLitery(literaC);}
	else if(tekst[i]=='d' || tekst[i]=='D'){dlugoscZnaku=6;dodajMacierzLitery(literaD);}
	else if(tekst[i]=='e' || tekst[i]=='E'){dlugoscZnaku=6;dodajMacierzLitery(literaE);}
	else if(tekst[i]=='f' || tekst[i]=='F'){dlugoscZnaku=6;dodajMacierzLitery(literaF);}
	else if(tekst[i]=='g' || tekst[i]=='G'){dlugoscZnaku=6;dodajMacierzLitery(literaG);}
	else if(tekst[i]=='h' || tekst[i]=='H'){dlugoscZnaku=6;dodajMacierzLitery(literaH);}
	else if(tekst[i]=='i' || tekst[i]=='I'){dlugoscZnaku=4;dodajMacierzLitery(literaI);}
	else if(tekst[i]=='j' || tekst[i]=='J'){dlugoscZnaku=6;dodajMacierzLitery(literaJ);}
	else if(tekst[i]=='k' || tekst[i]=='K'){dlugoscZnaku=6;dodajMacierzLitery(literaK);}
	else if(tekst[i]=='l' || tekst[i]=='L'){dlugoscZnaku=6;dodajMacierzLitery(literaL);}
	else if(tekst[i]=='m' || tekst[i]=='M'){dlugoscZnaku=6;dodajMacierzLitery(literaM);}
	else if(tekst[i]=='n' || tekst[i]=='N'){dlugoscZnaku=6;dodajMacierzLitery(literaN);}
	else if(tekst[i]=='o' || tekst[i]=='O'){dlugoscZnaku=6;dodajMacierzLitery(literaO);}
	else if(tekst[i]=='p' || tekst[i]=='P'){dlugoscZnaku=6;dodajMacierzLitery(literaP);}
	else if(tekst[i]=='q' || tekst[i]=='Q'){dlugoscZnaku=6;dodajMacierzLitery(literaQ);}
	else if(tekst[i]=='r' || tekst[i]=='R'){dlugoscZnaku=6;dodajMacierzLitery(literaR);}
	else if(tekst[i]=='s' || tekst[i]=='S'){dlugoscZnaku=6;dodajMacierzLitery(literaS);}
	else if(tekst[i]=='t' || tekst[i]=='T'){dlugoscZnaku=6;dodajMacierzLitery(literaT);}
	else if(tekst[i]=='u' || tekst[i]=='U'){dlugoscZnaku=6;dodajMacierzLitery(literaU);}
	else if(tekst[i]=='v' || tekst[i]=='V'){dlugoscZnaku=6;dodajMacierzLitery(literaV);}
	else if(tekst[i]=='w' || tekst[i]=='W'){dlugoscZnaku=6;dodajMacierzLitery(literaW);}
	else if(tekst[i]=='x' || tekst[i]=='X'){dlugoscZnaku=6;dodajMacierzLitery(literaX);}
	else if(tekst[i]=='y' || tekst[i]=='Y'){dlugoscZnaku=6;dodajMacierzLitery(literaY);}
	else if(tekst[i]=='z' || tekst[i]=='Z'){dlugoscZnaku=6;dodajMacierzLitery(literaZ);}
	else if(tekst[i]==' '){dlugoscZnaku=4;dodajMacierzLitery(literaSpacja);}
	else if(tekst[i]=='0'){dlugoscZnaku=6;dodajMacierzLitery(litera0);}
	else if(tekst[i]=='1'){dlugoscZnaku=6;dodajMacierzLitery(litera1);}
	else if(tekst[i]=='2'){dlugoscZnaku=6;dodajMacierzLitery(litera2);}
	else if(tekst[i]=='3'){dlugoscZnaku=6;dodajMacierzLitery(litera3);}
	else if(tekst[i]=='4'){dlugoscZnaku=6;dodajMacierzLitery(litera4);}
	else if(tekst[i]=='5'){dlugoscZnaku=6;dodajMacierzLitery(litera5);}
	else if(tekst[i]=='6'){dlugoscZnaku=6;dodajMacierzLitery(litera6);}
	else if(tekst[i]=='7'){dlugoscZnaku=6;dodajMacierzLitery(litera7);}
	else if(tekst[i]=='8'){dlugoscZnaku=6;dodajMacierzLitery(litera8);}
	else if(tekst[i]=='9'){dlugoscZnaku=6;dodajMacierzLitery(litera9);}
	else if(tekst[i]=='%'){dlugoscZnaku=7;dodajMacierzLitery(literaProcent);}
	else if(tekst[i]=='?'){dlugoscZnaku=6;dodajMacierzLitery(literaZnakZapytania);}
	else if(tekst[i]=='/'){dlugoscZnaku=6;dodajMacierzLitery(literaSlash);}
	else if(tekst[i]=='>'){dlugoscZnaku=6;dodajMacierzLitery(literaWiekszeOd);}
	else if(tekst[i]=='<'){dlugoscZnaku=6;dodajMacierzLitery(literaMniejszeOd);}
	else if(tekst[i]=='='){dlugoscZnaku=6;dodajMacierzLitery(literaRownaSie);}
	else if(tekst[i]=='_'){dlugoscZnaku=6;dodajMacierzLitery(literaPodloga);}
	else if(tekst[i]=='-'){dlugoscZnaku=6;dodajMacierzLitery(literaMinus);}
	else if(tekst[i]=='+'){dlugoscZnaku=6;dodajMacierzLitery(literaPlus);}
	else if(tekst[i]=='!'){dlugoscZnaku=2;dodajMacierzLitery(literaWykrzyknik);}
	else if(tekst[i]=='|'){dlugoscZnaku=2;dodajMacierzLitery(literaKreskaPionowa);}
	else if(tekst[i]=='('){dlugoscZnaku=4;dodajMacierzLitery(literaNawiasLewy);}
	else if(tekst[i]==')'){dlugoscZnaku=4;dodajMacierzLitery(literaNawiasPrawy);}
	else if(tekst[i]=='.'){dlugoscZnaku=3;dodajMacierzLitery(literaKropka);}
	else if(tekst[i]==','){dlugoscZnaku=3;dodajMacierzLitery(literaPrzecinek);}
	else if(tekst[i]==58){dlugoscZnaku=3;dodajMacierzLitery(literaDwukropek);}
	else{dlugoscZnaku=6;dodajMacierzLitery(literaZnakZapytania);}


}
}

void DostosujDoPrzewijania(int ile_led){
	int aktualnaDlugosc = dlugoscTablicyGlownej();
	for(int i=0; i<8;i++){
		for(int j=0; j<8*ile_led; j++){
			tablicaGlowna[i][aktualnaDlugosc+j]=0;
		}
		}

	aktualnaDlugosc = dlugoscTablicyGlownej();
	for(int i=0; i<8;i++){
		for(int j=0; j<8*ile_led; j++){
			tablicaGlowna[i][aktualnaDlugosc+j]=tablicaGlowna[i][j];
		}
		}


}


void ArrayToLed(int ile_led, int speed){
	int aktualnaDlugosc=(ile_led*8)+1;

	if(mode==1){aktualnaDlugosc = dlugoscTablicyGlownej();}

	for(int przewijacz=0;przewijacz<aktualnaDlugosc-(ile_led*8);przewijacz++){

	for(uint8_t i=0; i<8; i++){
		HAL_GPIO_WritePin(cs_GPIO_Port, cs_Pin, 0);
	for(int k=0; k<ile_led; k++){
		uint8_t decimal=0;
	for(int j=0; j<8; j++){

	decimal= decimal+tablicaGlowna[i][(k*8)+j+przewijacz]*(int)pow(2,7-j);

	}
	write_byte(i+1);
	write_byte(decimal);



	}


	HAL_GPIO_WritePin(cs_GPIO_Port, cs_Pin, 1); //koniec ciągu danych
	}
HAL_Delay(20*(9-speed));
}

}


void write_byte (uint8_t byte)
{
for (int i=0; i<8; i++){   //8 razy bo tyle ma bajt i tyle przesunięc wykonamy
	HAL_GPIO_WritePin(clk_GPIO_Port, clk_Pin, 0);  //stan niski na zegarze
	HAL_GPIO_WritePin(data_in_GPIO_Port, data_in_Pin, byte&0x80); //bierzemy najstarszy bit
	byte= byte <<1; //przesuwamy w lewo
	HAL_GPIO_WritePin(clk_GPIO_Port, clk_Pin, 1);  //stan wysoki na zegarze
}
}

void maxInit(uint8_t adres, uint8_t data){
	HAL_GPIO_WritePin(cs_GPIO_Port, cs_Pin, 0); //cs na niskim

	for(int i=0;i<ilosc_led;i++){

	write_byte(adres);
	write_byte(data);
}
	HAL_GPIO_WritePin(cs_GPIO_Port, cs_Pin, 1);  //cs na wysokim
}






/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE BEGIN 1 */


  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */


  if(otworzPlikZParametrami())otworzPlikZTekstem();

  wypelnijGlownaTablice();

  KonwertujTekst(strlen(tekst)-1);

  maxInit(0x09,0x00);
  maxInit(0x0a,kontrast);//kontrast
  maxInit(0x0b,0x07);//
  maxInit(0x0c,0x01);//
  maxInit(0x0f,0x00);//

  DostosujDoPrzewijania(ilosc_led);

//

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  ArrayToLed(ilosc_led,szybkosc);

  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

















  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
