#include "Hfile.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
//#include <cstring.h>
//MODIFICARE CURSOR
void ModifyCursorFile(){
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO inf;
    inf.dwSize=100;
    SetConsoleCursorInfo(h,&inf);
}
void HideCursor(){

    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO inf;
    inf.dwSize=100;
    inf.bVisible=FALSE;
    SetConsoleCursorInfo(h,&inf);
}
void ModifyCursorCommand(){
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO inf;
    inf.dwSize=1;
    SetConsoleCursorInfo(h,&inf);
}
//MODIFICARE CURSOR
//MISCARE
POZITIE MiscareUP(POZITIE poz){
    POZITIE nou;
    nou.x=poz.x;
    nou.y=poz.y;
    if(nou.y!=6)
        nou.y--;
    else
        printf("\a");
    return nou;
}
POZITIE MiscareLF(POZITIE vechi,int lin){
    POZITIE nou=vechi;
    if(nou.x==2 && nou.y==6)
        printf("\a");
    else if(nou.x==2 && nou.y!=6){
        nou.x=NRCOL[lin]*3+1;
        nou.y--;
    }
    else
        nou.x--;
    return nou;
}
POZITIE MiscareDW(POZITIE vechi,int lun,int lin){
   POZITIE nou;
   int nrl,rest;
   nrl=lun/NRCOL[lin];
   rest=lun%NRCOL[lin];
   if(rest!=0)
        nrl++;
    nou=vechi;
   if(rest==0){
       if(nou.y==nrl+5)
            printf("\a");
       else
            nou.y++;
   }else{
        if(nou.y==nrl+5)
            printf("\a");
        else{
            if(nou.y==nrl+4 && nou.x>3*rest+1)
                nou.x=3*rest+1;
            nou.y++;
        }
    }
    return nou;
}
POZITIE MiscareRG(POZITIE vechi,int lun,int lin){
    POZITIE nou;
    int nrl,rest;
    nou.x=vechi.x;
    nou.y=vechi.y;
    nrl=lun/NRCOL[lin];
    rest=lun%NRCOL[lin];
    if(rest!=0)
        nrl++;
    if(rest<0){
        printf("\a");
        return nou;
    }
    if(rest==0){
        if(nou.x==NRCOL[lin]*3+1 && nou.y==nrl+5)
            printf("\a");
        else if(nou.x==NRCOL[lin]*3+1){
            nou.x=2;
            nou.y++;
        }else
            nou.x++;
    }else{
        if(nou.x==rest*3+1 && nou.y==nrl+5)
            printf("\a");
        else if(nou.x==NRCOL[lin]*3+1 && nou.y!=nrl+5){
            nou.x=2;
            nou.y++;
        }else
            nou.x++;
    }
    return nou;
}
//MISCARE
//PRESET/SET
void PRESET(FISIER *fis,CONTINUT*cont,POZITIE*poz){
    *fis=PresetFISIER();
    *cont=PresetCONTINUT();
    *poz=PresetPOZITIE();
}
FISIER PresetFISIER(){
    FISIER nou;
    nou.Cale.cont=strdup("<new doc>");
    nou.Cale.cap=strlen(nou.Cale.cont);
    nou.f=NULL;
    return nou;
}
FISIER SetFISIER(char*sursa){
    FISIER nou;
    nou.Cale=SetSIR(strlen(sursa));
    strcpy(nou.Cale.cont,sursa);
    return nou;
}
SIR SetSIR(int ncap){
    SIR nou;
    nou.cont=(char*)malloc(ncap+1);
    nou.cont[0]=0;
    nou.cap=ncap;
    return nou;
}
SIR RealocSIR(SIR vechi,int depus){
    SIR nou;
    if(vechi.cap-strlen(vechi.cont)<depus)
        if(depus==1)
            nou.cap=4+vechi.cap;
        else
            nou.cap=10+vechi.cap;
    nou.cont=(char*)realloc(vechi.cont,nou.cap);
    return nou;
}
CONTINUT SetCONTINUT(int n){
    CONTINUT nou;
    nou.HEX=(char*)malloc(3*n+1);
    nou.nrOct=n;
    return nou;
}
CONTINUT PresetCONTINUT(){
    CONTINUT nou;
    nou.HEX=strdup("<no content>");
    nou.nrOct=-1;
    return nou;
}
POZITIE PresetPOZITIE(){
    POZITIE nou;
    nou.x=2;
    nou.y=6;
    return nou;
}
//PRESET/SET
uchar * buildAscii(CONTINUT source){
    uchar *c,buff[4];
    int i,x;
    if(source.nrOct<0){
        c=(char*)malloc(1);
        c[0]=0;
        return c;
    }
    c=(uchar*)malloc(source.nrOct);
    for(i=0;i<source.nrOct;i++){
        strncpy(buff,source.HEX+i*3,3);
        sscanf(buff,"%x ",&x);
        c[i]=(uchar)x;
    }
    return c;
}
char * PrimescComanda(int lin){
    SIR com;
    char car;
    int poz=0;
    gotoxy(1,8+lin);
    printf("Command:");
    com=SetSIR(10);
    while(1){
        car=getch();
        if(car==0 || car==0xE0){
            getch();
            continue;
        }
        if(car=='\r')
            break;
        if(car=='\b'){
            if(poz!=0){
                poz--;
                printf("\b \b");
            }
            continue;
        }
        putchar(car);
        com=RealocSIR(com,1);
        com.cont[poz++]=car;
    }
    com.cont[poz]=0;
    return com.cont;
}
char * ExtragNUME(char*sursa){
    SIR c;
    int n;
    if(strrchr(sursa,'\\')!=NULL)
        n=strlen(strrchr(sursa,'\\'))-1;
    else
        n=strlen(sursa);
    c=SetSIR(n);
    if(strrchr(sursa,'\\')!=NULL)
        strcpy(c.cont,strrchr(sursa,'\\')+1);
    else
        strcpy(c.cont,sursa);
    return c.cont;
}
//FISIERE
int SalvareInFisier(CONTINUT sursa,FISIER dest){
    uchar *c=buildAscii(sursa);
    dest.f=fopen(dest.Cale.cont,"wb");
    if(sursa.nrOct<0){
        fclose(dest.f);
        return 0;
    }
    if(dest.f==NULL)
        return -1;
    fwrite(c,1,sursa.nrOct,dest.f);
    fclose(dest.f);
    return 1;
}
CONTINUT DeschidereFisier(FISIER open){
    CONTINUT rez=PresetCONTINUT();
    uchar *c;
    int i;
    open.f=fopen(open.Cale.cont,"rb");
    if(open.f==NULL)
        return rez;
    fseek(open.f,0,SEEK_END);
    rez.nrOct=ftell(open.f);
    if(rez.nrOct==0){
        rez=PresetCONTINUT();
        rez.nrOct=-2;
        return rez;
    }
    fseek(open.f,0,SEEK_SET);
    c=(uchar*)malloc(rez.nrOct);
    fread(c,1,rez.nrOct,open.f);
    fclose(open.f);
    rez=SetCONTINUT(rez.nrOct);
    for(i=0;i<rez.nrOct;i++)
        sprintf(rez.HEX+i*3,"%02X ",c[i]);
    free(c);
    return rez;
}
//FISIER
//AFISARE
void AfisASCII(CONTINUT source){
    uchar *c=buildAscii(source);
    int i,linnoua=0;
    system("cls");
    HideCursor();
    if(source.nrOct<=0){
        textcolor(YELLOW);
        printf("ASCII content:");
        cprintf("<no content>");
        return;
    }
    textcolor(YELLOW);
    printf("ASCII content(");
    cprintf("yellow ");
    printf("means special char):\n\n");
    for(i=0;i<source.nrOct;i++){
        switch(c[i]){
            case 0:
                linnoua+=cprintf("<%s>","NULL");
                break;
            case 1:
                linnoua+=cprintf("<%s>","SOH");
                break;
            case 2:
                linnoua+=cprintf("<%s>","STX");
                break;
            case 3:
                linnoua+=cprintf("<%s>","ETX");
                break;
            case 4:
                linnoua+=cprintf("<%s>","EOT");
                break;
            case 5:
                linnoua+=cprintf("<%s>","ENQ");
                break;
            case 6:
                linnoua+=cprintf("<%s>","ACK");
                break;
            case 7:
                linnoua+=cprintf("<%s>","BEL");
                break;
            case 8:
                linnoua+=cprintf("<%s>","BS");
                break;
            case 9:
                linnoua+=cprintf("<%s>","HT");
                break;
            case 10:
                linnoua+=cprintf("<%s>","LF");
                break;
            case 11:
                linnoua+=cprintf("<%s>","VT");
                break;
            case 12:
                linnoua+=cprintf("<%s>","FF");
                break;
            case 13:
                linnoua+=cprintf("<%s>","CR");
                break;
            case 14:
                linnoua+=cprintf("<%s>","SO");
                break;
            case 15:
                linnoua+=cprintf("<%s>","SI");
                break;
            case 16:
                linnoua+=cprintf("<%s>","DLE");
                break;
            case 17:
                linnoua+=cprintf("<%s>","DC1");
                break;
            case 18:
                linnoua+=cprintf("<%s>","DC2");
                break;
            case 19:
                linnoua+=cprintf("<%s>","DC3");
                break;
            case 20:
                linnoua+=cprintf("<%s>","DC4");
                break;
            case 21:
                linnoua+=cprintf("<%s>","NAK");
                break;
            case 22:
                linnoua+=cprintf("<%s>","SYN");
                break;
            case 23:
                linnoua+=cprintf("<%s>","ETB");
                break;
            case 24:
                linnoua+=cprintf("<%s>","CAN");
                break;
            case 25:
                linnoua+=cprintf("<%s>","EM");
                break;
            case 26:
                linnoua+=cprintf("<%s>","SUB");
                break;
            case 27:
                linnoua+=cprintf("<%s>","ESC");
                break;
            case 28:
                linnoua+=cprintf("<%s>","FS");
                break;
            case 29:
                linnoua+=cprintf("<%s>","GS");
                break;
            case 30:
                linnoua+=cprintf("<%s>","RS");
                break;
            case 31:
                linnoua+=cprintf("<%s>","US");
                break;
            case 127:
                linnoua+=cprintf("<%s>","DEL");
                break;
            case 255:
                linnoua+=cprintf("<%s>","nbsp");
                break;
            default:
                linnoua+=printf("%c",c[i]);
        }
        if(linnoua>=48){
            linnoua=0;
            printf("\n");
        }
    }

}
void AfisareTabel(char *nume,CONTINUT cont,int *lung,MOD m){
    int i,nrlinii;
    system("cls");
    while(NRCOL[*lung]*3<strlen(nume)+1)
        ++*lung;
    nrlinii=cont.nrOct/NRCOL[*lung];
    printf("\n\n");
    printf("%c",StanSus);
    for(i=0;i<strlen(nume);i++)
        printf("%c",LinOriz);
    printf("%c\n",DreaSus);
    printf("%c%s%c\n",LinVert,nume,LinVert);
    printf("%c",int1110);
    for(i=0;i<strlen(nume);i++)
        printf("%c",LinOriz);
    printf("%c",int1101);
    for(i=0;i<NRCOL[*lung]*3-strlen(nume)-1;i++)
        printf("%c",LinOriz);
    printf("%c",DreaSus);
    for(i=0;i<nrlinii;i++)
        printf("\n%c%.*s%c",LinVert,NRCOL[*lung]*3,cont.HEX+NRCOL[*lung]*3*i,LinVert);
    if(cont.nrOct%NRCOL[*lung]!=0)
        printf("\n%c%-*.*s%c",LinVert,3*NRCOL[*lung],3*NRCOL[*lung],cont.HEX+3*i*NRCOL[*lung],LinVert);
    printf("\n%c",StanJos);
    for(i=0;i<NRCOL[*lung]*3;i++)
        printf("%c",LinOriz);
    printf("%c",DreaJos);
    afisareMod(m);
}
void afisareMod(MOD mod){
    if(mod==mod1)
        printf("\nCommand mode");
    else
        printf("\nEdit mode");
}
void AfisHELP(MOD m){
    system("cls");
    printf("COMMAND MODE:"
           "\n\t-use arrow keys to move around"
           "\n\t-press <BS> to delete the current byte"
           "\n\t-press <ESC> to exit the program"
           "\n\t-press 'e' to enter EDIT MODE"
           "\n\t-press 'a' to see the ASCII content"
           "\n\t-press ':' to enter a command"
           "\n\tThese are the commands:"
           "\n\t\t-o <path>(open the file located at <path>)"
           "\n\t\t-s(save current file); s <path>(to save the file at <path>)"
           "\n\t\t-n(open an empty file); n <path>(open an empty file at <path>)"
           "\n\t\t-c(close current),cs(close and save current file)"
           "\n\t\t-cs <path>(close and save at path)"
           "\n\t\t-l <num> to change the number of columns(<num> is 4, 8 or 16)"
           );
    if(m==mod1)
        return;
    system("cls");
    printf("EDIT MODE:"
           "\n\t-use arrow keys to move around"
           "\n\t-press <BS> to delete the current byte"
           "\n\t-press <ESC> to exit the EDIT MODE"
           "\n\t-use one of the following keys to change the value of a byte:"
           "\n\t\t-keys{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F}"
           "\n\t-place the cursor between two bytes and press one of the keys from above  \t to insert a new byte"
           );
}
//AFISARE
//COMANDA
CONTINUT StergereOctetCurent(CONTINUT vechi,POZITIE *poz,int col){
    CONTINUT nou=vechi;
    int preal,rest;
    if(nou.nrOct<0){
        printf("\a");
        return nou;
    }
    if(nou.nrOct==1){
        nou=PresetCONTINUT();
        *poz=PresetPOZITIE();
        return nou;
    }
    preal=poz->x-2+(poz->y-6)*NRCOL[col]*3;
    rest=preal%3;
    preal/=3;
    strcpy(nou.HEX+preal*3,nou.HEX+preal*3+3);
    nou.nrOct--;
    if(preal==0)
        *poz=PresetPOZITIE();
    else
        for(;rest>-1;rest--)
            *poz=MiscareLF(*poz,col);
    return nou;
}
int luareDecizieinComanda(){
    unsigned char c;
    c=getch();
    while(1){
        if(c==0 || c==0xE0){
            c=getch();
            switch(c){
                case UP:
                    return 0;
                case RG:
                    return 1;
                case DW:
                    return 2;
                case LF:
                    return 3;//miscare
                case 59:
                    return 4;
            }
        }
        else{
            switch(c){
                case 27:
                    return 5;//parasesc programul
                case '\b':
                    return 6;//stergi octetul curent
                case 'e':
                    return 7;//trecere la mod editare
                case ':':
                    return 8;//primesc comanda
                case 'a':
                    return 9;//afisare continut ascii
                default:
                    return -1;
            }
        }
    }
}
void switchinComanda(int dec,FISIER* fis,CONTINUT *cont,POZITIE *poz,int *lin,MOD* m){
    char*c;
    FISIER nou;
    int nrl=cont->nrOct/NRCOL[*lin],rez,nr;
    if(cont->nrOct%NRCOL[*lin]!=0)
        nrl++;
    switch(dec){
    //miscarecursor
            case 0:
                *poz=MiscareUP(*poz);
                break;
                //UP
            case 1:
                *poz=MiscareRG(*poz,cont->nrOct,*lin);
                break;
                //RG
            case 2:
                *poz=MiscareDW(*poz,cont->nrOct,*lin);
                break;
                //DW
            case 3:
                *poz=MiscareLF(*poz,*lin);
                break;
                //LF
                //modificarefisier
            case 4:
                AfisHELP(*m);
                getch();
                break;//help
            case 5:
                system("cls");
                printf("Program closed");
                exit(0);
                break;
            case 6://stergere octet
                *cont=StergereOctetCurent(*cont,poz,*lin);
                break;
            case 7://trecere la editare
                *m=mod2;
                break;
            case 8:
                ModifyCursorCommand();
                c=PrimescComanda(nrl);
                rez=InterpretezComanda(c);
                switch(rez){
                    case -1:
                        printf("\nInvalid command"),getch();
                        break;
                    case 0://open
                        nou=SetFISIER(fis->Cale.cont);
                        *fis=SetFISIER(c+2);
                        *cont=DeschidereFisier(*fis);
                        *poz=PresetPOZITIE();
                        if(cont->nrOct!=-1)
                            system("cls");
                        else{
                            printf("\nFailed to open file at %s",c+2),getch();
                            *fis=SetFISIER(nou.Cale.cont);
                            *cont=DeschidereFisier(*fis);
                        }
                        break;
                    case 1://save at path
                        nou=SetFISIER(c+2);
                        *poz=PresetPOZITIE();
                        if(SalvareInFisier(*cont,nou)!=-1)
                            printf("\nSuccessfully saved at %s",c+2),getch();
                        else
                            printf("\nFailed to save at %s",c+2),getch();
                        break;
                    case 2://just save
                        if(strcmp(fis->Cale.cont,"<new doc>")==0)
                            printf("\nNo path available."),getch();
                        else
                            SalvareInFisier(*cont,*fis);
                        break;
                    case 3://new file at path
                        *cont=PresetCONTINUT();
                        *fis=SetFISIER(c+2);
                        *poz=PresetPOZITIE();
                        SalvareInFisier(*cont,*fis);
                        break;
                    case 4://just new file
                        PRESET(fis,cont,poz);
                        break;
                    case 5://justclose
                        PRESET(fis,cont,poz);
                        break;
                    case 6://close & save
                        if(strcmp(fis->Cale.cont,"<new doc>")==0)
                            printf("\nNo valid path"),getch();
                        else{
                        SalvareInFisier(*cont,*fis);
                        PRESET(fis,cont,poz);
                        }
                        break;
                    case 7:
                        nr=atoi(c+2);
                        if(nr==0)
                            printf("\nInvalid command"),getch();
                        else if(nr!=4 && nr!=8 && nr!=16)
                            printf("\nInvalid number of columns"),getch();
                        else {
                            *lin=nr/4-1;
                            if(*lin==3)
                                --*lin;
                            *poz=PresetPOZITIE();

                        }
                        break;
                    case 8://close & save at path
                        nou=SetFISIER(c+3);
                        if(SalvareInFisier(*cont,nou)!=-1)
                            PRESET(fis,cont,poz);
                        else
                            printf("\nCouldn't save"),getch();
                        break;
                }
                free(c);
                ModifyCursorFile();
                break;
            case 9:
                AfisASCII(*cont);
                if(getch()==0)
                    getch();
                ModifyCursorFile();
                break;
    }
}
int InterpretezComanda(char*com){
    switch(com[0]){
        case 'o':
            if(com[1]==' ' && strlen(com)>2)
                return 0;
            else
                return -1;
        case 's':
            if(com[1]==' ' && strlen(com)>2)
                return 1;//save at path
            else if(com[1]=='\0')
                return 2;//save current
            else
                return -1;
        case 'n':
            if(com[1]==' ' && strlen(com)>2)
                return 3;//new at path
            else if(com[1]==0)
                return 4;//just new file
            else
                return -1;
        case 'c':
            if(com[1]=='\0')
                return 5;//just close
            else if(com[1]=='s' && com[2]==0)
                return 6;//close & save
            else if(com[2]==' ' && strlen(com)>3)
                return 8;//close & save at path
            else
                return -1;
        case 'l':
            if(com[1]==' ' && strlen(com)>2)
                return 7;//change nr of columns
            else
                return -1;
        default:
            return -1;
    }
}
//COMANDA
//EDIT MODE
void switchinEditMode(int dec,CONTINUT*cont,POZITIE*poz,int lin,MOD*m){
    char c;
    switch(dec){
        case -1:
            return;
        case 0:
            *poz=MiscareUP(*poz);
            break;
            //UP
        case 1:
            *poz=MiscareRG(*poz,cont->nrOct,lin);
            break;
            //RG
        case 2:
            *poz=MiscareDW(*poz,cont->nrOct,lin);
            break;
            //DW
        case 3:
            *poz=MiscareLF(*poz,lin);
            break;
            //LF
        case 4:
            AfisHELP(*m);
            getch();
            break;
        case 5:
            *m=mod1;
            break;
        case 6:
            *cont=StergereOctetCurent(*cont,poz,lin);
            break;
        default:
            c=(char)dec;
            ModificareCONTINUT(cont,poz,lin,c);
    }
}
void ModificareCONTINUT(CONTINUT *cont,POZITIE * poz,int lin,char adaug){
    int pozreal,rest,pozb;
    SIR aux;
    char buff[4];
    pozreal=poz->x-2+(poz->y-6)*NRCOL[lin]*3;
    rest=pozreal%3;
    pozb=pozreal/3;
    if(strchr("abcdef",adaug)!=NULL)
        adaug=adaug+'A'-'a';
    if(cont->nrOct<0){
        *cont=SetCONTINUT(1);
        sprintf(cont->HEX,"%c0 ",adaug);
        *poz=MiscareRG(*poz,cont->nrOct,lin);
        return ;
    }
    if(rest<2){
        cont->HEX[pozreal]=adaug;
        *poz=MiscareRG(*poz,cont->nrOct,lin);
    }else{
        aux=SetSIR(strlen(cont->HEX+pozb*3+3));
        strcpy(aux.cont,cont->HEX+pozb*3+3);
        cont->HEX[pozreal+1]=0;
        sprintf(buff,"%c0 ",adaug);
        strcat(cont->HEX,buff);
        strcat(cont->HEX,aux.cont);
        cont->nrOct++;
        *poz=MiscareRG(*poz,cont->nrOct,lin);
        *poz=MiscareRG(*poz,cont->nrOct,lin);
        free(aux.cont);
    }
}
int luareDecizieinEditMode(){
    unsigned char c;
    c=getch();
    while(1){
        if(c==0 || c==0xE0){
            c=getch();
            switch(c){
                case UP:
                    return 0;
                case RG:
                    return 1;
                case DW:
                    return 2;
                case LF:
                    return 3;//miscare
                case 59:
                    return 4;//help
                default:
                    return -1;
            }
        }
        else{
            switch(c){
                case 27:
                    return 5;//trecere la mod comanda
                case '\b':
                    return 6;//stergi octetul curent
                default:
                    if(strchr("0123456789aAbBcCdDeEfF",c)!=NULL)
                        return (int)c;//modificare bit
                    else
                        return -1;
            }
        }
    }
}
//EDIT MODE


