/**
 * @file F02_encriptacion.h
 * @brief Librería para encriptación y desencriptación de texto usando cifrado César y reflexión de dígitos.
 *
 * Este archivo contiene funciones para encriptar y desencriptar caracteres y líneas de texto.
 * La encriptación utiliza un cifrado César con un desplazamiento de 3 para letras y una reflexión
 * respecto a '9' para dígitos. La desencriptación invierte estas operaciones para recuperar el texto
 * original. Los caracteres no alfanuméricos se mantienen sin cambios en ambos procesos.
 *
 * Dependencias:
 * - resources.h: Incluye librerías estándar de C++ (string, iostream, etc) para simplificar las inclusiones.
 *
 * @author badjavii
 * @date 06-23-2025
 */

#ifndef F02_ENCRIPTACION_H
#define F02_ENCRIPTACION_H
#include "../resources.h" // Importa las librerías estándar de C++ necesarias para la implementación

/**
 * @brief Cifra un solo carácter usando un cifrado César con un desplazamiento de 3.
 *
 * Esta función toma un carácter y, si es una letra mayúscula o minúscula,
 * lo desplaza 3 posiciones hacia adelante en el alfabeto, retornando al orden original si es necesario.
 * Los caracteres no alfabéticos se devuelven sin cambios.
 *
 * @param p El carácter a cifrar.
 * @return El carácter cifrado.
 */

char encriptarLetra(char p)
{
    if (isupper(p))
        return char(65 + ((int(p) - 65 + 3) % 26));
    if (islower(p))
        return char(97 + ((int(p) - 97 + 3) % 26));
    return p;
}

/**
 * @brief Cifra un dígito reflejándolo con respecto a '9'.
 *
 * Si el carácter de entrada es un dígito ('0'-'9'), esta función devuelve el
 * carácter de dígito "reflejado" correspondiente (por ejemplo, '0' se convierte en '9', '1' se convierte en '8', etc.).
 * Si el carácter de entrada no es un dígito, se devuelve sin cambios.
 *
 * @param p El carácter a cifrar.
 * @return El carácter cifrado si p es un dígito; en caso contrario, devuelve p sin cambios.
 */

char encriptarDigito(char p)
{
    if (isdigit(p))
        return char(57 - (int(p) - 48)); // simétrico respecto a '9'
    return p;
}

/**
 * @brief Encripta un carácter determinando si es un dígito o una letra.
 *
 * Esta función comprueba si el carácter de entrada es un dígito o una letra y aplica
 * la función de encriptación adecuada (encriptarDigito para dígitos, encriptarLetra para letras).
 * Los caracteres no alfanuméricos se devuelven sin cambios.
 *
 * @param p El carácter a encriptar.
 * @return El carácter encriptado.
 */

char encriptarCaracter(char p)
{
    if (isdigit(p))
        return encriptarDigito(p);
    else
        return encriptarLetra(p);
}

/**
 * @brief Encripta una línea de texto carácter por carácter.
 *
 * Esta función toma una cadena de texto y aplica la función encriptarCaracter
 * a cada uno de sus caracteres, construyendo y devolviendo la línea encriptada.
 *
 * @param linea La línea de texto original que se desea encriptar.
 * @return string La línea encriptada resultante.
 */

string encriptarLinea(string linea)
{
    string linea_encriptada = "";
    for (char letra : linea)
    {
        linea_encriptada += encriptarCaracter(letra);
    }
    return linea_encriptada;
}

/************************* DESENCRIPTACION ********************************/

/**
 * @brief Descifra un solo carácter encriptado con un cifrado César de desplazamiento 3.
 *
 * Esta función toma un carácter encriptado y, si es una letra mayúscula o minúscula,
 * lo desplaza 3 posiciones hacia atrás en el alfabeto, retornando al orden original si es necesario.
 * Los caracteres no alfabéticos se devuelven sin cambios.
 *
 * @param p El carácter a descifrar.
 * @return El carácter descifrado.
 */

char desencriptarLetra(char p)
{
    if (isupper(p))
        return char(65 + ((int(p) - 65 - 3 + 26) % 26));
    if (islower(p))
        return char(97 + ((int(p) - 97 - 3 + 26) % 26));
    return p;
}

/**
 * @brief Descifra un dígito reflejado con respecto a '9'.
 *
 * Si el carácter de entrada es un dígito ('0'-'9'), esta función devuelve el
 * carácter de dígito "reflejado" correspondiente (por ejemplo, '9' se convierte en '0', '8' se convierte en '1', etc.).
 * Si el carácter de entrada no es un dígito, se devuelve sin cambios.
 *
 * @param p El carácter a descifrar.
 * @return El carácter descifrado si p es un dígito; en caso contrario, devuelve p sin cambios.
 */

char desencriptarDigito(char p)
{
    if (isdigit(p))
        return char(57 - (int(p) - 48)); // simétrico respecto a '9'
    return p;
}

/**
 * @brief Descifra un carácter determinando si es un dígito o una letra.
 *
 * Esta función comprueba si el carácter de entrada es un dígito o una letra y aplica
 * la función de desencriptación adecuada (desencriptarDigito para dígitos, desencriptarLetra para letras).
 * Los caracteres no alfanuméricos se devuelven sin cambios.
 *
 * @param p土地 El carácter a descifrar.
 * @return El carácter descifrado.
 */

char desencriptarCaracter(char p)
{
    if (isdigit(p))
        return desencriptarDigito(p);
    else
        return desencriptarLetra(p);
}

/**
 * @brief Descifra una línea de texto encriptada carácter por carácter.
 *
 * Esta función toma una cadena de texto encriptada y aplica la función desencriptarCaracter
 * a cada uno de sus caracteres, construyendo y devolviendo la línea descifrada.
 *
 * @param linea La línea de texto encriptada que se desea descifrar.
 * @return string La línea descifrada resultante.
 */

string desencriptarLinea(string linea)
{
    string linea_desencriptada = "";
    for (char letra : linea)
    {
        linea_desencriptada += desencriptarCaracter(letra);
    }
    return linea_desencriptada;
}

#endif // F02_ENCRIPTACION_H