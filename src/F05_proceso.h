#ifndef F05_PROCESO_H
#define F05_PROCESO_H
#include "../resources.h"
#include "F01_archivo.h"

void proceso(int i)
{
    const string rutaTrabajo = "file_workspace/";
    const string archivoOriginal = rutaTrabajo + "original.txt", extensionCopia = ".txt", extensionEncriptado = ".sha", extensionDesencriptado = ".des";
    string archivoCopia, archivoEncriptado, archivoDesencriptado;
    string hash1, hash2;
    bool resultadoComparacion;

    // 1- Copiar el archivo original.txt en i.txt
    archivoCopia = rutaTrabajo + to_string(i) + extensionCopia;
    generarCopia(archivoOriginal, archivoCopia);

    // 2- Encriptar i.txt y guardar el resultado en i.sha
    archivoEncriptado = rutaTrabajo + to_string(i) + extensionEncriptado;
    encriptarArchivo(archivoCopia, archivoEncriptado);

    // 3- Generar el hash SHA-256 de i.txt
    hash1 = generarHashArchivo(archivoCopia);

    // 4- Generar otro hash SHA-256 de i.txt
    hash2 = generarHashArchivo(archivoCopia);

    // 5- Comparar los hashes
    resultadoComparacion = compararString(hash1, hash2);

    // 6- Desencriptar i.sha en otro archivo i.des
    archivoDesencriptado = rutaTrabajo + to_string(i) + extensionDesencriptado;
    desencriptarArchivo(archivoEncriptado, archivoDesencriptado);

    // 7- Comparar el contenido de i.des con original.txt
    resultadoComparacion = compararArchivos(archivoDesencriptado, archivoOriginal);
}

#endif // F05_PROCESO_H