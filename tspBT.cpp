#include <iostream>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

int paso=1;
//estructura que contiene la estructura de un Ciudad
struct Ciudad{
    float x,y;
    int indice;
};

//TSP GREEDY PARA CALCULAR LA FUNCION DE ACOTACION
Ciudad calcularCiudadMasCercana(vector<Ciudad> ciudades,vector<bool> visitados,Ciudad actual,double **distancia){
    Ciudad min;
    double distanciaMin=numeric_limits<double>::max();

    for(int i=0;i<ciudades.size();i++){
        if(!visitados[ciudades[i].indice-1] && ciudades[i].indice != actual.indice){
            if(distancia[ciudades[i].indice-1][actual.indice-1] < distanciaMin){
                distanciaMin=distancia[ciudades[i].indice-1][actual.indice-1];
                min=ciudades[i];
            }
        }
    }
    return min;
}

bool hayCiudades(vector<bool> visitados){
    bool resultado=false;
    for(int i=0;i<visitados.size();i++){
        if(!visitados[i]){
            resultado=true;
        }
    }
    return resultado;
}


void tspGreedy(vector<Ciudad> ciudades,double **distancia,vector<Ciudad> & solucion,double &distanciaCamino){
    vector<bool> visitados;
    distanciaCamino=0;
    for(int i=0;i<ciudades.size();i++){
        visitados.push_back(false);
    }
    visitados[ciudades.front().indice-1]=true;
    solucion.push_back(ciudades.front());

    while(hayCiudades(visitados)){
        //Escogemos la ciudad mas cercana
        Ciudad masCercana=calcularCiudadMasCercana(ciudades,visitados,solucion.back(),distancia);
        distanciaCamino+=distancia[solucion.back().indice-1][masCercana.indice-1];
        solucion.push_back(masCercana);
        visitados[masCercana.indice-1]=true;
    }
    //Aniadimos la ultima ciudad
    distanciaCamino+=distancia[solucion.front().indice-1][solucion.back().indice-1];
    solucion.push_back(solucion.front());
}



//FUNCIONES PARA CALCULAR TSP BACKTRACING
//Calculo de distancia entre Ciudads
double distancia(Ciudad p1,Ciudad p2){
    return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

void imprimirRutaMinima(vector<Ciudad> rutaMinima){
    for(int i=0;i<rutaMinima.size();i++)
        cout <<rutaMinima[i].indice<<" "<< rutaMinima[i].x<<" "<<rutaMinima[i].y<<" "<<endl;
}

//TSP USANDO BACKTRACING
void tsp(vector<Ciudad> ruta, vector<Ciudad>noVisitadas,double &costeMinimo,vector<Ciudad> &rutaMinima ,double costeActual, double **distancia,int numCiudades){
    //Caso base
    if(ruta.size()==numCiudades){
        costeActual+=distancia[ruta.front().indice-1][ruta.back().indice-1];
        ruta.push_back(ruta.front());
        if(costeActual<=costeMinimo){
            costeMinimo=costeActual;
            rutaMinima=ruta;
        }
    }
    
    //Backtracing
    else{
        //Para cada una de las posibles soluciones
        for(auto it=noVisitadas.begin() ;it!=noVisitadas.end();++it){
            //Probamos esta opcion
            vector<Ciudad> auxRuta=ruta;
            vector<Ciudad> auxNoVisitadas=noVisitadas;
            //Comprobamos si la solucion tiene mayor coste que la solucion minima
            //Si es asi dejamos de comprobar
            double costeAux=costeActual + distancia[ruta.back().indice -1][it->indice -1];
            if(costeAux < costeMinimo){
                //Aniadimos a auxRuta
                auxRuta.push_back(*it);
            
                //Borramos de auxNoVisitadas
                noVisitadas.erase(it);
                
                //Calculamos la ruta
                tsp(auxRuta,noVisitadas, costeMinimo,rutaMinima, costeAux, distancia, numCiudades);

                noVisitadas=auxNoVisitadas;
            }
        }
    }
}


int main(int argc,char **argv){
     if(argc<2){
        cout<<"Falta el fichero de texto.)";
        exit(-1);
    }
    string fichero=argv[1];

    //Definimos un vector de la stl
    vector<Ciudad> ciudades;

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

    /*
    Definimos una matriz L donde L[i][j] son las distancias
    entre la ciudad i y la ciudad j.
    L[i][i] valdra 0
    */

    int tam=ciudades.size();
    
  	double** L = new double*[tam];
  	
	for(int i = 0; i < tam; ++i)
    	L[i] = new double[tam];
    	
    for(int i=0;i<tam;i++){
    	for(int j=0;j<tam;j++){
    		L[i][j]= 0;
		}
	}
    //Calculamos las distancias y las ponemos en la matriz
    double distanciaActual;
    for(int i=0;i<tam;i++){
        for(int j=0;j<tam;j++){
            if(i!=j){
                distanciaActual=distancia(ciudades[i],ciudades[j]);
                L[i][j]=distanciaActual;
            }
        }
    }

    cout <<"Distancia entre ciudades"<<endl;
    for(int i=0;i<tam;i++){
        for(int j=0;j<tam;j++){
            cout <<L[i][j]<<" ";
        }
        cout <<endl;
    }
    cout <<"*****************"<<endl;
    //Inicializamos costeMin con el algoritmo greedy
    double costeMinimo;
    vector<Ciudad> greedy;
    tspGreedy(ciudades,L,greedy,costeMinimo);

    vector<Ciudad> ruta,rutaMinima;
    Ciudad ciudadPrimera=ciudades.front();
    ciudades.erase(ciudades.begin());
    ruta.push_back(ciudadPrimera);

    double costeActual=0;
    int numCiudades=ciudades.size()+1;

    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;
    tantes=high_resolution_clock::now(); 
    
    tsp(ruta,ciudades,costeMinimo,rutaMinima,costeActual,L,numCiudades);
    tdespues=high_resolution_clock::now();
    transcurrido = duration_cast<duration<double>>(tdespues - tantes);      

    cout <<endl <<"Coste de la solucion: "<<costeMinimo <<endl;
    cout <<endl<<"Recorrido"<<endl;
 

    imprimirRutaMinima(rutaMinima);
    cout <<"Tiempo de ejecución: "<<transcurrido.count()<<endl;
    //Borramos de memoria dinamica la matriz
    for(int i=0;i<tam;i++){
        delete [] L[i];
    }
    delete [] L;
    
}