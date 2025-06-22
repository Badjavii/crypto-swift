#ifndef ENCRIPTACION_H
#define ENCRIPTACION_H
#include <fstream>
#include <string>
using namespace std;

/**
* @brief Cifra un solo car�cter usando un cifrado C�sar con un desplazamiento de 3.
*
* Esta funci�n toma un car�cter y, si es una letra may�scula o min�scula,
* lo desplaza 3 posiciones hacia adelante en el alfabeto, retornando al orden original si es necesario.
* Los caracteres no alfab�ticos se devuelven sin cambios.
*
* @param p El car�cter a cifrar.
* @return El car�cter cifrado.
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
* @brief Cifra un d�gito reflej�ndolo con respecto a '9'.
*
* Si el car�cter de entrada es un d�gito ('0'-'9'), esta funci�n devuelve el
* car�cter de d�gito "reflejado" correspondiente (por ejemplo, '0' se convierte en '9', '1' se convierte en '8', etc.).
* Si el car�cter de entrada no es un d�gito, se devuelve sin cambios.
*
* @param p El car�cter a cifrar.
* @return El car�cter cifrado si p es un d�gito; en caso contrario, devuelve p sin cambios.
*/

char encriptarDigito(char p)
{
    if (isdigit(p))
        return char(57 - (int(p) - 48)); // sim�trico respecto a '9'
    return p;
}

/**
* @brief Encripta un car�cter determinando si es un d�gito o una letra.
*
* Esta funci�n comprueba si el car�cter de entrada es un d�gito o una letra y aplica
* la funci�n de encriptaci�n adecuada (encriptarDigito para d�gitos, encriptarLetra para letras).
* Los caracteres no alfanum�ricos se devuelven sin cambios.
*
* @param p El car�cter a encriptar.
* @return El car�cter encriptado.
*/

char encriptarCaracter(char p)
{
    if (isdigit(p))
        return encriptarDigito(p);
    else
        return encriptarLetra(p);
}

/**
 * @brief Encripta una l�nea de texto car�cter por car�cter.
 *
 * Esta funci�n toma una cadena de texto y aplica la funci�n encriptarCaracter
 * a cada uno de sus caracteres, construyendo y devolviendo la l�nea encriptada.
 *
 * @param linea La l�nea de texto original que se desea encriptar.
 * @return std::string La l�nea encriptada resultante.
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
