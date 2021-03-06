//4 mochilas en 4 hilos
//la matriz matrizMejoresValores es un vector de vector de enteros, esto evita el desbordamiento en la pila que dan 
//las llamadas recursivas en la implementacion tradicional de la mochila
//al lanzar 4 mochilas en paralelo se consume mas RAM pero esperamos minimizarlo con la pereza
//al usar bucle para recoger las 4 lanzadas en paralelo hacemos una especie de wait
#include <vector>
#include <chrono>
#include <iostream> //cout
#include <future> //async

bool todasTerminadas(const std::vector<bool> & b_tareas){
    bool b_accum = true;
    for(auto b_tarea : b_tareas) {
        b_accum &= b_tarea;
    }
    return b_accum;
}

//solucion no recursiva de la mochila con memoria dinamica para evitar stackoverflow
int mochila(const std::vector<int> & pesos, const std::vector<int> & valores, const int capacidad) {
    const int numeroElementos = pesos.size();
    int sinElementos[capacidad];
    for(int i=0;i<capacidad;i++)sinElementos[i]=0;
    std::vector<int> vi_sinElementos(sinElementos, sinElementos + sizeof(sinElementos) / sizeof(int) );
    std::vector<std::vector<int>> matrizMejoresValores(1,vi_sinElementos);
    for(int indiceElemento = 1;indiceElemento<numeroElementos;indiceElemento++){
        std::vector<int> fila(1,0);//crea tantas filas(vacias) como elementos hay para elegir
        for(int capacidadActual = 1;capacidadActual<capacidad;capacidadActual++){
            int valor = 0;
            int pesoElemento = pesos[indiceElemento];
            if (capacidadActual<pesoElemento){
                //no mete el elemento
                valor = matrizMejoresValores[indiceElemento-1][capacidadActual];
            }else{
                //si meter el elemento da mas que no meterlo
                if( matrizMejoresValores[indiceElemento-1][capacidadActual-pesoElemento] + valores[indiceElemento] > matrizMejoresValores[indiceElemento-1][capacidadActual] ){
                    //mete el elemento
                    valor = matrizMejoresValores[indiceElemento-1][capacidadActual-pesoElemento] + valores[indiceElemento];
                }
                else {
                    //no mete el elemento
                    valor = matrizMejoresValores[indiceElemento-1][capacidadActual];
                }
            }
            fila.push_back(valor);//apila el mejor valor de la mochila para la iteracion x,y donde 
            //x es el indiceElemento e y es la capacidadActual
        }
        matrizMejoresValores.push_back(fila);
    }
    int temp = 0;
    int i = numeroElementos-1;
    int j = capacidad-1;
    if(static_cast<int>(matrizMejoresValores.size())>i && i>=0){
        if(static_cast<int>(matrizMejoresValores[i].size())>j && j>=0){
           temp  = matrizMejoresValores[i][j];
            //la ultima posicion de la matriz guarda el mejor valor para 
            //todos los elementos para la capacidad de la mochila
        }
    }
    return temp;
}

//ejecucion de una mochila
void unaMochila(void){
    int a_valores1[] = {0,9,13,153,50,15,68,27,39,23,52,11,32,24,48,73,42,43,22,7,18,4,30};
    int a_pesos1[] = {0,150,35,200,160,60,45,60,40,30,10,70,30,15,10,40,70,75,80,20,12,50,10};
    std::vector<int> pesos1 (a_pesos1, a_pesos1 + sizeof(a_pesos1) / sizeof(int) );
    std::vector<int> valores1 (a_valores1, a_valores1 + sizeof(a_valores1) / sizeof(int) );
    const int capacidad1=10000;

    auto t_empieza = std::chrono::steady_clock::now();
    int v = mochila(pesos1,valores1,capacidad1);
    auto t_termina = std::chrono::steady_clock::now();
    auto t_duracion = t_termina - t_empieza;
    std::cout << std::chrono::duration <double, std::milli> (t_duracion).count() << " ms" << std::endl;
    std::cout << v << std::endl;
}

