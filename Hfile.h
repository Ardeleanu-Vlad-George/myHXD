#ifndef HFILE_INCLUDED
#define HFILE_INCLUDED
#include <stdio.h>
#define uchar unsigned char
#define LinVert 179
#define LinOriz 196
#define DreaJos 217
#define StanSus 218
#define DreaSus 191
#define StanJos 192
#define int1101 193
#define int1110 195
#define UP 72
#define RG 77
#define DW 80
#define LF 75
#define NRCOL "\4\10\x10"
//typedef enum {col1=4,col2=8,col3=16} NRCOL;
typedef enum {mod1,mod2} MOD;
typedef struct{
    int cap;
    char *cont;
}SIR;
typedef struct{
    int x;
    int y;
} POZITIE;
typedef struct{
    FILE * f;
    SIR Cale;
} FISIER;
typedef struct{
    int nrOct;
    char *HEX;
} CONTINUT;
//SET/PRESET
void PRESET(FISIER*,CONTINUT*,POZITIE*);
CONTINUT SetCONTINUT(int);
POZITIE PresetPOZITIE();
FISIER PresetFISIER();
CONTINUT PresetCONTINUT();
SIR SetSIR(int);
SIR RealocSIR(SIR,int);
//SET/PRESET
char*ExtragNUME(char*);
int SalvareInFisier(CONTINUT,FISIER);
CONTINUT DeschidereFisier(FISIER);
char*PrimescComanda(int);
uchar * buildAscii(CONTINUT);
void AfisareTabel(char*,CONTINUT,int*,MOD);
//pastraeza de aici in sus
void afisareMod(MOD);
void switchinComanda(int,FISIER*,CONTINUT*,POZITIE*,int*,MOD*);
int luareDecizieinComanda();
void ModificareCONTINUT(CONTINUT *,POZITIE * ,int ,char );
#endif // HFILE_INCLUDED
