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
//estructura que contiene la estructura de un Ciudad
struct Ciudad{
    float x,y;
    int indice;
};

//Calculo de distancia entre Ciudads
double distancia(Ciudad p1,Ciudad p2){
    return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

void tsp(vector<Ciudad> N, vector<Ciudad> &S, double &distanciaRecorrida,double *L[]){
	
	vector<Ciudad> vaux, vmin;
	vector<Ciudad> naux;
	double aux, min=numeric_limits<double>::max();
	
	
	//CASO BASE queda una ciudad sin recorrer
	if (N.empty()){
		distanciaRecorrida+=L[S.front().indice -1][S.back().indice -1];
		S.push_back(S.front());
	}
	else{
		for(auto it=N.begin() ; it!=N.end() ; ++it){
            aux=L[(it->indice)-1][(S.back().indice)-1];
			//distancia(*it,S.back());
			vaux = S;
			naux = N;
			
			vaux.push_back(*it);
			N.erase(it);
			
			tsp(N,vaux,aux,L);
			
			if(aux < min){
				min = aux;
				vmin = vaux;
			}
			
			N=naux;
		}
		
		distanciaRecorrida += min;
		S = vmin;
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
    
    vector<Ciudad> S;
    double distanciaRecorrida=0;
    S.push_back(ciudades[0]);
    ciudades.erase(ciudades.begin());
    int contador=0;
    
    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;
    tantes=high_resolution_clock::now(); 
    
    tsp(ciudades,S,distanciaRecorrida,L);
    
    tdespues=high_resolution_clock::now();
    transcurrido = duration_cast<duration<double>>(tdespues - tantes);      
    
    
    cout << ciudades.size() +1 <<"\t"<<transcurrido.count()<<endl;

    cout <<endl <<"Solucion: "<<distanciaRecorrida <<endl;

    for(int i=0;i<S.size();i++){
        cout << S[i].x<<" "<<S[i].y<<" "<<endl;
    }
    
    
    //Borramos de memoria dinamica la matriz
    for(int i=0;i<tam;i++){
        delete [] L[i];
    }
    delete [] L;
    
}
