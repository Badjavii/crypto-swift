#include <iostream>
#include <string>
#include "../src/sha256.h"
using namespace std;

/**
 * @file sha256.cpp
 * @brief Archivo de prueba unitaria para la librería SHA-256.
 *
 * Este archivo contiene una función principal que prueba la funcionalidad de la clase `sha256`
 * para calcular el hash SHA-256 de un mensaje de texto.
 *
 * El programa crea una instancia del contexto SHA-256, calcula el hash de un mensaje de ejemplo
 * ("Hola Mundo") y muestra tanto el mensaje original como el hash resultante por consola.
 *
 * @author badjavii
 * @date 06-23-2025
 */

int main()
{
    const char *mensaje = "Hola Mundo";

    sha256 contexto;
    string hash = contexto.sha_return(mensaje);

    cout << "Mensaje: " << mensaje << endl;
    cout << "Hash SHA-256: " << hash << endl;

    return 0;
}
