#include <iostream>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <chrono>
#include <queue>
#include <set>

using namespace std;
using namespace std::chrono;


//estructura que contiene la estructura de un Ciudad
struct Ciudad{
    float x,y;
    int indice;
};

struct ruta{
    vector<Ciudad> rutaActual;
    vector<bool> visitados;
    double costeActual;
    double costeEstimado;
};

bool operator<(const ruta &r1,const ruta &r2){
    return (r1.costeActual+r1.costeEstimado) > (r2.costeActual+r2.costeEstimado);
}

void imprimirRutaMinima(vector<Ciudad> rutaMinima){
    for(int i=0;i<rutaMinima.size();i++)
        cout <<rutaMinima[i].indice<<" "<< rutaMinima[i].x<<" "<<rutaMinima[i].y<<" "<<endl;
}

//TSP GREEDY
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

//Calculo de distancia entre Ciudads
double distancia(Ciudad p1,Ciudad p2){
    return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

double calcularCosteEstimado(const ruta &r,vector<Ciudad> ciudades,double **distancias){
    double costeEstimado=0;
    for(int i=0;i<ciudades.size();i++){
        //Si no esta le calculamos el coste estimado
        if(!r.visitados[ciudades[i].indice-1]){
            costeEstimado+=distancias[r.rutaActual.front().indice-1][ciudades[i].indice-1];
        }
    }
}

void imprimirCola(priority_queue<ruta> cola){
    priority_queue<ruta> aux=cola;
    for(int i=0;i<cola.size();i++){
        ruta rutaActual=aux.top();
        imprimirRutaMinima(rutaActual.rutaActual);
        aux.pop();
        cout <<endl;
    }
}

void tsp(vector<Ciudad> ciudades,double &costeMin,double **distancias,vector<Ciudad> & rutaMin,int &numNodosExpandidos,int &tamMaximoCola,int &numPodas){
    //Datos de la solucion
    numNodosExpandidos=0;
    numPodas=0;
    
    //Creamos una cola con prioridad de objetos ruta
    priority_queue<ruta> cola;  
    
    //Insertamos la primera ruta que empieza en la primera ciudad
    ruta nodoActual;
    nodoActual.rutaActual.push_back(ciudades.front());
    nodoActual.costeActual=0;
    for(int i=0;i<ciudades.size();i++){
        nodoActual.visitados.push_back(false);
    }
    nodoActual.visitados[ciudades.front().indice-1]=true;
    nodoActual.costeEstimado=calcularCosteEstimado(nodoActual,ciudades,distancias);
    cola.push(nodoActual);

    while(!cola.empty()){
        //IMPRIMIMOS LAS COLA DE NODOS
        if(cola.size()>tamMaximoCola){
            tamMaximoCola=cola.size();
        }

        //Extraemos el primer nodo de la cola
        nodoActual=cola.top();
        cola.pop();
        

        //Comprobamos si es un recorrido completo
        if(nodoActual.rutaActual.size()==ciudades.size()){
            //cout <<"El recorrido es completo"<<endl;
            //Calculamos la distancia entre el primero y el ultimo
            nodoActual.costeActual+=distancias[nodoActual.rutaActual.front().indice-1][nodoActual.rutaActual.back().indice-1];
            nodoActual.rutaActual.push_back(nodoActual.rutaActual.front());
            if(nodoActual.costeActual<=costeMin){
                //cout <<"La distancia del recorrido es menor que la minima: "<<nodoActual.costeActual<<" asi que actualizamos la ruta minima"<<endl;
                costeMin=nodoActual.costeActual;
                rutaMin=nodoActual.rutaActual;
            }
            
        }


        //Si el nodo tiene un coste menor al costeMin lo expandimos
        else if(nodoActual.costeActual<costeMin){
            for(int i=0;i<ciudades.size();i++){
                //Si el nodo no se ha visitado
                if(!nodoActual.visitados[ciudades[i].indice-1]){
                    ruta nodoNuevo=nodoActual;
                    nodoNuevo.visitados[ciudades[i].indice-1]=true;
                    nodoNuevo.costeActual+=distancias[nodoNuevo.rutaActual.back().indice-1][ciudades[i].indice-1];
                    //Si el coste actual es superior al coste min no lo aniadimos
                    if(nodoNuevo.costeActual<costeMin){
                        nodoNuevo.rutaActual.push_back(ciudades[i]);
                        nodoNuevo.costeEstimado=calcularCosteEstimado(nodoNuevo,ciudades,distancias);
                        //Lo aniadimos a la cola
                        cola.push(nodoNuevo);
                        numNodosExpandidos++;
                    }
   
                }
            }
        }
        //Podamos
        else{
            numPodas++;
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
    cout <<"Distancias entre ciudades"<<endl;
    for(int i=0;i<tam;i++){
        for(int j=0;j<tam;j++){
            cout <<L[i][j] << " ";
        }
        cout <<endl;
    }
    cout <<"****************"<<endl;
    
    vector<Ciudad> rutaMin;
    double costeMin;
    
    //Para inicializar el costeMin lo hacemos con el algoritmo greedy
    vector<Ciudad> greedy;
    
    tspGreedy(ciudades,L,greedy,costeMin);    


    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;
    tantes=high_resolution_clock::now(); 
    int numPodas,numNodosExpandidos,tamMaximoCola=0;
    tsp(ciudades,costeMin,L,rutaMin,numNodosExpandidos,tamMaximoCola,numPodas);
    tdespues=high_resolution_clock::now();
    transcurrido = duration_cast<duration<double>>(tdespues - tantes);      
    

    cout <<endl <<"Coste de la solucion: "<<costeMin <<endl;
    cout <<endl<<"Recorrido"<<endl;
    imprimirRutaMinima(rutaMin);
    cout <<endl;
    cout <<"Numero de podas: "<<numPodas<<endl;
    cout <<"Numero de nodos expandidos: "<<numNodosExpandidos<<endl;
    cout <<"Tamanio maximo de la cola de nodos vivos: " <<tamMaximoCola<<endl;
    cout <<"Tiempo de ejecución: " <<transcurrido.count()<<endl;
    
    //Borramos de memoria dinamica la matriz
    for(int i=0;i<tam;i++){
        delete [] L[i];
    }
    delete [] L;
    
}