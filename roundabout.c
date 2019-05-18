#include<X11/Xlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#define ILOSC_OBIEKTOW 5
#define GORA	0
#define DOL		1
#define PRAWO	2
#define LEWO	3
#define X_GORA_IN		270
#define Y_GORA_IN		130
#define X_GORA_OUT	210
#define Y_GORA_OUT	130
#define X_DOL_IN		210
#define Y_DOL_IN		370
#define X_DOL_OUT		270
#define Y_DOL_OUT		370
#define X_PRAWO_IN	370
#define Y_PRAWO_IN	270
#define X_PRAWO_OUT	370
#define Y_PRAWO_OUT	210
#define X_LEWO_IN		130
#define Y_LEWO_IN		210
#define X_LEWO_OUT	130
#define Y_LEWO_OUT	270
#define ROZMIAR_T       20
#define ROZMIAR_S		20
#define ROZMIAR_P		10

int ile_lewo=0;
int ile_prawo=0;
int ile_dol=0;
int ile_gora=0;
int obiekty_startowe[4];
int obiekty_koncowe[4];
int x_we[4],y_we[4],x_wy[4],y_wy[4];
sem_t jpoziom,jpion;
sem_t g,gl,l,dl,dd,dp,p,pg;
sem_t pasy;
sem_t wjazd[4];
sem_t wyjazd[4];
pthread_t samochody[4];
pthread_t piesi[4];
Window w;
XEvent e;
int s;
Display *d;
GC green_gc;
XColor green_col;
Colormap green_colormap;
char green[] = "#00FF00";
GC red_gc;
XColor red_col;
Colormap red_colormap;
char red[] = "#FF0000";
void rysujTramwaj(int px, int py){
XFillRectangle(d, w, DefaultGC(d, s), px, py, ROZMIAR_S, 50);
}
void rysujPieszego(int px, int py){
XDrawArc(d, w, DefaultGC(d, s), px, py, ROZMIAR_P, ROZMIAR_P, 0, 360*64);
}
void rysujSamochod(int px, int py){
XFillRectangle(d, w, DefaultGC(d, s), px, py, ROZMIAR_S,ROZMIAR_S);
}
void pokazPieszego(int px, int py, int ile, int vec, int flip){
int i;
for (i = 0;i<ile;i++){
	rysujPieszego(px,py);
	if (flip == 0) py -= vec * (ROZMIAR_P+5);
	else px -= vec * (ROZMIAR_P+5);
	XFlush(d);
}
}

