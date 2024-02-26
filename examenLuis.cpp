// examenLuis.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

//constantes
const int MAX_FURGOS = 10;
const int MAX_PAQUETES = 50;
const string CENTINELA = "XXX";

//tipos
typedef enum tDistrito {Centro, Salamanca, Moncloa, Retiro};
struct tPaquete {
    string codigo;
    tDistrito distrito;
    double tamano;
};
struct tListaPaquetes {
    tPaquete paquetes[MAX_PAQUETES];
    int cont = 0;
};
struct tFurgoneta {
    tDistrito distrito;
    double capacidad = 1.5;
    tListaPaquetes cargados;
};
struct tListaFurgos {
    tFurgoneta furgonetas [MAX_FURGOS];
    int cont = 0;
};

//prototipos
    //añade en la lista los paquetes con la info de paquetes.txt. Si se puede abrir devuelve true y false si no
    //si hay más paquetes que el MAX_PAQUETES, se ignoran
bool cargar(tListaPaquetes& listaPaquetes);
void mostrar(const tListaPaquetes& listaPaquetes);
string distrito(tDistrito distrito);
int siguiente(const tListaPaquetes& listaPaquetes);
void eliminar(tListaPaquetes& listaPaquetes, int pos);
void agregar(tListaPaquetes& listaPaquetes, tPaquete paquete);
int buscar(const tListaFurgos& listaFurgos, tPaquete paquete);
void asignar(tListaPaquetes& listaPaquetes, tListaPaquetes& listaNoAsignados, tListaFurgos& listaFurgos);


int main()
{
    tListaPaquetes listaPaquetes;
    cargar(listaPaquetes);
    mostrar(listaPaquetes);
    cout << siguiente(listaPaquetes) << endl;
    eliminar(listaPaquetes, siguiente(listaPaquetes));
    mostrar(listaPaquetes);
}

void mostrar(const tListaPaquetes& listaPaquetes) {
    if (listaPaquetes.cont == 0) {
        cout << "No hay paquetes" << endl;
    }
    else {
        for (int i = 0; i < listaPaquetes.cont; i++)
            cout << listaPaquetes.paquetes[i].codigo << " " << setw(12) << left << distrito(listaPaquetes.paquetes[i].distrito) << " " << listaPaquetes.paquetes[i].tamano << "m3" << endl;
    }
}


bool cargar(tListaPaquetes& listaPaquetes) {
    bool ok = false;
    ifstream archivo;
    tPaquete paquete;
    archivo.open("paquetes.txt");
    if (archivo.is_open()) {
        ok = true;
        archivo >> paquete.codigo;
        while (paquete.codigo != CENTINELA && listaPaquetes.cont < MAX_PAQUETES) {
            int distrito;
            archivo >> distrito; 
            paquete.distrito = (tDistrito)distrito;
            archivo >> paquete.tamano;
            listaPaquetes.paquetes[listaPaquetes.cont] = paquete;
            listaPaquetes.cont++;
            archivo >> paquete.codigo;
        }
        archivo.close();
    }
    return ok;
}

string distrito(tDistrito distrito) {
    string dist;
    switch (distrito) {
    case 0:
        dist = "Centro";
        break;
    case 1:
        dist = "Salamanca";
        break;
    case 2:
        dist = "Moncloa";
        break;
    case 3:
        dist = "Retiro";
        break;
    }
    return dist;
}

int siguiente(const tListaPaquetes& listaPaquetes) {
    int mayor = 0;    
    for (int i = 1; i < listaPaquetes.cont; i++)
        if (listaPaquetes.paquetes[i].tamano > listaPaquetes.paquetes[mayor].tamano) {
            mayor = i;           
        }
    return mayor;
}

void eliminar(tListaPaquetes& listaPaquetes, int pos) {
    for (int i = pos + 1; i < listaPaquetes.cont - 1; i++)
        listaPaquetes.paquetes[i -1] = listaPaquetes.paquetes[i];
    listaPaquetes.cont--;
}

void agregar(tListaPaquetes& listaPaquetes, tPaquete paquete) {
     if (listaPaquetes.cont < MAX_PAQUETES) {   
        listaPaquetes.paquetes[listaPaquetes.cont] = paquete;
        listaPaquetes.cont++;
    }
}

int buscar(const tListaFurgos& listaFurgos, tPaquete paquete) {
    int pos = 0;
    bool encontrado = false;
    while (pos < listaFurgos.cont && !encontrado) {
        if (listaFurgos.furgonetas[pos].capacidad >= paquete.tamano && listaFurgos.furgonetas[pos].distrito == paquete.distrito)
            encontrado = true;
        else
            pos++;
    }
    if (!encontrado)
        pos = -1;
    return pos;
}

void asignar(tListaPaquetes& listaPaquetes, tListaPaquetes& listaNoAsignados, tListaFurgos& listaFurgos) {
    tPaquete paquete;
    int posP, posF;
    while (listaPaquetes.cont > 0){
        posP = siguiente(listaPaquetes);
        paquete = listaPaquetes.paquetes[posP];
        //se eliimina el paquete de la lista incial en cualquier caso, puesto que ya lo he guardado
        eliminar(listaPaquetes, posP);
        posF = buscar(listaFurgos, paquete);
        // si el paquete no se puede colocar en las furgos que ya están y no queda furgos disponibles
        if (posF == -1 && listaFurgos.cont <  MAX_FURGOS){
           // se da de alta una nueva furgoneta con el paquete
            listaFurgos.furgonetas[listaFurgos.cont].distrito = paquete.distrito;
            posF = listaFurgos.cont;
            listaFurgos.cont++;            
        }
        //si el paquete se puede meter en una furgo que ya está dada de alta, resta la capacidad con el tamaño del nuevo paquete y se
        //añade el paquete a la lista de paquetes no asignados
        if (posF == -1) {
            agregar(listaNoAsignados, paquete);
        }
        else{
            listaFurgos.furgonetas[posF].distrito = paquete.distrito;
            listaFurgos.furgonetas[posF].capacidad -= paquete.tamano;
            agregar(listaFurgos.furgonetas[posF].cargados, paquete);

        }     
        
    }
   
}