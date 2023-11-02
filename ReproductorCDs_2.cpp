#include "pch.h"
#include <string>
#include <list>
#include <limits>
#include <algorithm>
#include <fstream>
#include <queue>
#include <iostream>
#include "Cancion.h"
#include "CD.h"


using namespace System;

std::list<CD_ELEMENTS> Albumes;
std::queue<Cancion> Cola;

int main(array<System::String^>^ args)
{


    std::string Direccion;
    CD Reproductor;
    int Opcion, Opcion1;
    bool Main = true;
    bool Exito = false;

    while (Main)
    {
        bool Menu = true;

        do
        {

            std::cout << "__________________________________________" << std::endl;
            std::cout << "||        REPRODUCTOR DE MUSICA           " << std::endl;
            std::cout << "__________________________________________" << std::endl;
            std::cout << std::endl;

            std::cout << "INGRESE UNA OPCION" << std::endl;
            std::cout << "1. CARGAR ARCHIVOS" << std::endl;
            std::cout << "2. CERRAR" << std::endl;
            std::cin >> Opcion;

            if (std::cin.fail() || (Opcion < 1) || (Opcion > 2))
            {
                std::cerr << "Valor invalido. Ingrese un número entre 1 y 2." << std::endl;
                Console::ReadKey();
                Console::Clear();
            }
            else
            {
                break;
            }

        } while (true);

        std::cin.ignore();

        switch (Opcion)
        {
        case 1:

            Console::Clear();
            Reproductor.Reinicio(Cola, Albumes);
            std::cout << "Ingrese la ruta de la carpeta donde se ubican sus " << "\x1b[31mCD's\x1b[0m\n";
            std::getline(std::cin, Direccion);
            std::cout << std::endl;


            Reproductor.LeerCarpeta(Direccion, Albumes, Exito);
            std::cout << "\n";


            if (Exito)
            {
                while (Menu)
                {
                    Console::Clear();

                    std::cout << "---------------------// REPRODUCTOR //---------------------" << std::endl;
                    std::cout << "1. Agregar Cancion" << std::endl;
                    std::cout << "2. Ver Cola de Reproduccion" << std::endl;
                    std::cout << "3. Reproduccion Actual" << std::endl;
                    std::cout << "4. Reproduccion Siguiente" << std::endl;
                    std::cout << "5. Ordenar" << std::endl;
                    std::cout << "6. Salir" << std::endl;
                    std::cin >> Opcion1;

                    if (std::cin.fail() || Opcion1 < 1 || Opcion1 > 6)
                    {
                        std::cerr << "Valor invalido. Ingrese un número entre 1 y 6." << std::endl;
                        Console::ReadKey();
                        std::cin.clear();
                        std::cin.ignore(9999, '\n');
                        continue;
                    }

                    switch (Opcion1)
                    {
                    case 1:

                        Reproductor.ListaCDs(Albumes, Cola);
                        Console::Clear();

                        break;

                    case 2:

                        Reproductor.CheckCola(Cola);

                        std::cout << "\n";
                        Console::ReadKey();

                        break;

                    case 3:

                        Reproductor.ReproduccionActual(Reproductor.ReturnCancionActual());

                        Console::ReadKey();

                        break;

                    case 4:

                        Reproductor.ReproduccionSiguiente(Cola);

                        Console::ReadKey();

                        break;

                    case 5:

                        Console::Clear();

                        if (!Cola.empty())
                        {
                            Reproductor.CheckColaDef(Cola);
                            Reproductor.PonerCancionActual(nullptr);

                        }
                        else
                        {
                            std::cout << "COLA DE REPRODUCCION VACIA";
                            Console::ReadKey();
                        }


                        break;

                    case 6:

                        Menu = false;
                        break;
                    }
                    Console::Clear();


                }
            }
            else
            {
                Console::ReadKey();
                Console::Clear();
            }

            break;

        case 2:

            Console::Clear();
            Main = false;

            break;

        }


    }


    return 0;
}