//ejecucion de 4 mochilas en paralelo
void cuatroMochilas(void){
    int resulMin = 2222;//por poner un numero

    std::vector<std::future<int>> tareas;
    int a_valores1[] = {0,9,13,153,50,15,68,27,39,23,52,11,32,24,48,73,42,43,22,7,18,4,30};
    int a_pesos1[] = {0,20,35,20,16,16,15,16,24,23,10,17,13,15,10,24,27,17,18,20,12,35,10};
    std::vector<int> pesos1 (a_pesos1, a_pesos1 + sizeof(a_pesos1) / sizeof(int) );
    std::vector<int> valores1 (a_valores1, a_valores1 + sizeof(a_valores1) / sizeof(int) );
    const int capacidad1=10000;

    int a_valores2[] = {0,3,4,5,8,10};
    int a_pesos2[] = {0,2,3,4,5,9};
    std::vector<int> pesos2 (a_pesos2, a_pesos2 + sizeof(a_pesos2) / sizeof(int) );
    std::vector<int> valores2 (a_valores2, a_valores2 + sizeof(a_valores2) / sizeof(int) );
    const int capacidad2=20;

    int a_valores3[] = {0,1,2,3};
    int a_pesos3[] = {0,3,2,1};
    std::vector<int> pesos3 (a_pesos3, a_pesos3 + sizeof(a_pesos3) / sizeof(int) );
    std::vector<int> valores3 (a_valores3, a_valores3 + sizeof(a_valores3) / sizeof(int) );
    const int capacidad3=5;

    int a_valores4[] = {0,9,13,153,50,15,68,27,39,23,52,11,32,24,48,73,42,43,22,7,18,4,30};
    int a_pesos4[] = {0,150,35,200,160,60,45,60,40,30,10,70,30,15,10,40,70,75,80,20,12,50,10};
    std::vector<int> pesos4 (a_pesos4, a_pesos4 + sizeof(a_pesos4) / sizeof(int) );
    std::vector<int> valores4 (a_valores4, a_valores4 + sizeof(a_valores4) / sizeof(int) );
    const int capacidad4=10000;

    auto t_empieza = std::chrono::steady_clock::now();

    //lanza los 4 hilos
    tareas.push_back(
                     std::async(
                            std::launch::async,
                            [pesos1,valores1,capacidad1](){
                                return mochila(pesos1,valores1,capacidad1);
                            }
                          )
                    );
    tareas.push_back( std::async(std::launch::async,
                                 [pesos2,valores2,capacidad2](){return mochila(pesos2,valores2,capacidad2);}) );
    tareas.push_back( std::async(std::launch::async,
                                 [pesos3,valores3,capacidad3](){return mochila(pesos3,valores3,capacidad3);}) );
    tareas.push_back( std::async(std::launch::async,
                                 [pesos4,valores4,capacidad4](){return mochila(pesos4,valores4,capacidad4);}) );

    //espera a que terminen los 4 hilos
    std::vector<bool>b_tareas(4,false);
    int i=0;
    while(!todasTerminadas(b_tareas)) {
        if(tareas[i].wait_for(std::chrono::seconds(0))==std::future_status::ready){
            b_tareas[i]=true;
        }
        i++;
        if(i==4)i=0;
    }
    //recoge las soluciones de los 4 hilos
    for(auto &tarea : tareas) {
        resulMin = std::min(resulMin,tarea.get());
    }

    auto t_termina = std::chrono::steady_clock::now();
    auto t_duracion = t_termina - t_empieza;
    std::cout << std::chrono::duration <double, std::milli> (t_duracion).count() << " ms" << std::endl;
    std::cout << resulMin << std::endl;
}

int main(int argc,char** argv){
   cuatroMochilas();
}
