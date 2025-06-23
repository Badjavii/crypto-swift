#include <iostream>
#include <string>
#include "../src/encriptacion.h"
using namespace std;

/**
 * @file encriptacion_test.cpp
 * @brief Archivo de prueba unitaria para la librería de encriptación.
 *
 * Este archivo contiene una función principal que prueba la funcionalidad de las funciones
 * `encriptarLinea` y `desencriptarLinea` definidas en la librería `encriptacion.h`.
 *
 * El programa toma un mensaje de ejemplo ("Hola Mundo 1234"), lo encripta utilizando la función
 * `encriptarLinea`, luego lo desencripta con `desencriptarLinea` y muestra por consola el mensaje
 * original, el mensaje encriptado y el mensaje desencriptado para verificar la correcta operación
 * de ambas funciones.
 *
 * @author badjavii
 * @date 06-23-2025
 */

int main()
{
    const char *mensaje = "Hola Mundo 1234";

    string mensaje_encriptado = encriptarLinea(mensaje);

    string mensaje_desencriptado = desencriptarLinea(mensaje_encriptado);

    cout << "Mensaje original:      " << mensaje << endl;
    cout << "Mensaje encriptado:    " << mensaje_encriptado << endl;
    cout << "Mensaje desencriptado: " << mensaje_desencriptado << endl;

    return 0;
}
