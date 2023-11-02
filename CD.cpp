#include "pch.h"
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <vector>
#include "Cancion.h"
#include "CD.h"
#include "dirent.h"

using namespace System;

//METODO PARA ORDENAR EN ORDEN ALFABETICO EN LOS SORTS
bool CompararArtista(const Cancion& primero, const Cancion& segundo)
{
	return primero.NombreArtista < segundo.NombreArtista;

}

bool CompararNombre(const Cancion& primero, const Cancion& segundo)
{
	return primero.NombreCancion < segundo.NombreCancion;

}

bool CompararDuracion(const Cancion& primero, const Cancion& segundo)
{
	return primero.Duracion < segundo.Duracion;

}

void CD::LeerCarpeta(std::string direccion, std::list<CD_ELEMENTS>& Lista, bool& LecturaValida)
{

	DIR* directorio;
	struct dirent* elemento;
	std::string item, name, previo;
	CD_ELEMENTS CDS;
	int Cantidad = 0;
	Cancion Rola;
	int Contador = 0;

	//ABRE EL DIRECTORIO
	if (directorio = opendir(direccion.c_str())) {

		while (elemento = readdir(directorio)) {

			//IGNORAR LOS PUNTOS QUE SALEN CUANDO SE ABRE UN DIRECTORIO
			if (std::string(elemento->d_name) != "." && std::string(elemento->d_name) != "..") {

				//DIVIDE EL NOMBRE PARA IGNORAR EL .txt
				size_t pos = std::string(elemento->d_name).find_last_of(".");

				CDS.Nombre = std::string(elemento->d_name).substr(0, pos);
				CDS.NumeroCanciones = 0;
				CDS.Lista.clear();

				//LEER CADA .txt
				std::string Acceso = direccion + "/" + elemento->d_name;

				std::fstream archivo(Acceso);

				if (archivo.peek() != std::char_traits<char>::eof())
				{
					//SE LEE CADA ARCHIVO Y SE GUARDA EN EL ITEM
					while (getline(archivo, item))
					{
						if (VerificarFormato(item) && previo != item)
						{
							previo = item;
							int separador = 0;
							std::string phrase = "";

							//DIVIDE CADA PARTE DEL STRING POR LOS "||" PARA ALMACENAR CADA DATO DE LA CANCION
							for (int i = 0; i < item.length(); i++)
							{

								if (item[i] != '|')
								{
									phrase += item[i];

								}
								else
								{

									if (separador == 0)
									{
										Rola.NombreCancion = phrase;


									}
									else if (separador == 2)
									{
										Rola.NombreArtista = phrase;


									}

									phrase = " ";
									separador++;
								}

								if (i == item.length() - 1)
								{
									Rola.Duracion = phrase;

								}
							}

							//SUBE LA CANCION A LA LISTA DEL CD, DEFINE EL ALBUM, AUMENTA LA CANTIDAD DE CANCIONES DEL CD
							Rola.Album = CDS.Nombre;

							CDS.Lista.push_back(Rola);

							Contador++;

						}//MANEJO DE ERRORES
						else if (previo == item)
						{

							std::cerr << "\x1b[31mERROR\x1b[0m, Archivo: " << "\x1b[31m" << CDS.Nombre << "\x1b[0m" << "; Tipo de Error: \x1b[31mLinea repetida\x1b[0m" << "; Numero de Linea: " << "\x1b[31m" << (Contador + 1) << "\x1b[0m";
							LecturaValida = false;
							return;

						}
						else
						{
							std::cerr << "\x1b[31mERROR\x1b[0m, Archivo: " << "\x1b[31m" << CDS.Nombre << "\x1b[0m" << "; Tipo de Error: \x1b[31mFormato de linea incorrecto\x1b[0m" << "; Numero de Linea: " << "\x1b[31m" << (Contador + 1) << "\x1b[0m";
							LecturaValida = false;
							return;
						}

						

						
					}

				}
				else
				{

					std::cerr << "ERROR, Archivo: " << "\x1b[31m" << CDS.Nombre << "\x1b[0m" << "; Tipo de Error: \x1b[31mArchivo vacio\x1b[0m.";
					LecturaValida = false;
					return;

				}

				CDS.NumeroCanciones = Contador;
				Contador = 0;

				//METER EL CD ENTERO A LA LISTA DE CDS
				Lista.push_back(CDS);


			}
		}
	}
	else
	{
		std::cerr << "ERROR,\x1b[31mNo se pudo abrir el directorio\x1b[0m";
		LecturaValida = false;
		return;
	}

	//CERRAR EL DIRECTORIO //LA LECTURA HA SIDO VALIDA
	closedir(directorio);
	LecturaValida = true;
	return;




};

