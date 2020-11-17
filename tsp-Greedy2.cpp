//COMPILACION: g++ -std=c++11 -O2 -o cercania cercania.cpp
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

//Calculo de distancia entre Ciudads
double distancia(Ciudad p1,Ciudad p2){
    return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

//Devuelve la posicion de la ciudad mas cercana
void ciudadMasCercana(vector<Ciudad> &ciudades,vector<Ciudad> &solucion,vector<Ciudad>::iterator &ciudadComparada,double &distanciaTotal){
    double distanciaActual,distanciaCiudadMasCercana;
    vector<Ciudad>::iterator ciudadMasCercana;
    

    //Insertamos la ciudadActual a solucion y la borramos de ciudades
    solucion.push_back(*ciudadComparada);

    //Eliminamos de ciudades la ciudadActual
    ciudades.erase(ciudadComparada);


    distanciaCiudadMasCercana=distancia(ciudades[0],solucion.back());
    ciudadMasCercana=ciudades.begin();
    auto it=ciudades.begin();
    //El primer elemento nos lo saltamos
    ++it;

    for(;it!=ciudades.end();++it){
        distanciaActual=distancia(*it,solucion.back());
        if(distanciaActual<distanciaCiudadMasCercana){
            ciudadMasCercana=it;
            distanciaCiudadMasCercana=distanciaActual;
        }  
        
    }
    distanciaTotal+=distanciaCiudadMasCercana;
    
    ciudadComparada=ciudadMasCercana;
}


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
    
    //Cada posicion del vector representa una ciudad
    //La ciudad viene representada por las coordenadas x e y

    auto ciudadActual = ciudades.begin();
    double distanciaTotal=0;

    while(ciudades.size()>1){

        ciudadMasCercana(ciudades,solucion,ciudadActual,distanciaTotal);        
    }
    
    //Insertamos la ultima ciudad libre
    solucion.push_back(ciudades[0]);

    distanciaTotal+=distancia(ciudades[0],solucion[0]);
    
    auto it=solucion.begin();
    for (;it!=solucion.end();++it){
        cout << it->indice << endl;
    }
    cout <<endl;
    cout <<"La distancia total del recorrido es: "<<distanciaTotal<<endl;
    
}