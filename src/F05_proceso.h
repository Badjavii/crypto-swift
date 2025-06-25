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

    cout << "Inicio: Copia " << i << endl;

    // 1- Copiar el archivo original.txt en i.txt
    archivoCopia = rutaTrabajo + to_string(i) + extensionCopia;
    cout << "   Copia " << i << ": " << archivoCopia << endl;
    generarCopia(archivoOriginal, archivoCopia);
    cout << "   Copia exitosa." << endl;

    // 2- Encriptar i.txt y guardar el resultado en i.sha
    archivoEncriptado = rutaTrabajo + to_string(i) + extensionEncriptado;
    cout << "   Encriptando copia " << i << endl;
    encriptarArchivo(archivoCopia, archivoEncriptado);
    cout << "   Encriptación exitosa." << endl;

    // 3- Generar el hash SHA-256 de i.txt
    cout << "   Generando hash de copia " << i << endl;
    hash1 = generarHashArchivo(archivoCopia);
    cout << "   Hash generado: " << hash1 << endl;

    // 4- Generar otro hash SHA-256 de i.txt
    cout << "   Generando otro hash de copia " << i << endl;
    hash2 = generarHashArchivo(archivoCopia);
    cout << "   Otro hash generado: " << hash2 << endl;

    // 5- Comparar los hashes
    cout << "   Comparando hashes de copia " << i << endl;
    resultadoComparacion = compararString(hash1, hash2);
    if (resultadoComparacion)
    {
        cout << "   Los hashes son iguales." << endl;
    }
    else
    {
        cout << "   Los hashes son diferentes." << endl;
    }

    // 6- Desencriptar i.sha en otro archivo i.des
    archivoDesencriptado = rutaTrabajo + to_string(i) + extensionDesencriptado;
    cout << "   Desencriptando copia " << i << endl;
    desencriptarArchivo(archivoEncriptado, archivoDesencriptado);
    cout << "   Desencriptación exitosa." << endl;

    // 7- Comparar el contenido de i.des con original.txt
    cout << "   Comparando contenido de copia " << i << " con original" << endl;
    resultadoComparacion = compararArchivos(archivoDesencriptado, archivoOriginal);
    if (resultadoComparacion)
    {
        cout << "   El contenido de la copia " << i << " es igual al original." << endl;
    }
    else
    {
        cout << "   El contenido de la copia " << i << " es diferente al original." << endl;
    }

    cout << "Fin Copia " << i << endl
         << "Tiempo de ejecución de la copia " << i << ": " << endl;
}

#endif // F05_PROCESO_H