bool CD::VerificarFormato(std::string elemento) {

	int Separadores = 0;
	//TIENE QUE HABER 4 SEPARADORES Y PALABRAS O NUMEROS EN EL ULTIMO CASO PARA QUE SEA VERDADERO, EN CUALQUIER OTRO CASO RETORNA FALSO
	for (size_t i = 0; i < elemento.length(); i++)
	{
		if (elemento[i] == '|')
		{
			Separadores++;

			//SEPARADORES INPARES
			if (Separadores % 2 != 0)
			{
				if (elemento[i + 2] == '|')
				{
					return false;

				}
				else if (elemento[i - 1] == ' ')
				{
					return false;
				}

			}
		}
	}
	//QUE HAYA NUMEROS LA FINAL
	if (Separadores == 4)
	{
		size_t pos = elemento.find_last_of('|');
		int ContadorSignoPt = 0;

		std::string Restante = elemento.substr(pos + 1, elemento.length());

		for (char c : Restante)
		{
			if (!isdigit(c))
			{
				if (c == ':')
				{
					ContadorSignoPt++;
					if (ContadorSignoPt > 1)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}

		}

	}
	else
	{
		return false;
	}

	return true;

}

void CD::ListaCDs(std::list<CD_ELEMENTS>& Lista, std::queue<Cancion>& Cola) {
	//EL PROPOSITO DE ESTE MENU ES MANDAR CANCIONES DE NUESTRA COLECCION CREADA EN "LeerCarpeta" A LA COLA DE REPRODUCCION

	Console::Clear();

	//VECTOR DE LISTAS
	std::vector<CD_ELEMENTS> ColecccionCDs;
	//IGUALA EL VECTOR A LA LISTA DE CDS PROPORCIONADA
	ColecccionCDs.assign(Lista.begin(), Lista.end());

	while (true) {

		std::cout << "BIENVENIDO A SU COLECCION DE CDs: " << std::endl
			<< "INGRESE EL NUMERO DE CD PARA ACCEDER" << std::endl;

		for (int i = 0; i < ColecccionCDs.size(); i++)
		{
			std::cout << (i + 1) << ". " << ColecccionCDs[i].Nombre << std::endl;
		}

		std::cin.ignore(999, '\n');
		std::cin.clear();
		int opcion;
		std::cin >> opcion;


		//MANEJO DE ERRORES
		if (std::cin.fail() || opcion < 1 || opcion > ColecccionCDs.size())
		{
			std::cin.clear();
			Console::Clear();
			std::cerr << "\x1b[31mOpcion invalida. Por favor, ingrese un numero entre 1 y 13.\x1b[0m" << std::endl;
			continue;
		}
		else
		{
			Console::Clear();

			//MOSTRAR LAS CANCIONES DE LA LISTA DEL CD (USANDO VECTORES)
			std::vector<Cancion> CD_Tracks;

			opcion = opcion - 1;
			//ASIGNAR LAS CANCIONES DEL CD AL VECTOR
			CD_Tracks.assign(ColecccionCDs[opcion].Lista.begin(), ColecccionCDs[opcion].Lista.end());

			while (true)
			{
				std::cout << "Del \033[34mCD\033[0m:" << CD_Tracks[opcion].NombreCancion << std::endl;
				std::cout << "INGRESE EL NUMERO DE CANCION" << std::endl;

				

				//PRINT
				for (int i = 0; i < CD_Tracks.size(); i++)
				{
					std::cout << (i + 1) << ". " << CD_Tracks[i].NombreCancion << std::endl;
				}

				std::cin.ignore(999, '\n');
				std::cin.clear();

				int Track;
				std::cin >> Track;

				//CON LA CANCION SELECCIONADA SE MANDA A LA QUEUE
				if (std::cin.fail() || Track < 1 || Track > CD_Tracks.size())
				{
					std::cin.clear();
					Console::Clear();
					std::cerr << "\x1b[31mOpcion	invalida. Por favor, ingrese un numero dentro del rango de canciones.\x1b[0m" << std::endl;
					std::cout << std::endl;
				}
				else
				{
					//TRACK SELECCIONADA EXITOSAMENTE
					Track--;
					Cola.push(CD_Tracks[Track]);
					break;
				}
			}

			break;
		}



	}

}

void CD::CheckCola(std::queue<Cancion> Cola) {

	//ESTO SIMPLEMENTE ES EL MENU PARA INTERACTUAR CON LA COLA

	Console::Clear();

	if (Cola.empty()) {

		std::cout << "LA COLA SE ENCUENTRA VACIA: " << std::endl;

	}
	while (true) {

		std::cin.ignore(9999, '\n');

		std::cout << "INGRESE EL NUMERO DE OPCION: " << std::endl
			<< "1. ORDENAR POR ARTISTA: " << std::endl
			<< "2. ORDENAR POR NOMBRE: " << std::endl
			<< "3. ORDENAR POR DURACION: " << std::endl
			<< "4. SIN ORDENAR" << std::endl;

		int opcion;
		std::cin >> opcion;

		//MANEJO DE ERRORES
		if (std::cin.fail() || opcion < 1 || opcion > 4)
		{
			std::cin.clear();
			Console::Clear();
			std::cerr << "\x1b[31mOpcion	invalida. Por favor, ingrese un numero entre 1 y 4\x1b[0m" << std::endl;
			std::cout << "\n";
			continue;

		}
		else {
			int N;
			switch (opcion) {


			case 1:



				while (true) {
					Console::Clear();
					std::cout << "DESEA ORDENAR DE FORMA ASCENDENTE O DESCENDENTE? " << std::endl
						<< "1. ASCENDENTE" << std::endl
						<< "2. DESCENDENTE" << std::endl;

					std::cin >> N;

					//MANEJO DE ERRORES
					if (std::cin.fail() || N < 1 || N > 2)
					{
						std::cin.clear();
						Console::Clear();
						std::cerr << "\x1b[31mOpcion invalida. Por favor, ingrese un numero entre 1 y 2\x1b[0m" << std::endl;
						std::cout << "\n";
						continue;

					}
					else {
						SortByArtista(Cola, N);
						Console::ReadKey();
						break;
					}
					
				}
				break;

			case 2:

				while (true) {
					Console::Clear();
					std::cout << "DESEA ORDENAR DE FORMA ASCENDENTE O DESCENDENTE? " << std::endl
						<< "1. ASCENDENTE" << std::endl
						<< "2. DESCENDENTE" << std::endl;

					std::cin >> N;

					//MANEJO DE ERRORES
					if (std::cin.fail() || N < 1 || N > 2)
					{
						std::cin.clear();
						Console::Clear();
						std::cerr << "\x1b[31mOpcion invalida. Por favor, ingrese un numero entre 1 y 2\x1b[0m" << std::endl;
						std::cout << "\n";
						continue;

					}
					else {
						SortByNombre(Cola, N);
						Console::ReadKey();
						break;
					}
					
				}
				break;

			case 3:

				while (true) {
					Console::Clear();
					std::cout << "DESEA ORDENAR DE FORMA ASCENDENTE O DESCENDENTE? " << std::endl
						<< "1. ASCENDENTE" << std::endl
						<< "2. DESCENDENTE" << std::endl;

					std::cin >> N;

					//MANEJO DE ERRORES
					if (std::cin.fail() || N < 1 || N > 2)
					{
						std::cin.clear();
						Console::Clear();
						std::cerr << "\x1b[31mOpcion invalida. Por favor, ingrese un numero entre 1 y 2\x1b[0m" << std::endl;
						std::cout << "\n";
						continue;

					}
					else {
						SortByDuracion(Cola, N);
						Console::ReadKey();
						break;
					}
					
				}
				break;


			case 4:
				Console::Clear();
				Imprimir(Cola);
				break;


			}
		}
		break;
	}
	return;
}

void CD::CheckColaDef(std::queue<Cancion>& Cola) {

	//ESTO SIMPLEMENTE ES EL MENU PARA INTERACTUAR CON LA COLA

	Console::Clear();

	if (Cola.empty()) {

		std::cout << "LA COLA SE ENCUENTRA VACIA: " << std::endl;

	}
	while (true) {

		std::cin.ignore(9999, '\n');

		std::cout << "INGRESE EL NUMERO DE OPCION: " << std::endl
			<< "1. ORDENAR POR ARTISTA: " << std::endl
			<< "2. ORDENAR POR NOMBRE: " << std::endl
			<< "3. ORDENAR POR DURACION: " << std::endl
			<< "4. SIN ORDENAR" << std::endl;

		int opcion;
		std::cin >> opcion;

		//MANEJO DE ERRORES
		if (std::cin.fail() || opcion < 1 || opcion > 4)
		{
			std::cin.clear();
			Console::Clear();
			std::cerr << "\x1b[31mOpcion	invalida. Por favor, ingrese un numero entre 1 y 4\x1b[0m" << std::endl;
			std::cout << "\n";
			continue;

		}
		else {
			int N;
			switch (opcion) {


			case 1:



				while (true) {
					Console::Clear();
					std::cout << "DESEA ORDENAR DE FORMA ASCENDENTE O DESCENDENTE? " << std::endl
						<< "1. ASCENDENTE" << std::endl
						<< "2. DESCENDENTE" << std::endl;

					std::cin >> N;

					//MANEJO DE ERRORES
					if (std::cin.fail() || N < 1 || N > 2)
					{
						std::cin.clear();
						Console::Clear();
						std::cerr << "\x1b[31mOpcion invalida. Por favor, ingrese un numero entre 1 y 2\x1b[0m" << std::endl;
						std::cout << "\n";
						continue;

					}
					else {
						SortByArtista(Cola, N);
						Console::ReadKey();
						break;
					}

				}
				break;

			case 2:

				while (true) {
					Console::Clear();
					std::cout << "DESEA ORDENAR DE FORMA ASCENDENTE O DESCENDENTE? " << std::endl
						<< "1. ASCENDENTE" << std::endl
						<< "2. DESCENDENTE" << std::endl;

					std::cin >> N;

					//MANEJO DE ERRORES
					if (std::cin.fail() || N < 1 || N > 2)
					{
						std::cin.clear();
						Console::Clear();
						std::cerr << "\x1b[31mOpcion invalida. Por favor, ingrese un numero entre 1 y 2\x1b[0m" << std::endl;
						std::cout << "\n";
						continue;

					}
					else {
						SortByNombre(Cola, N);
						Console::ReadKey();
						break;
					}

				}
				break;

			case 3:

				while (true) {
					Console::Clear();
					std::cout << "DESEA ORDENAR DE FORMA ASCENDENTE O DESCENDENTE? " << std::endl
						<< "1. ASCENDENTE" << std::endl
						<< "2. DESCENDENTE" << std::endl;

					std::cin >> N;

					//MANEJO DE ERRORES
					if (std::cin.fail() || N < 1 || N > 2)
					{
						std::cin.clear();
						Console::Clear();
						std::cerr << "\x1b[31mOpcion invalida. Por favor, ingrese un numero entre 1 y 2\x1b[0m" << std::endl;
						std::cout << "\n";
						continue;

					}
					else {
						SortByDuracion(Cola, N);
						Console::ReadKey();
						break;
					}

				}
				break;


			case 4:
				Console::Clear();
				Imprimir(Cola);
				break;


			}
		}
		break;
	}
	return;
}

void CD::SortByArtista(std::queue<Cancion>& Cola, int Opcion) {


	//LLENAR VECTOR
	std::vector<Cancion> ColaVector;

	while (!Cola.empty())
	{
		ColaVector.push_back(Cola.front());
		Cola.pop();
	}

	//ORDENAR VECTOR
	sort(ColaVector.begin(), ColaVector.end(), &CompararArtista);

	//OPCION = 1.ORDEN ASCENDENTE  ELSE. ORDEN DESCENDENTE
	if (Opcion == 1)
	{
		for (const auto& Cancion : ColaVector)
		{
			Cola.push(Cancion); //EL CHISTE ES AGREGAR LAS CANCIONES A LA COLA DE FORMA ORDENADA

		}
	}
	else
	{	//REVERSE INVIERTE EL ORDENAMIENTO DEL VECTOR (QUE YA ESTA ORDENADO)
		reverse(ColaVector.begin(), ColaVector.end());

		for (const auto& Cancion : ColaVector)
		{
			Cola.push(Cancion);
		}
	}

	ColaVector.clear();

	
	Imprimir(Cola);

	return;



}

void CD::SortByNombre(std::queue<Cancion>& Cola, int Opcion) {


	//LLENAR VECTOR
	std::vector<Cancion> ColaVector;

	while (!Cola.empty())
	{
		ColaVector.push_back(Cola.front());
		Cola.pop();
	}

	//ORDENAR VECTOR
	sort(ColaVector.begin(), ColaVector.end(), &CompararNombre);

	//OPCION = 1.ORDEN ASCENDENTE  ELSE. ORDEN DESCENDENTE
	if (Opcion == 1)
	{
		for (const auto& Cancion : ColaVector)
		{
			Cola.push(Cancion); //EL CHISTE ES AGREGAR LAS CANCIONES A LA COLA DE FORMA ORDENADA

		}
	}
	else
	{	//REVERSE INVIERTE EL ORDENAMIENTO DEL VECTOR (QUE YA ESTA ORDENADO)
		reverse(ColaVector.begin(), ColaVector.end());

		for (const auto& Cancion : ColaVector)
		{
			Cola.push(Cancion);
		}
	}

	ColaVector.clear();

	//PRINT
	Imprimir(Cola);

	return;



}

void CD::SortByDuracion(std::queue<Cancion>& Cola, int Opcion) {


	//LLENAR VECTOR
	std::vector<Cancion> ColaVector;

	while (!Cola.empty())
	{
		ColaVector.push_back(Cola.front());
		Cola.pop();
	}

	//ORDENAR VECTOR
	sort(ColaVector.begin(), ColaVector.end(), &CompararDuracion);

	//OPCION = 1.ORDEN ASCENDENTE  ELSE. ORDEN DESCENDENTE
	if (Opcion == 1)
	{
		for (const auto& Cancion : ColaVector)
		{
			Cola.push(Cancion); //EL CHISTE ES AGREGAR LAS CANCIONES A LA COLA DE FORMA ORDENADA

		}
	}
	else
	{	//REVERSE INVIERTE EL ORDENAMIENTO DEL VECTOR (QUE YA ESTA ORDENADO)
		reverse(ColaVector.begin(), ColaVector.end());

		for (const auto& Cancion : ColaVector)
		{
			Cola.push(Cancion);
		}
	}

	ColaVector.clear();

	//PRINT
	Imprimir(Cola);

	return;



}

void CD::PonerCancionActual(Cancion* Rola) {

	Reproduciendo = Rola;
}

Cancion* CD::ReturnCancionActual() {

	return Reproduciendo;

}

void CD::ReproduccionActual(Cancion* Rola) {

	//IMPRIMIR LA CANCION SIENDO REPRODUCIDA NADA MAS
	Console::Clear();

	if (Reproduciendo == nullptr)
	{
		std::cout << std::endl;
		std::cout << "\t" << "REPRODUCCION EN PAUSA";

	}
	else
	{
		std::cout << std::endl;

		std::cout << "\t" << "\t" << "\t" << "AHORA REPRODUCIENDO" << std::endl;

		std::cout << "\t" << "CANCION: " << "\t" << Reproduciendo->NombreCancion << std::endl;

		std::cout << "\t" << "BY:" << "\t" << Reproduciendo->NombreArtista << std::endl;

		std::cout << "\t" << "DURACION:" << "\t" << Reproduciendo->Duracion << std::endl;

		std::cout << "\t" << "ALBUM (CD): " << "\t" << Reproduciendo->Album << std::endl;

	}

	return;

}

void CD::ReproduccionSiguiente(std::queue<Cancion>& Cola) {

	Console::Clear();

	if (Cola.empty())
	{
		std::cout << "COLA DE REPRODUCCION VACIA";

	}
	else
	{
		//SI NO HAY REPRODUCIENDO QUE SE PONGA LA PRIMER CANCION DE LA COLA (FIFO)
		if (ReturnCancionActual() == nullptr)
		{
			PonerCancionActual(&Cola.front());

		}
		else
		{
			//ALGORITMO DE ELEMINAR DE LA COLA, MANDAR LA PRIMERA PARA EL FINAL Y LA ANTERIOR COMO PRIMERA
			Cancion Temp = Cola.front();

			Cola.pop();

			Cola.push(Temp);

			PonerCancionActual(&Cola.front());


		}

		//IMPRIMIR REPRODUCIENDO
		std::cout << "----	REPRODUCIENDO: " << ReturnCancionActual()->NombreCancion << std::endl
			<< "----	BY: " << ReturnCancionActual()->NombreArtista;
	}

	return;

}

void CD::Imprimir(std::queue<Cancion> Cola) {
	//IMPRIMIR TODA LA COLA 
	Console::Clear();

	std::cout << "\033[34mNombre\033[0m                                   \033[34mArtista\033[0m                                 \033[34mDuracion\033[0m" << std::endl;

	while (!Cola.empty())
	{
		std::cout << std::left << std::setw(40) << Cola.front().NombreCancion << std::setw(40) << Cola.front().NombreArtista << std::setw(40) << Cola.front().Duracion << std::endl;
		Cola.pop();
	}

	return;
}

void CD::Reinicio(std::queue<Cancion>& Cola, std::list<CD_ELEMENTS>& Albums) {

	//REINICIAR TODO, REPRODUCIENDO, COLA DE REPRODUCCION Y LISTA DE CDS
	PonerCancionActual(nullptr);

	while (!Cola.empty())
	{
		Cola.pop();
	}

	Albums.clear();

}

