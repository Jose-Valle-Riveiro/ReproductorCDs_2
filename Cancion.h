#pragma once
#include "pch.h"
#include <string>
#include <list>
#include <limits>
#include <algorithm>
#include <fstream>
#include <queue>

using namespace System;
//STRUCT CANCION CONTIENE LOS ELEMENTOS DE CADA CANCION INDIVIDUAL
//RECORDEMOS QUE EL CD ES UNA LISTA SIMPLEMENTE ENLAZADA DE CANCIONES

struct Cancion {



	std::string NombreCancion;
	std::string NombreArtista;
	std::string Album;
	std::string Duracion;


};