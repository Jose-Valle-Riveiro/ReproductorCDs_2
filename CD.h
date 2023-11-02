#pragma once
#include "pch.h"
#include <iostream>
#include <string>
#include <list>
#include <limits>
#include <algorithm>
#include <queue>
#include <fstream>
#include "Cancion.h"


using namespace System;
//STRUCT CD CONTIENE LOS ELEMENTOS DE CADA CD INDIVIDUAL//
struct CD_ELEMENTS {

	std::string Nombre;

	int NumeroCanciones;

	std::list<Cancion> Lista;


};
ref class CD
{
private:
	Cancion* Reproduciendo = nullptr;
public:

	void LeerCarpeta(std::string direccion, std::list<CD_ELEMENTS>& Lista, bool& LecturaValida);
	bool VerificarFormato(std::string elemento);
	void ListaCDs(std::list<CD_ELEMENTS>& Lista, std::queue<Cancion>& Cola);
	void CheckCola(std::queue<Cancion> Cola);
	void CheckColaDef(std::queue<Cancion>& Cola);
	void SortByArtista(std::queue<Cancion>& Cola, int formato);
	void SortByNombre(std::queue<Cancion>& Cola, int formato);
	void SortByDuracion(std::queue<Cancion>& Cola, int formato);
	void PonerCancionActual(Cancion* Rola);
	void ReproduccionActual(Cancion* Rola);
	Cancion* ReturnCancionActual();
	void ReproduccionSiguiente(std::queue<Cancion>& Cola);
	void Imprimir(std::queue<Cancion> Cola);
	void Reinicio(std::queue<Cancion>& Cola, std::list<CD_ELEMENTS>& Albums);

};

