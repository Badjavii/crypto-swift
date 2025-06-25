/**
 * @file F01_archivo.h
 * @brief Librería para el manejo de archivos con encriptación y comparación.
 *
 * Proporciona funciones para copiar, encriptar, desencriptar, comparar, leer el contenido
 * y generar hashes SHA-256 de archivos. Utiliza las librerías F02_encriptacion.h para
 * encriptación/desencriptación, F04_comparar.h para comparación de cadenas, y
 * F03_sha256.h para generación de hashes.
 *
 * Dependencias:
 * - resources.h: Incluye librerías estándar de C++ (string, iostream, etc) para simplificar las inclusiones.
 * - F02_encriptacion.h: Proporciona funciones de encriptación y desencriptación de caracteres.
 * - F04_comparar.h: Proporciona la función para comparar cadenas.
 * - F03_sha256.h: Proporciona la clase para generar hashes SHA-256.
 *
 * @author badjavii
 * @date 06-23-2025
 */

#ifndef F01_ARCHIVO_H
#define F01_ARCHIVO_H
#include "../resources.h" // Importa las librerías estándar de C++ necesarias para la implementación
#include "F02_encriptacion.h"
#include "F04_comparar.h"
#include "F03_sha256.h"

/**
 * @brief Genera una copia exacta de un archivo.
 *
 * Lee el archivo fuente byte por byte y escribe su contenido en el archivo destino.
 *
 * @param archivoEntrada Ruta del archivo fuente.
 * @param archivoDestino Ruta donde se guardará la copia.
 */
void generarCopia(const string &archivoEntrada, const string &archivoDestino)
{
    ifstream origen(archivoEntrada, ios::binary);  //
    ofstream destino(archivoDestino, ios::binary); //

    if (!origen.is_open() || !destino.is_open())
    {
        cerr << "Error al abrir los archivos\n";
        return;
    }

    char byte;
    while (origen.get(byte))
    {
        destino.put(byte); // Escribe exactamente cada byte leído
    }

    origen.close();
    destino.close();
}

/**
 * @brief Encripta un archivo y guarda el resultado.
 *
 * Lee el archivo fuente byte por byte, encripta cada carácter usando encriptarCaracter
 * y escribe el resultado en el archivo de salida.
 *
 * @param archivoEntrada Ruta del archivo a encriptar.
 * @param archivoSalida Ruta del archivo encriptado (se sobreescribe si existe).
 */
void encriptarArchivo(const string &archivoEntrada, const string &archivoSalida)
{
    ifstream entrada(archivoEntrada, ios::binary); // Abre el archivo de entrada en modo binario
    ofstream salida(archivoSalida, ios::binary);   // Abre el archivo de salida en modo binario

    if (!entrada.is_open() || !salida.is_open())
    {
        cerr << "Error al abrir los archivos\n";
        return;
    }

    char byte;
    while (entrada.get(byte))
    {
        salida.put(encriptarCaracter(byte)); // Encripta cada carácter y lo escribe en el archivo de salida
    }

    entrada.close();
    salida.close();
}

/**
 * @brief Desencripta un archivo y guarda el resultado.
 *
 * Lee el archivo encriptado byte por byte, desencripta cada carácter usando
 * desencriptarCaracter y escribe el resultado en el archivo de salida.
 *
 * @param archivoEntrada Ruta del archivo encriptado.
 * @param archivoSalida Ruta del archivo desencriptado.
 */
void desencriptarArchivo(const string &archivoEntrada, const string &archivoSalida)
{
    ifstream entrada(archivoEntrada, ios::binary); // Abre el archivo de entrada en modo binario
    ofstream salida(archivoSalida, ios::binary);   // Abre el archivo de salida en modo binario

    if (!entrada.is_open() || !salida.is_open())
    {
        cerr << "Error al abrir los archivos\n";
        return;
    }

    char byte;
    while (entrada.get(byte))
    {
        salida.put(desencriptarCaracter(byte)); // Desencripta cada carácter y lo escribe en el archivo de salida
    }
}

/**
 * @brief Compara el contenido de dos archivos línea por línea.
 *
 * Lee ambos archivos línea por línea y usa compararString para verificar si son idénticos.
 *
 * @param archivo1 Ruta del primer archivo.
 * @param archivo2 Ruta del segundo archivo.
 * @return bool true si los archivos son idénticos, false en caso contrario.
 */
bool compararArchivos(const string &archivo1, const string &archivo2)
{
    ifstream archi1(archivo1);
    ifstream archi2(archivo2);

    if (!archi1.is_open() || !archi2.is_open())
        return false; // Si alguno de los archivos no se abre, no son iguales

    string linea1, linea2;
    while (getline(archi1, linea1) && getline(archi2, linea2))
    {
        if (!compararString(linea1, linea2))
            return false; // Si alguna línea es diferente, los archivos no son iguales
    }

    bool resultado = archi1.eof() && archi2.eof(); // Ambos archivos deben llegar al final al mismo tiempo
    archi1.close();
    archi2.close();
    return resultado; // Retorna true si ambos archivos son iguales, false en caso contrario
}

/**
 * @brief Lee el contenido completo de un archivo.
 *
 * Lee el archivo byte por byte y retorna su contenido como una cadena.
 *
 * @param ruta Ruta del archivo a leer.
 * @return string Contenido del archivo, o cadena vacía si no se puede abrir.
 */
string devolverContenidoArchivo(const string &ruta)
{
    ifstream entrada(ruta, ios::binary); // Abre el archivo de lectura en modo binario para evitar problemas con caracteres especiales
    if (!entrada.is_open())
        return "";

    string contenido;
    char c;
    while (entrada.get(c))
    {
        contenido += c;
    }

    entrada.close();
    return contenido;
}

/**
 * @brief Genera el hash SHA-256 de un archivo.
 *
 * Lee el contenido del archivo con devolverContenidoArchivo y genera su hash SHA-256
 * usando sha_return de la clase sha256.
 *
 * @param archivo Ruta del archivo a procesar.
 * @return string Hash SHA-256 en formato hexadecimal, o cadena vacía si no se puede leer.
 */

string generarHashArchivo(const string &archivo)
{
    string contenido = devolverContenidoArchivo(archivo);
    if (contenido.empty())
        return "";

    sha256 contexto;
    return contexto.sha_return(contenido);
}

#endif // F01_ARCHIVO_H