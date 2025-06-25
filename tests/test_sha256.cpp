/**
 * @file test_sha256.cpp
 * @brief Prueba unitaria para la librería SHA-256.
 *
 * Este archivo contiene una prueba unitaria que verifica la funcionalidad de la clase sha256
 * definida en F03_sha256.h. La prueba calcula y compara hashes SHA-256 para demostrar la
 * sensibilidad del algoritmo a cambios en el mensaje de entrada, utilizando la función
 * compararString de F04_comparar.h para la comparación.
 *
 * Dependencias:
 * - resources.h: Incluye librerías estándar de C++ (string, iostream, etc) para simplificar las inclusiones.
 * - F03_sha256.h: Contiene la implementación de la clase sha256.
 * - F04_comparar.h: Define la función compararString para comparar los hashes.
 *
 * @author badjavii
 * @date 06-23-2025
 */

#include "../resources.h"
#include "../src/F03_sha256.h"
#include "../src/F04_comparar.h"

/**
 * @brief Ejecuta una prueba unitaria para la clase sha256.
 *
 * Calcula el hash SHA-256 de los mensajes "Hola Mundo" y "Hola Mundo AAA" utilizando
 * el método sha_return de la clase sha256. Muestra ambos mensajes y sus hashes en
 * formato hexadecimal por consola, y compara los hashes con compararString para
 * determinar si son idénticos o si el mensaje ha sido manipulado.
 *
 * @return int Retorna 0 si la prueba se ejecuta correctamente.
 */

int main()
{
    string mensaje = "Hola Mundo";

    sha256 contexto;
    string hash = contexto.sha_return(mensaje);

    cout << "Mensaje:                 " << mensaje << endl;
    cout << "Hash SHA-256:            " << hash << endl;

    mensaje = "Hola Mundo AAA";

    string hash_manipulado = contexto.sha_return(mensaje);

    cout << "Mensaje manipulado:      " << mensaje << endl;
    cout << "Hash SHA-256 manipulado: " << hash_manipulado << endl;

    if (compararString(hash, hash_manipulado))
    {
        cout << "\nLos hashes son iguales." << endl;
    }
    else
    {
        cout << "\nLos hashes son diferentes. El mensaje ha sido manipulado" << endl;
    }

    return 0;
}