/**
 * @file F04_comparar.h
 * @brief Librería para comparar cadenas de texto.
 *
 * Este archivo proporciona una función para comparar dos cadenas de texto y determinar
 * si son idénticas. La función es utilizada en pruebas unitarias para verificar la
 * igualdad de contenido, como hashes o mensajes encriptados.
 *
 * Dependencias:
 * - resources.h: Incluye librerías estándar de C++ (string, iostream, etc) para simplificar las inclusiones.
 *
 * @author badjavii
 * @date 06-23-2025
 */

#ifndef F04_COMPARAR_H
#define F04_COMPARAR_H
#include "../resources.h" // Importa las librerías estándar de C++ necesarias para la implementación

/**
 * @brief Compara dos cadenas de texto para determinar si son idénticas.
 *
 * Esta función compara las cadenas a y b utilizando el operador de igualdad de C++.
 * Devuelve true si las cadenas son idénticas (mismo contenido y longitud), y false
 * en caso contrario.
 *
 * @param a Primera cadena de texto a comparar.
 * @param b Segunda cadena de texto a comparar.
 * @return bool true si las cadenas son idénticas, false si son diferentes.
 */

bool compararString(string &a, string &b)
{
    return (a == b); // Compara dos cadenas de texto
}

#endif // F04_COMPARAR_H