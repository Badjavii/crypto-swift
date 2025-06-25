/**
 * @file test_encriptacion.cpp
 * @brief Prueba unitaria para las funciones de encriptación y desencriptación.
 *
 * Este archivo contiene una prueba unitaria que verifica la funcionalidad de las funciones
 * encriptarLinea y desencriptarLinea definidas en F02_encriptacion.h. La prueba
 * asegura que el proceso de encriptación sea reversible mediante la desencriptación,
 * mostrando los resultados por consola para su verificación.
 *
 * Dependencias:
 * - resources.h: Incluye librerías estándar de C++ (string, iostream, etc) para simplificar las inclusiones.
 * - F02_encriptacion.h: Contiene las funciones de encriptación y desencriptación.
 *
 * @author badjavii
 * @date 06-23-2025
 */

#include "../resources.h"
#include "../src/F02_encriptacion.h"

/**
 * @brief Ejecuta una prueba unitaria para las funciones de encriptación y desencriptación.
 *
 * Encripta el mensaje "PRUEBA DE ARCHIVO CON CANCION DE BISFP 8038" usando encriptarLinea,
 * luego lo desencripta con desencriptarLinea. Muestra por consola el mensaje original,
 * el mensaje encriptado y el mensaje desencriptado para verificar que la desencriptación
 * recupera el texto original.
 *
 * @return int Retorna 0 si la prueba se ejecuta correctamente.
 */

int main()
{
    const char *mensaje = "PRUEBA DE ARCHIVO CON CANCION DE BISFP 8038";

    // Encriptar y desencriptar el mensaje
    string mensaje_encriptado = encriptarLinea(mensaje);

    // Desencriptar el mensaje encriptado
    string mensaje_desencriptado = desencriptarLinea(mensaje_encriptado);

    cout << "Mensaje original:      " << mensaje << endl;
    cout << "Mensaje encriptado:    " << mensaje_encriptado << endl;
    cout << "Mensaje desencriptado: " << mensaje_desencriptado << endl;

    return 0;
}