void pokazTramwaj(int px,int py, int ile, int vec,int flip){
int i;
for (i = 0;i<ile;i++){
	rysujTramwaj(px,py);
	if (flip == 0) py -= vec * (ROZMIAR_S+5);
	else px -= vec * (ROZMIAR_S+5);
	XFlush(d);
}
void pokazSamochod(int px,int py, int ile, int vec,int flip){
int i;
for (i = 0;i<ile;i++){
	rysujSamochod(px,py);
	if (flip == 0) py -= vec * (ROZMIAR_S+5);
	else px -= vec * (ROZMIAR_S+5);
	XFlush(d);
}
}

void wyczyscPozycjeTramwaju(int wpx, int wpy){
XSetForeground( d, DefaultGC(d, s), WhitePixel(d, s));
rysujPieszego(wpx,wpy);
XFlush(d);
}
void wyczyscPozycjePieszego(int wpx, int wpy){
XSetForeground( d, DefaultGC(d, s), WhitePixel(d, s));
rysujPieszego(wpx,wpy);
XFlush(d);
}
void wyczyscPozycjeSamochodu(int wpx, int wpy){
XSetForeground( d, DefaultGC(d, s), WhitePixel(d, s));
rysujSamochod(wpx,wpy);
XFlush(d);
}
void zmienPozycjePieszego(int spx, int spy, int npx, int npy){
wyczyscPozycjePieszego(spx,spy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
XFlush(d);
rysujPieszego(npx,npy);
XFlush(d);
sleep(1);
}



void rys(int npx, int npy, int gdzie){
int cel=0;

do{
//gora
if(npx==210&&npy==150){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=170;
	npy=180;
	rysujSamochod(npx,npy);
XFlush(d);}
//lewogora
if(npx==170&&npy==180){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=150;
	npy=210;
	rysujSamochod(npx,npy);
XFlush(d);}
//lewo
if(npx==150&&npy==210&&gdzie!=LEWO){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=150;
	npy=270;
	rysujSamochod(npx,npy);
XFlush(d);}
else if(npx==150&&npy==210&&gdzie==LEWO){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
npx=X_LEWO_IN-ile_lewo*25;
npy=Y_LEWO_IN;
ile_lewo++;
//npx=130;
//npy=210;
rysujSamochod(npx,npy);
cel=1;
XFlush(d);
}
//lewo
if(npx==150&&npy==270){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=170;
	npy=320;
	rysujSamochod(npx,npy);
XFlush(d);
}
//lewodol
if(npx==170&&npy==320){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=210;
	npy=330;
	rysujSamochod(npx,npy);
XFlush(d);}
//dol
if(npx==210&&npy==330&&gdzie!=DOL){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=270;
	npy=330;
	rysujSamochod(npx,npy);
XFlush(d);}
else if(npx==210&&npy==330&&gdzie==DOL){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
npx=X_DOL_IN;
npy=Y_DOL_IN+ile_dol*25;
ile_dol++;
rysujSamochod(npx,npy);
cel=1;
XFlush(d);
}
//dol
if(npx==270&&npy==330){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=310;
	npy=320;
	rysujSamochod(npx,npy);
XFlush(d);
}
//prawodol
if(npx==310&&npy==320){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=330;
	npy=270;
	rysujSamochod(npx,npy);
XFlush(d);}
//prawo
if(npx==330&&npy==270&&gdzie!=PRAWO){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=330;
	npy=210;
	rysujSamochod(npx,npy);
XFlush(d);}
else if(npx==330&&npy==270&&gdzie==PRAWO){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
npx=X_PRAWO_IN+ile_prawo*25;
npy=Y_PRAWO_IN;
ile_prawo++;
//npx=370;
//npy=210;
rysujSamochod(npx,npy);
cel=1;
XFlush(d);
}
//prawo
if(npx==330&&npy==210){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=310;
	npy=180;
	rysujSamochod(npx,npy);
XFlush(d);}
//prawogora
if(npx==310&&npy==180){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=270;
	npy=150;
	rysujSamochod(npx,npy);
XFlush(d);
}
//gora
if(npx==270&&npy==150&&gdzie!=GORA){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=210;
	npy=150;
	rysujSamochod(npx,npy);
XFlush(d);}
else if(npx==270&&npy==150&&gdzie==GORA){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
npx=X_GORA_IN;
npy=Y_GORA_IN-ile_gora*25;
ile_gora++;
//npx=270;
//npy=130;
rysujSamochod(npx,npy);
cel=1;
XFlush(d);
}
if(npx==150&&npy==210){
sleep(1);
wyczyscPozycjeSamochodu(npx,npy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	npx=150;
	npy=270;
	rysujSamochod(npx,npy);
XFlush(d);}
}while(cel==0);
cel=0;

}



void zmienPozycjeSamochodu(int spx, int spy, int npx, int npy, int gdzie){
wyczyscPozycjeSamochodu(spx,spy);
XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
XFlush(d);
if((spx==210&&spy==30)||(spx==210&&spy==55)||(spx==210&&spy==80)||(spx==210&&spy==105)||(spx==210&&spy==130)){
sleep(1);
	npx=210;
	npy=150;
	rysujSamochod(npx,npy);
XFlush(d);
rys(npx,npy,gdzie);
}else
if((spx==30&&spy==270)||(spx==55&&spy==270)||(spx==80&&spy==270)||(spx==105&&spy==270)||(spx==130&&spy==270)){
sleep(1);
	npx=150;
	npy=270;
	rysujSamochod(npx,npy);
XFlush(d);
rys(npx,npy,gdzie);
}else
if((spx==270&&spy==470)||(spx==270&&spy==445)||(spx==270&&spy==420)||(spx==270&&spy==395)||(spx==270&&spy==370)){
sleep(1);
	npx=270;
	npy=330;
	rysujSamochod(npx,npy);
XFlush(d);
rys(npx,npy,gdzie);
}else
if((spx==470&&spy==210)||(spx==445&&spy==210)||(spx==420&&spy==210)||(spx==395&&spy==210)||(spx==370&&spy==210)){
sleep(1);
	npx=330;
	npy=210;
	rysujSamochod(npx,npy);
XFlush(d);
rys(npx,npy,gdzie);
}
sleep(1);
}

void rysujDroge(){
		XDrawLine(d, w, DefaultGC(d,s), 200,50,200,150);
		XDrawLine(d, w, DefaultGC(d,s), 240,50,240,450);
		XDrawLine(d, w, DefaultGC(d,s), 260,50,260,450);
		XDrawLine(d, w, DefaultGC(d,s), 300,50,300,150);
		XDrawLine(d, w, DefaultGC(d,s), 200,350,200,450);
		XDrawLine(d, w, DefaultGC(d,s), 300,350,300,450);
		XDrawLine(d, w, DefaultGC(d,s), 50,200,150,200);
		XDrawLine(d, w, DefaultGC(d,s), 50,240,450,240);
		XDrawLine(d, w, DefaultGC(d,s), 50,260,450,260);
		XDrawLine(d, w, DefaultGC(d,s), 50,300,150,300);
		XDrawLine(d, w, DefaultGC(d,s), 350,200,450,200);
		XDrawLine(d, w, DefaultGC(d,s), 350,300,450,300);

		XDrawArc(d, w, DefaultGC(d,s),150,150,100,100,90*64,90*64);
		XDrawArc(d, w, DefaultGC(d,s),150,250,100,100,180*64,90*64);
		XDrawArc(d, w, DefaultGC(d,s),250,150,100,100,0*64,90*64);
		XDrawArc(d, w, DefaultGC(d,s),250,250,100,100,270*64,90*64);
		XDrawArc(d, w, DefaultGC(d,s),200,200,100,100,0*64,360*64);
}
void wyczyscDroge(){
XSetForeground( d, DefaultGC(d, s), WhitePixel(d, s));
XFillRectangle(d, w, DefaultGC(d, s), X_GORA_IN, 0,20,200);
XFillRectangle(d, w, DefaultGC(d, s), X_DOL_IN, 300,20,200);
XFillRectangle(d, w, DefaultGC(d, s), 0, Y_LEWO_IN,250,20);
XFillRectangle(d, w, DefaultGC(d, s), 350, Y_PRAWO_IN,250,20);
XFlush(d);
}

void *tramwaj(void* arg){
int a = (int) arg;
int l;
int deltay,deltax;
int ile;
ile = 1 + rand()%ILOSC_OBIEKTOW;
int sx,sy,kx,ky;
if (a == GORA){ sx = x_wy[a] - 22; sy = y_wy[a] + 10; }
else if (a == DOL){ sx = x_wy[a] + 32; sy = y_wy[a]; }
else if (a == LEWO){ sx = x_wy[a] + 10; sy = y_wy[a] + 32; }
else if (a == PRAWO){ sx = x_wy[a]; sy = y_wy[a] - 22; }
if (a == GORA) pokazTramwaj(sx,sy,ile,1,0);
else if (a == DOL) pokazTramwaj(sx,sy,ile,-1,0);
else if (a == LEWO) pokazTramwaj(sx,sy,ile,1,1);
else if (a == PRAWO) pokazTramwaj(sx,sy,ile,-1,1);
sem_wait(&pasy);
deltay = sy;
deltax = sx;
for (l = 0; l < ile;l++){
	if (a == GORA){
		sy = y_wy[a] + 10 - (ile-l-1)* (ROZMIAR_P+5);
		kx = x_we[a] + 37; ky = sy;
		}
	else if (a == DOL){
		sy = y_wy[a] + (ile-l-1)* (ROZMIAR_P+5);
		kx = x_we[a] - 27; ky = sy;
		}
	else if (a == LEWO){
		sx = x_wy[a] + 10 - (ile-l-1)*(ROZMIAR_P+5);
		kx = sx; ky = y_we[a] - 27;
		}
	else if (a == PRAWO) {
		sx = x_wy[a] + (ile-l-1)*(ROZMIAR_P+5);
		kx = sx; ky = y_we[a] + 37;
		}
	zmienPozycjePieszego(sx,sy,kx,ky);
}
sleep(1);
sem_post(&pasy);
}

void *pieszy(void* arg){
int a = (int) arg;
int l;
int deltay,deltax;
int ile;
ile = 1 + rand()%ILOSC_OBIEKTOW;
int sx,sy,kx,ky;
if (a == GORA){ sx = x_wy[a] - 22; sy = y_wy[a] + 10; }
else if (a == DOL){ sx = x_wy[a] + 32; sy = y_wy[a]; }
else if (a == LEWO){ sx = x_wy[a] + 10; sy = y_wy[a] + 32; }
else if (a == PRAWO){ sx = x_wy[a]; sy = y_wy[a] - 22; }
if (a == GORA) pokazPieszego(sx,sy,ile,1,0);
else if (a == DOL) pokazPieszego(sx,sy,ile,-1,0);
else if (a == LEWO) pokazPieszego(sx,sy,ile,1,1);
else if (a == PRAWO) pokazPieszego(sx,sy,ile,-1,1);
sem_wait(&pasy);
deltay = sy;
deltax = sx;
for (l = 0; l < ile;l++){
	if (a == GORA){
		sy = y_wy[a] + 10 - (ile-l-1)* (ROZMIAR_P+5);
		kx = x_we[a] + 37; ky = sy;
		}
	else if (a == DOL){
		sy = y_wy[a] + (ile-l-1)* (ROZMIAR_P+5);
		kx = x_we[a] - 27; ky = sy;
		}
	else if (a == LEWO){
		sx = x_wy[a] + 10 - (ile-l-1)*(ROZMIAR_P+5);
		kx = sx; ky = y_we[a] - 27;
		}
	else if (a == PRAWO) {
		sx = x_wy[a] + (ile-l-1)*(ROZMIAR_P+5);
		kx = sx; ky = y_we[a] + 37;
		}
	zmienPozycjePieszego(sx,sy,kx,ky);
}
sleep(1);
sem_post(&pasy);
}
void *samochod(void* arg){
int a = (int) arg;
int ile;
int gdzie;
ile = 1 + rand()%ILOSC_OBIEKTOW;
int l;
int sx,sy,kx,ky;
sx = x_wy[a];
sy = y_wy[a];
obiekty_startowe[a] = ile;
if (a == GORA) pokazSamochod(sx,sy,obiekty_startowe[a],1,0);
else if (a == DOL) pokazSamochod(sx,sy,obiekty_startowe[a],-1,0);
else if (a == LEWO) pokazSamochod(sx,sy,obiekty_startowe[a],1,1);
else if (a == PRAWO) pokazSamochod(sx,sy,obiekty_startowe[a],-1,1);
if (a == GORA || a == DOL) sem_wait(&jpion);
if (a == LEWO || a == PRAWO) sem_wait(&jpoziom);

for(l = 0; l < ile; l++){
	sem_wait(&wyjazd[a]);
	do{
		gdzie = rand()%4;
	}while(gdzie == a);
	sem_wait(&wjazd[gdzie]);
	if(gdzie == LEWO ){
		kx = x_we[gdzie]- obiekty_koncowe[gdzie] * (ROZMIAR_S+5) - 5;
		ky = y_we[gdzie];
		}
	else if(gdzie == PRAWO){
		kx = x_we[gdzie]+ obiekty_koncowe[gdzie] * (ROZMIAR_S+5)+ 10;
		ky = y_we[gdzie];
		}
	else if(gdzie == GORA ){
		ky = y_we[gdzie] - obiekty_koncowe[gdzie] * (ROZMIAR_S+5) - 5;
		kx = x_we[gdzie];
		}
	else if(gdzie == DOL){
		ky = y_we[gdzie] + obiekty_koncowe[gdzie] * (ROZMIAR_S+5) + 10;
		kx = x_we[gdzie];
		}
	if (a == GORA) sy = y_wy[a] - (obiekty_startowe[a]-1)* (ROZMIAR_S+5);
	else if (a == DOL) sy = y_wy[a] + (obiekty_startowe[a]-1)* (ROZMIAR_S+5);
	else if (a == LEWO) sx = x_wy[a] - (ile-l-1)*(ROZMIAR_S+5);
	else if (a == PRAWO) sx = x_wy[a] + (ile-l-1)*(ROZMIAR_S+5);
	zmienPozycjeSamochodu(sx,sy,kx,ky,gdzie);
	obiekty_koncowe[gdzie]++;
	obiekty_startowe[a]--;
	sem_post(&wyjazd[a]);
	sem_post(&wjazd[gdzie]);
	}
if (a == GORA || a == DOL)  sem_post(&jpion);
if (a == LEWO || a == PRAWO)  sem_post(&jpoziom);
}
void main(void) {
srand(time(0));
x_wy[PRAWO] = X_PRAWO_OUT;
y_wy[PRAWO] = Y_PRAWO_OUT;
x_wy[LEWO] = X_LEWO_OUT;
y_wy[LEWO] = Y_LEWO_OUT;
x_wy[DOL] = X_DOL_OUT;
y_wy[DOL] = Y_DOL_OUT;
x_wy[GORA] = X_GORA_OUT;
y_wy[GORA] = Y_GORA_OUT;
x_we[PRAWO] = X_PRAWO_IN;
y_we[PRAWO] = Y_PRAWO_IN;
x_we[LEWO] = X_LEWO_IN;
y_we[LEWO] = Y_LEWO_IN;
x_we[DOL] = X_DOL_IN;
y_we[DOL] = Y_DOL_IN;
x_we[GORA] = X_GORA_IN;
y_we[GORA] = Y_GORA_IN;
d=XOpenDisplay(NULL);
if(d==NULL) {
	printf("Cannot open display\n");
	exit(1);
   }
s=DefaultScreen(d);
w=XCreateSimpleWindow(d, RootWindow(d, s),5, 5, 500, 500, 2, BlackPixel(d, s), WhitePixel(d, s));
XSelectInput(d, w, ExposureMask | KeyPressMask);
green_colormap = DefaultColormap(d, 0);
green_gc = XCreateGC(d, w, 0, 0);
XParseColor(d, green_colormap, green, &green_col);
XAllocColor(d, green_colormap, &green_col);
red_colormap = DefaultColormap(d, 0);
red_gc = XCreateGC(d, w, 0, 0);
XParseColor(d, red_colormap, red, &red_col);
XAllocColor(d, red_colormap, &red_col);
XMapWindow(d, w);
XNextEvent(d, &e);
int ws;
for(ws = 0; ws < 4; ws++) {
	sem_init(&wjazd[ws], 1, 1);
	sem_init(&wyjazd[ws], 1, 1);
 	obiekty_koncowe[ws]=0;
 	obiekty_startowe[ws]=0;
 	}
sem_init(&jpoziom, 0, 0);
sem_init(&jpion, 0, 0);
sem_init(&pasy, 0, 4);
for(;;){
	rysujDroge(); XFlush(d); sleep(1);
	sem_wait(&pasy);
	sem_wait(&pasy);
	sem_wait(&pasy);
	sem_wait(&pasy);
	XSetForeground( d, DefaultGC(d, s), BlackPixel(d, s));
	int gen;
	for (gen = 0; gen < 4; gen++)
	{
		pthread_create(&samochody[gen],NULL,samochod,(void*)gen);
		pthread_create(&piesi[gen],NULL,pieszy,(void*)gen);
	}
	sleep(1);
	sem_post(&jpion);
	sem_post(&jpion);
	sem_post(&jpoziom);
	sem_post(&jpoziom);
	pthread_join(samochody[GORA],NULL);
	pthread_join(samochody[DOL],NULL);
	XFlush(d);
	pthread_join(samochody[LEWO],NULL);
	pthread_join(samochody[PRAWO],NULL);
	for(ws = 0; ws < 4; ws++) {
 		obiekty_koncowe[ws]=0;
 		obiekty_startowe[ws]=0;
 	}
	wyczyscDroge();
	XFlush(d); sleep(1);
	sem_post(&pasy);
	sem_post(&pasy);
	sem_post(&pasy);
	sem_post(&pasy);
	sleep(1);
	for (gen = 0; gen < 4; gen++) pthread_join(piesi[gen],NULL);
}
sleep(5);
}