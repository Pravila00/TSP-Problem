//COMPILACION: g++ -std=c++11 -O2 -o insercion insercion.cpp
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#include <string.h>
#include <stdio.h>

using namespace std;
//estructura que contiene la estructura de un Ciudad
struct Ciudad{
    float x,y;
    int indice;
};

//Calculo de distancia entre Ciudades
double distancia(Ciudad p1,Ciudad p2){
    return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

//Calcula la ciudad mas al oeste, al este y al norte
void calculoABC(Ciudad &A,Ciudad &B,Ciudad &C,vector<Ciudad> &ciudades){
    A=ciudades[0];
    B=ciudades[0];
    C=ciudades[0];
    vector<Ciudad>::iterator it=ciudades.begin();
    vector<Ciudad>::iterator auxA=ciudades.begin();
    vector<Ciudad>::iterator auxB=ciudades.begin();
    vector<Ciudad>::iterator auxC=ciudades.begin();
    ++it;
    /*
        Primero recorremos el vector para encontrar A(el punto más al norte) y B(el punto más al Sur) ya que al ser puntos contrarios
        no tendremos el problema de que sean el mismo punto como puede ocurrir con C(el punto más Oeste).
        Una vez hemos borrado A y B al hacer otro for para buscar el C, no tendremos el problema de que C pueda ser igual que A o
        igual que C.
    */
    for(;it!=ciudades.end();++it){
        if(it->y > auxA->y){
            auxA=it;
        } 
        if(it->y < auxB->y){
            auxB=it;
        }  
    }
    A=*auxA;
    B=*auxB;
    ciudades.erase(auxA);
    ciudades.erase(auxB); 
    for(it=ciudades.begin();it!=ciudades.end();++it){
        if(it->x < auxC->x){
            auxC=it;
        }
    }
    C=*auxC;
    ciudades.erase(auxC);
}

//Devuelve un iterador a la ciudad mas cercana a un punto
vector<Ciudad>::iterator ciudadMasCercana(Ciudad &ciudadComparada,vector<Ciudad> &ciudades,double &distanciaTotal){
    
    double distanciaActual,distanciaCiudadMasCercana;
    vector<Ciudad>::iterator ciudadMasCercana;

    distanciaCiudadMasCercana=distancia(ciudades[0],ciudadComparada);
    ciudadMasCercana=ciudades.begin();
    auto it=ciudades.begin();
    ++it;

    for(;it!=ciudades.end();++it){
        distanciaActual=distancia(*it,ciudadComparada);
        if(distanciaActual<distanciaCiudadMasCercana){
            ciudadMasCercana=it;
            distanciaCiudadMasCercana=distanciaActual;
        }  
        
    }
    distanciaTotal=distanciaCiudadMasCercana;
    
    return ciudadMasCercana;
}

//Devuelve la posicion de la ciudad mas cercana

int main(int argc,char**argv){

    if(argc<2){
        cout<<"Falta el fichero de texto.)";
        exit(-1);
    }
    string fichero=argv[1];

    //Definimos un vector de la stl
    vector<Ciudad> ciudades;
    vector<Ciudad> solucion;

    //Procedemos a la lectura del fichero
    ifstream ficheroEntrada;
    string nombreFichero,comentario,tipo,dimension,weight_type,node;

    //Abrimos el fichero
    ficheroEntrada.open(fichero);

    if(!ficheroEntrada.is_open()){
        exit(-1);
    }

    //Leemos el nombre del fichero
    getline(ficheroEntrada,nombreFichero);
    getline(ficheroEntrada,comentario);
    getline(ficheroEntrada,tipo);
    getline(ficheroEntrada,dimension);
    getline(ficheroEntrada,weight_type);
    getline(ficheroEntrada,node);
    
    while(!ficheroEntrada.eof()){
        
       char num_ciudad[16];
       char xChar[16];
       char yChar[16];
       ficheroEntrada >> num_ciudad;
       
       ficheroEntrada >> xChar;
       ficheroEntrada >> yChar;


        Ciudad p;
        p.indice=atoi(num_ciudad);
        p.x=atof(xChar);
        p.y=atof(yChar);
        
        ciudades.push_back(p);
    }

    //Eliminamos el ultimo porque se introduce un Ciudad con el valor EOF por el fichero
	ciudades.pop_back();

    //Cerramos el fichero
    ficheroEntrada.close();
    
    double distanciaTotal=0;

    //Empezamos con estos tres pivotes
    Ciudad A,B,C;

    /*
    A-> la ciudad mas al norte
    B-> la ciudad mas al sur
    C-> la ciudad mas al oeste
    */

    calculoABC(A,B,C,ciudades);
    
    
    solucion.push_back(A);
    solucion.push_back(B);
    solucion.push_back(C); 
    distanciaTotal+=distancia(A,B);
    distanciaTotal+=distancia(B,C);
    double distanciaA,distanciaB,distanciaC;
    /*  
    1- Obtener la ciudad mas cercana A, las mas cercana a B
       y la mas cercana a C
    2- Nos quedamos con la que aumente menos distancia en distanciaTotal
    3- Asignamos a esta ciudad como sucesora de la anterior
    4- Eliminamos esta ciudad de ciudades y la añadimos a solucion
    */
   

    while(ciudades.size()>0){
        //PRIMER PASO
        auto itA=ciudadMasCercana(A,ciudades,distanciaA);
        auto itB=ciudadMasCercana(B,ciudades,distanciaB);
        auto itC=ciudadMasCercana(C,ciudades,distanciaC);

        //SEGUNDO, TERCER Y CUARTO PASO
        if(distanciaA > distanciaB && distanciaA > distanciaC){
            solucion.push_back(*itA);
            A=*itA;
            ciudades.erase(itA);
            distanciaTotal+=distanciaA;
        }
        else if(distanciaB > distanciaA && distanciaB > distanciaC){
            solucion.push_back(*itB);
            B=*itB;
            ciudades.erase(itB);
            distanciaTotal+=distanciaB;
        }
        else{
            solucion.push_back(*itC);
            C=*itC;
            ciudades.erase(itC);
            distanciaTotal+=distanciaC;
        }
    }
    
    distanciaTotal+=distancia(solucion.back(),solucion[0]);
    
    //Mostrar la solucion
    auto it=solucion.begin();
    for (;it!=solucion.end();++it){
        cout << it->indice << endl;
    }
    cout <<endl;
    cout <<"La distancia total del recorrido es: "<<distanciaTotal<<endl;
    
}