#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Client{
public:
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    char buffer[1024];
    Client()
    {
        cout<<"Conectando al servidor..."<<endl<<endl;
        WSAStartup(MAKEWORD(2,0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_addr.s_addr = inet_addr("192.168.0.105");
        addr.sin_family = AF_INET;
        addr.sin_port = htons(5555);
        connect(server, (SOCKADDR *)&addr, sizeof(addr));
        cout << "Conectado al Servidor!" << endl;
    }
    void Enviar(const char* msg)
    {
        strcpy(buffer, msg);
        send(server, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        cout << "0" << endl;
    }
    void Recibir()
    {
        recv(server, buffer, sizeof(buffer), 0);
        cout << "El servidor dice: " << buffer << endl;
        memset(buffer, 0, sizeof(buffer));
    }
    void CerrarSocket()
    {
       closesocket(server);
       WSACleanup();
       cout << "Socket cerrado." << endl << endl;
    }
};

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

int pala_a = 10;
int pala_b = 10;
int puntos_a = 0;
int puntos_b = 0;
int a=0;//0 = partida jugandose/ 1 = partida terminada
char tecla;
int x=10;
int y=15;
int dirx=1;
int diry=1;
int bx;
int by;

//funcion que posiciona el cursor en las coordenadas x e y
void gotoxy(int x, int y){
    HANDLE hCon;
    COORD dwPos;

    dwPos.X = x;
    dwPos.Y = y;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon,dwPos);
}

//ocultar el cursor
void OcultarCursor(){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 2;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo(hCon, &cci);
}

void pintar_Campo(){
    //lineas horizontales
    for (int i=2;i<78;i++){
        gotoxy(i,3);
        printf("%c",205);
        gotoxy(i,23);
        printf("%c",205);
    }

    //lineas verticales
    for(int i=4;i<23;i++){
        gotoxy(2,i);
        printf("%c",186);
        gotoxy(77,i);
        printf("%c",186);
    }

    //esquinas
    gotoxy(2,3);
    printf("%c",201);
    gotoxy(2,23);
    printf("%c",200);
    gotoxy(77,3);
    printf("%c",187);
    gotoxy(77,23);
    printf("%c",188);
}

//pintar paletas
void pintar_palas(){
    //pala jugador a
    gotoxy(4,pala_a);
    printf("%c",18);
    gotoxy(4,pala_a+1);
    printf("%c",18);
    gotoxy(4,pala_a-1);
    printf("%c",18);


    //pala jugador b
    gotoxy(75,pala_b);
    printf("%c",18);
    gotoxy(75,pala_b+1);
    printf("%c",18);
    gotoxy(75,pala_b-1);
    printf("%c",18);
}

//pintar marcador
void pinta_goles(){
    gotoxy(1,1);printf("Equipo A = %d",puntos_a);
    gotoxy(65,1);printf("Equipo b = %d",puntos_b);
    if(puntos_a==10 || puntos_b==10){
        a=1;
    }
}

//borrar palas
void borrar_palas(){
    //pala jugador a
    gotoxy(4,pala_a);
    printf(" ");
    gotoxy(4,pala_a+1);
    printf(" ");
    gotoxy(4,pala_a-1);
    printf(" ");

    //pala jugador b
    gotoxy(75,pala_b);
    printf(" ");
    gotoxy(75,pala_b+1);
    printf(" ");
    gotoxy(75,pala_b-1);
    printf(" ");
}

//mover paletas
void moverPala(int a){
    if(a==1 && pala_a>5){
        borrar_palas();
        pala_a--;
        pintar_palas();
    }
    if(a==2 && pala_a<21){
        borrar_palas();
        pala_a++;
        pintar_palas();
    }
    if(a==3 && pala_b>5){
        borrar_palas();
        pala_b--;
        pintar_palas();
    }
    if(a==4 && pala_b<21){
        borrar_palas();
        pala_b++;
        pintar_palas();
    }

}

//teclas pulsadas
void teclear(){
    if (kbhit()){
        tecla=getch();
        if(tecla=='q'){
            moverPala(1);
        }
        if(tecla=='a'){
            moverPala(2);
        }
        if(tecla=='o'){
            moverPala(3);
        }
        if(tecla=='l'){
            moverPala(4);
        }
    }
}


//game over
bool game_over(){
    if(a==1){
        return true;
    }else{
        return false;
    }
}

//mover pelota
void mover_bola(){
    bx=x;
    by=y;
    //direccion de la pelota
    if(dirx==1){
        x++;
    }else{
        x--;
    }

    if(diry==1){
        y++;
    }else{
        y--;
    }

    //comprobar paletas devolucion
    if(x==75 &&(y==pala_b || y == pala_b+1 || y == pala_b-1)){
        dirx=0;
    }
    if(x==4 && (y==pala_a || y == pala_a+1 || y == pala_a-1)){
        dirx=1;
    }

    //goles
    if(x==2){
        //gol
        dirx=1;
        x=35;
        y=15;
        puntos_b++;
        pinta_goles();
    }

    if(x==77){
        //gol
        dirx=0;
        x=35;
        y=15;
        puntos_a++;
        pinta_goles();
    }

    //choques techo y suelo
    if(y==22){
        diry=0;
    }
    if(y==4){
        diry=1;
    }

    //borrar pelota
    gotoxy(bx,by);printf(" ");

    //imprimir pelota
    gotoxy(x,y);printf("O");

}


int main()
{
    Client *Cliente = new Client();

        Cliente->Enviar("Aquí va el mensaje a enviar");
        Cliente->Recibir();
    while(!game_over()){
        teclear();
        mover_bola();
        Sleep(75);
    }
 OcultarCursor();
    pintar_Campo();
    pintar_palas();
    pinta_goles();


    gotoxy(33,13);

    if (puntos_a == 10){

        printf("GANA EL JUGADOR A");
    }
    else{
        printf("GANA EL JUGADOR B");
    }


    Cliente->CerrarSocket();
    getch();

    return 0;
}
