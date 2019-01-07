#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "Hfile.h"
int main()
{
    char*nume,*test;
    FISIER fis;
    CONTINUT cont;
    int Decizie;
    MOD m=mod1;
    POZITIE poz;
    int col=0;
    PRESET(&fis,&cont,&poz);
    ModifyCursorFile();
    while(1){
        nume=ExtragNUME(fis.Cale.cont);
        AfisareTabel(nume,cont,&col,m);
        gotoxy(poz.x,poz.y);
        switch(m){
            case 0:
                Decizie=luareDecizieinComanda();
                switchinComanda(Decizie,&fis,&cont,&poz,&col,&m);
                break;
            case 1:
                Decizie=luareDecizieinEditMode();
                switchinEditMode(Decizie,&cont,&poz,col,&m);
                break;
        }
    }
    return 0;
}
