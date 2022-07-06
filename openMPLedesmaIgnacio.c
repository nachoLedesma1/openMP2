#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct {
        int blanco;
        int azul;
        int rojo;
        int naranja;
        int verde;
    }Salud;
    typedef struct {
        Salud Estado;//Blanco, Azul, Rojo, Verde y Naranja
        int Edad; //en relación a la cantidad de semanas de vida
        int Herida; //ver si no poner de otro tipo
        int Enfermo; //cantidad de semanas que estuvo enferomo siendo 0 que esta sano
        int Podado;
    } Celda;

int aniosVida (int semanas){
    //48 semanas para un año (aunque es 52 en realidad preguntar cual tomar )

    return semanas/48; //como es int va a devolver solo en entero asi que si da 4,9 devuelve 4 nomas o si es 4,01 devuelve 4

}
void inicializar (Celda *ma[], int fila, int colunma){
    int ran;
    float ran2;
    for (int i = 0; i < fila; i++){
        for(int j = 0; j < colunma; j++){
            ma[i][j].Herida = rand() % 2; //random de 0 a 1
            ran = rand() % 101;// random de 0 a 100
            if(ran <= 65){//arboles sanos
                ma[i][j].Estado.verde = 1;
                ma[i][j].Estado.naranja = 0;
                ma[i][j].Estado.rojo = 0;
                ma[i][j].Estado.azul = 0;
                ma[i][j].Estado.blanco = 0;
            }
            else {
                ran2 = (rand() % 1001)/10;
                if(ran2 <= 1.5){//enfermo con sintomas
                    ma[i][j].Estado.verde = 0;
                    ma[i][j].Estado.naranja = 0;
                    ma[i][j].Estado.rojo = 1;
                    ma[i][j].Estado.azul = 0;
                    ma[i][j].Estado.blanco = 0;
                }
                else {
                    ran2 = (rand() % 1001)/10;
                    if(ran2 <= 0.5){//enfermo sin sintomas
                        ma[i][j].Estado.verde = 0;
                        ma[i][j].Estado.naranja = 1;
                        ma[i][j].Estado.rojo = 0;
                        ma[i][j].Estado.azul = 0;
                        ma[i][j].Estado.blanco = 0;
                    }
                    else{
                        ran = rand() % 101;// random de 0 a 100
                        if(ran <= 50){
                            ma[i][j].Estado.verde = 0;
                            ma[i][j].Estado.naranja = 0;
                            ma[i][j].Estado.rojo = 0;
                            ma[i][j].Estado.azul = 1;
                            ma[i][j].Estado.blanco = 0;
                        }
                        else {
                            ma[i][j].Estado.verde = 0;
                            ma[i][j].Estado.naranja = 0;
                            ma[i][j].Estado.rojo = 0;
                            ma[i][j].Estado.azul = 0;
                            ma[i][j].Estado.blanco = 1;
                        }
                    }
                }
            }
            //edades
            ran = rand() % 101;// edad de 0 a 100
            if(ran <= 30){//joven
                ma[i][j].Edad = rand() % 145; //random de 0 a 144 semanas
            }
            if(ran > 30 && ran<= 80){//adulto
                //se multilica por las 48 porque son la cantidad de semanas para hacer 1 año
                ma[i][j].Edad = rand() % (35*48 - 4*48 +1) + 4*48; //(N-M+1) + M; n mayor que m
            }
            if(ran > 80){
                ma[i][j].Edad = rand() % (100*48 - 36*48 + 1) + 36*48; // entre 36 y 100 el random
            }
            //por si geera problemas no incializar
            ma[i][j].Podado = 0;
            ma[i][j].Enfermo = 0;
        }//for j
    }//for i
}

void imprimirMatris (Celda **ma, int cant){

    printf ("La matris es la siguiente \n");
    for (int i= 0; i < cant; i++){
        for(int j = 0; j < cant; j++){
           if(ma[i][j].Estado.verde == 1){
                printf("V ");
            }
            if(ma[i][j].Estado.naranja == 1){
                printf("N ");
            }
            if(ma[i][j].Estado.rojo == 1){
                printf("R ");
            }
            if(ma[i][j].Estado.azul == 1){
                printf("A ");
            }
            if(ma[i][j].Estado.blanco == 1){
                printf("B ");
            }


        }//for j
        printf("\n");
    }//for i
}


