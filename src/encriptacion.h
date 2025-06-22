#ifndef ENCRIPTACION_H
#define ENCRIPTACION_H
#include <fstream>
#include <string>
using namespace std;

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
 * @return std::string La línea encriptada resultante.
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

#endif // ENCRIPTACION_H