main(int argc, char **argv){


    int t;//relog
    //cada t es igual a 1 semana
    int cantCelReal = 800;//para probar nomas despues agregar los valores reales

    int fil = cantCelReal, col = cantCelReal;

    float tiempos [10];

    Celda **matris, **matris2; //[cantCeldas][cantCeldas];

    matris = malloc (fil*sizeof(Celda *));

    for (int i=0;i<fil;i++){
        matris[i] = (Celda *) malloc (col*sizeof(Celda));
    }

    matris2 = malloc (fil*sizeof(Celda *));

    for (int i=0;i<fil;i++){
        matris2[i] = (Celda *) malloc (col*sizeof(Celda));
    }

    Salud vecino = {.blanco = 0,
                    .azul = 0,
                    .rojo = 0,
                    .naranja = 0,
                    .verde = 0};

    //enpieza el tiempo 

    for(int i = 0; i < 5; i ++){

   // for(int i = 0; i < 10; i ++){

        struct timespec begin, end; 
        clock_gettime(CLOCK_REALTIME, &begin);

        inicializar (matris, fil, col);
        //inicializar (matris2, 30, 30);
        //imprimirMatris(matris, fil);//puede ser fil o col total son lo mismo
        
        for (t = 0; t<1200; t++){
            Celda **aux; //para cuando se cambia de estado
            int  vecEnfermo, suceptibilidad, pContagio, cantVecin;
            int cantCeldas = cantCelReal-1; //porque lo hice así sin darme Cuenta
            int ran;
            Salud vecino;
            #pragma omp parallel num_threads(16) 
            {
            #pragma omp parallel for private (vecino, cantVecin)
            for(int i = 0; i<cantCelReal; i++){
                for(int j = 0; j<cantCelReal; j++){
                    vecino.blanco = 0;
                    vecino.azul = 0;
                    vecino.rojo = 0;
                    vecino.naranja = 0;
                    vecino.verde = 0;
                    if(matris[i][j].Estado.verde == 1){ // si esta sano
                        if((i == 0 || i == cantCeldas) && (j == 0 || j == cantCeldas)){ //Las cuatro esquinas
                            if(j == 0){
                                vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                            }
                            else {
                                vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;
                            }
                            if(i == 0){
                                if(j == 0){
                                    vecino.rojo = matris[i+1][j+1].Estado.rojo + vecino.rojo;
                                }
                                else {
                                    vecino.rojo = matris[i+1][j-1].Estado.rojo + vecino.rojo;
                                }
                                vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+2][j].Estado.rojo + vecino.rojo;
                            }
                            else {
                                if(j == 0){
                                    vecino.rojo = matris[i-1][j+1].Estado.rojo + vecino.rojo;
                                }
                                else {
                                    vecino.rojo = matris[i-1][j-1].Estado.rojo + vecino.rojo;
                                }
                                vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-2][j].Estado.rojo + vecino.rojo;
                            }
                            cantVecin = 5;
                        }//cuatro esquinas
                        if((i == 0 || i == cantCeldas) && (j == 1 || j == cantCeldas -1)){ //fila superior e inferior caso 7
                            if(j == 1){
                                vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                            }
                            else {
                                vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;
                            }
                            if(i == 0){ //fila superior
                                vecino.rojo = matris[i+1][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+1][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+2][j].Estado.rojo + vecino.rojo;
                            }
                            else { //fila inferior
                                vecino.rojo = matris[i-1][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-1][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-2][j].Estado.rojo + vecino.rojo;
                            }
                            cantVecin = 7;
                        }//fila superior e inferior
                        if((i == 1 || i == cantCeldas-1) && (j == 0 || j == cantCeldas)){//columna izquierda y derecha caso 7
                            if(i == 1){
                                vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+2][j].Estado.rojo + vecino.rojo;
                            }
                            else {
                                vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-2][j].Estado.rojo + vecino.rojo;
                            }
                            if(j == 0){
                                vecino.rojo = matris[i-1][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+1][j+1].Estado.rojo + vecino.rojo;
                            }
                            else {
                                vecino.rojo = matris[i-1][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+1][j-1].Estado.rojo + vecino.rojo;
                            }
                            cantVecin = 7;
                        }//columna izquierda y derecha
                        if((i == 0 || i == cantCeldas) && (j > 1 && j < cantCeldas-1)){ //demas casos 1ra fila superior e inferior
                            if(i == 0){
                                vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+2][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;

                                vecino.rojo = matris[i+1][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+1][j-1].Estado.rojo + vecino.rojo;
                            }
                            else {
                                vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-2][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;

                                vecino.rojo = matris[i-1][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-1][j-1].Estado.rojo + vecino.rojo;
                            }
                            cantVecin = 8;
                        }//demas casos 1ra fila superior e inferior
                        if((i > 1 && i < cantCeldas-1 ) && (j == 0 || j == cantCeldas)){//demas casos 1 columna izquierda y derecha
                            if(j == 0){
                                vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+2][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-2][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;

                                vecino.rojo = matris[i+1][j+1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-1][j+1].Estado.rojo + vecino.rojo;
                            }
                            else {
                                vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+2][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-2][j].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;

                                vecino.rojo = matris[i+1][j-1].Estado.rojo + vecino.rojo;
                                vecino.rojo = matris[i-1][j-1].Estado.rojo + vecino.rojo;
                            }
                            cantVecin = 8;
                        }//demas casos 1 columna izquierda y derecha
                        if((i == 1 || i == cantCeldas-1) && (j == 1 || j == cantCeldas-1)){ //Segunda casos de las esquinas
                            vecino.rojo = matris[i-1][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i-1][j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j+1].Estado.rojo + vecino.rojo;
                            if(i == 1){
                                vecino.rojo = matris[i+2][j].Estado.rojo + vecino.rojo;
                                if(j == 1){
                                    vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                                }
                                else{
                                    vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;
                                }
                            }
                            else {
                                vecino.rojo = matris[i-2][j].Estado.rojo + vecino.rojo;
                                if(j == 1){
                                    vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                                }
                                else{
                                    vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;
                                }
                            }
                            cantVecin = 10;
                        }//Segunda casos de las esquinas
                        if((i == 1 || i == cantCeldas-1) && (j > 1 && j < cantCeldas-1)){//segundo caso de superior e inferior
                            vecino.rojo = matris[i-1][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i-1][j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j+1].Estado.rojo + vecino.rojo;

                            vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;
                            if(i == 1){
                                vecino.rojo = matris[i+2][j].Estado.rojo + vecino.rojo;
                            }
                            else {
                                vecino.rojo = matris[i-2][j].Estado.rojo + vecino.rojo;
                            }
                            cantVecin = 11;
                        }//segundo caso de superior e inferior
                        if((i > 1 && i < cantCeldas-1) && (j == 1 || j == cantCeldas-1)){//segundo caso izquierda y derecha
                            vecino.rojo = matris[i-1][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i-1][j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j+1].Estado.rojo + vecino.rojo;

                            vecino.rojo = matris[i+2][j].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i-2][j].Estado.rojo + vecino.rojo;

                            if(j == 1){
                                vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                            }
                            else {
                                vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;
                            }
                            cantVecin = 11;
                        }//segundo caso izquierda y derecha
                        if(i >= 3 && i<cantCeldas-2 && j>= 3 && j<cantCeldas-2){
                            vecino.rojo = matris[i-1][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i-1][j].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i-1][j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i][j+1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j-1].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i+1][j+1].Estado.rojo + vecino.rojo;

                            vecino.rojo = matris[i+2][j].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i-2][j].Estado.rojo + vecino.rojo;

                            vecino.rojo = matris[i][j+2].Estado.rojo + vecino.rojo;
                            vecino.rojo = matris[i][j-2].Estado.rojo + vecino.rojo;

                            cantVecin = 12;
                        }

                //---------------------------------------------------------------------------
                        //#pragma omp critical 
                           // {
                            vecEnfermo = vecino.rojo / cantVecin; // saca un porcentaje de todos los enfermos
                            matris2[i][j].Estado.verde = 1;
                            if(matris[i][j].Herida == 1 && t != 0){ //  t!= 0 por si en la primera iteración ya tenia un aherida para no curarla porque no paso 1 sematrisna
                                // supongo de que ya paso 1 semana de que tuvo la herida asi que ya se sanó
                                matris2[i][j].Herida = 0;
                            }
                            if(aniosVida(matris[i][j].Edad) <= 3 ){//arol joven

                                suceptibilidad = 0.35; //por ser joven
                                if(matris[i][j].Herida == 1){
                                    suceptibilidad = suceptibilidad + 0.15;
                                }

                                //actualizaciones de estados
                                pContagio = ((vecEnfermo + suceptibilidad)*0.60) + 0.07;
                                ran = rand() % 101;// random de 0 a 100
                                if (ran <= 23 && ran != 0){ // es decir que que tiene una herida
                                    matris2[i][j].Herida = 1;
                                }
                                ran = rand() % 101;
                                if (ran <= pContagio){
                                    matris2[i][j].Estado.verde = 0;
                                    matris[i][j].Estado.verde = 0;

                                    matris2[i][j].Estado.naranja = 1;
                                    matris2[i][j].Enfermo ++;// aumento 1 porque va a pasar una semana en el proximo clock
                                }
                            }
                            if(aniosVida(matris[i][j].Edad) <= 35 && aniosVida(matris[i][j].Edad) > 3){

                                suceptibilidad = 0.17; //por ser adulto chiquito

                                if(matris[i][j].Herida == 1){
                                    suceptibilidad = suceptibilidad + 0.15;
                                }

                                //actualizaciones de estados
                                pContagio = ((vecEnfermo + suceptibilidad)*0.60) + 0.07;

                                ran = rand() % 101;// random de 0 a 100
                                if (ran <= 8 && ran != 0 ){ // es decir que que tiene una herida
                                    matris2[i][j].Herida = 1;
                                }
                                ran = rand() % 101;
                                if (ran <= pContagio){
                                    matris2[i][j].Estado.verde = 0;
                                    matris[i][j].Estado.verde = 0;                        

                                    matris2[i][j].Estado.naranja = 1;
                                   

                                    matris2[i][j].Enfermo ++;// aumento 1 porque va a pasar una semana en el proximo clock
                               }

                            }
                            if(aniosVida(matris[i][j].Edad) > 35){

                                suceptibilidad = 0.63; //por ser adulto viejo

                                if(matris[i][j].Herida == 1){
                                    suceptibilidad = suceptibilidad + 0.15;
                                }

                                //actualizaciones de estados
                                pContagio = ((vecEnfermo + suceptibilidad)*0.60) + 0.07;

                                ran = rand() % 101;// random de 0 a 100
                                if (ran <= 37 && ran != 0 ){ // es decir que que tiene una herida
                                    matris2[i][j].Herida = 1;
                                }
                                ran = rand() % 101;
                                if (ran <= pContagio){
                                    matris2[i][j].Estado.verde = 0;
                                    matris[i][j].Estado.verde = 0;                        

                                    matris2[i][j].Estado.naranja = 1;               
                
                                    matris2[i][j].Enfermo ++;// aumento 1 porque va a pasar una semana en el proximo clock
                                }
                            }
                            
                    }//si esta sano
                    //}
            //--------------------------------------------------------------------------------------------------------------
                    //#pragma omp critical 
                       // {
                        if(matris[i][j].Estado.naranja == 1){ //infectado con esporas
                            matris2[i][j].Estado.naranja = 1;
                            if(matris[i][j].Enfermo < 3){
                                matris2[i][j].Enfermo ++;// aumento 1
                           }
                           else {//de infectado con esporas a enfermo con síntomas
                               matris2[i][j].Estado.naranja = 0;
                               matris[i][j].Estado.naranja = 0;

                               matris2[i][j].Estado.rojo = 1;
                               matris2[i][j].Enfermo ++;// aumento 1
                           }
                        }
                        if(matris[i][j].Estado.rojo == 1){//enfermo con síntomas
                            matris2[i][j].Estado.rojo = 1;
                            ran = rand() % 101;// random de 0 a 100
                            if(ran <= 85){// 85% de que esto ocurra paso de enfermo con síntomas a enfermo con tratamiento
                                matris2[i][j].Estado.rojo = 0;
                                matris[i][j].Estado.rojo = 0;

                                matris2[i][j].Estado.azul = 1;
                            }
                            matris2[i][j].Enfermo ++;// aumento 1
                        }
                        if(matris[i][j].Estado.azul == 1){//enfermo con tratamiento antifúngico
                            matris2[i][j].Estado.azul = 1;
                            ran = rand() % 101;// random de 0 a 100
                            if(aniosVida(matris[i][j].Edad)<= 3){ //arbol joven
                                if(ran <= 3){//si tiene resistencia al hongo

                                    if(matris2[i][j].Enfermo <= 8){
                                        matris2[i][j].Estado.azul = 0;
                                        matris[i][j].Estado.azul = 0;

                                        matris2[i][j].Estado.verde = 1;//pasa a sano con la misma edad
                                        matris2[i][j].Enfermo = 0;//0 porque ya no está más enfermo en el proximo clock
                                    }
                                    else{//se poda el arbol porque ya pasaron 8 o más semanas del contagio
                                        matris2[i][j].Estado.azul = 0;
                                        matris[i][j].Estado.azul = 0;                

                                        matris2[i][j].Estado.blanco = 1;//podado
                                        matris2[i][j].Enfermo = 0; //0 porque ya no esta más enfermo en el proximo clock
                                        matris2[i][j].Podado = 1; //al proximo clock va a ser 1 semana de ser podado
                                    }
                                }
                                else{//en esta semana no tiene resistencia
                                    matris2[i][j].Enfermo ++; //pasa el tiempo nomas
                                }
                            }
                            if(aniosVida(matris[i][j].Edad) <= 35 && aniosVida(matris[i][j].Edad) > 3){//adulto
                                if(ran <= 15){//si tiene resistencia al hongo
                                    if(matris2[i][j].Enfermo <= 8){
                                        matris2[i][j].Estado.azul = 0;
                                        matris[i][j].Estado.azul = 0;

                                        matris2[i][j].Estado.verde = 1;//pasa a sano con la misma edad
                                        matris2[i][j].Enfermo = 0;//0 porque ya no está más enfermo en el proximo clock
                                    }
                                    else{//se poda el arbol porque ya pasaron 8 o más semanas del contagio
                                        matris2[i][j].Estado.azul = 0;
                                        matris[i][j].Estado.azul = 0;

                                        matris2[i][j].Estado.blanco = 1;//podado
                                        matris2[i][j].Enfermo = 0; //0 porque ya no esta más enfermo en el proximo clock
                                        matris2[i][j].Podado = 1; //al proximo clock va a ser 1 semana de ser podado
                                    }
                                }
                                else{//En esta semana no tiene resistencia
                                    matris2[i][j].Enfermo ++; //pasa el tiempo nomas
                                }

                            }
                            if(aniosVida(matris[i][j].Edad) > 35){//viejo
                                if(ran <= 53){//si tiene resistencia al hongo
                                    if(matris2[i][j].Enfermo <= 8){
                                        matris2[i][j].Estado.azul = 0;
                                        matris[i][j].Estado.azul = 0;

                                        matris2[i][j].Estado.verde = 1;//pasa a sano con la misma edad
                                        matris2[i][j].Enfermo = 0;//0 porque ya no está más enfermo en el proximo clock
                                    }
                                    else {// se reemplaza el arbol por uno nuevo
                                        matris2[i][j].Estado.azul = 0;
                                        matris[i][j].Estado.azul = 0;

                                        matris2[i][j].Estado.verde = 1;
                                        matris2[i][j].Edad = 48;// 1 año de edad
                                        matris2[i][j].Enfermo = 0;//0 porque ya no está más enfermo en el proximo clock
                                    }
                                }
                                else{//En esta semana no tiene resistencia
                                    matris2[i][j].Enfermo ++; //pasa el tiempo nomas
                                }
                            }
                        }
                        if(matris[i][j].Estado.blanco == 1){// arbol podado
                            matris2[i][j].Estado.blanco = 1;
                            if(matris2[i][j].Podado == 12){
                                //de podado a sano
                                matris2[i][j].Estado.blanco = 0;
                                matris[i][j].Estado.blanco = 0;

                                matris2[i][j].Estado.verde = 1;
                                matris2[i][j].Podado = 0;//para reiniciar la variable nomas
                            }
                            else{
                                matris2[i][j].Podado ++;
                            }
                        }
                    //}//critical
                }//for j
            }//for i

            //se actualizo ma2 y ahora solo tengo que cambiar el ṕuntero
            
            aux = matris;
            matris = matris2;
            matris2 = aux;

            }//del numero de threat          
        }//for t= tiempo de vida       
    
        //para probar no hay que imprimir nada
        
        //imprimirMatris(matris, fil);//puede ser fil o col total son lo mismo
        //Termina el tiempo
        clock_gettime(CLOCK_REALTIME, &end);
        long seg = end.tv_sec - begin.tv_sec;
        long nanoseg = end.tv_nsec - begin.tv_nsec;
        double total = seg + nanoseg*1e-9;
        //tiempos[i] = total;
        printf("El tiempo es %f\n ", total);
        //printf("Tiempo: %.3f segundos.\n", total);
    }//for cant de repes 
   /* printf("Los tiempos son: ");
    for(int j = 0; j<10; j++){
        printf("%f, ",tiempos[j]);
    } 
    printf("\n");*/
    //libero memoria
    for (int i= 0; i<fil; i++){//puede ser fil o col total son lo mismo
    free(matris[i]);
    free(matris2[i]);
    }

    free(matris);
    free(matris2);


}

