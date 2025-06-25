/**
 * @file test_archivo.cpp
 * @brief Prueba unitaria para las funciones de manejo de archivos.
 *
 * Este archivo contiene una prueba unitaria que verifica las funcionalidades de copia,
 * encriptación, desencriptación y comparación de archivos definidas en F01_archivo.h.
 * La prueba asegura que los procesos de encriptación y desencriptación sean reversibles
 * y que los hashes de los archivos reflejen cambios en el contenido.
 *
 * Dependencias:
 * - resources.h: Incluye librerías estándar de C++ (string, iostream, etc) para simplificar las inclusiones.
 * - F01_archivo.h: Contiene las funciones para manejar archivos (copia, encriptación, desencriptación, comparación).
 *
 * @author badjavii
 * @date 06-24-2025
 */

#include "../resources.h"
#include "../src/F01_archivo.h"

/**
 * @brief Ejecuta una prueba unitaria para las funciones de manejo de archivos.
 *
 * Realiza las siguientes operaciones:
 * - Genera una copia del archivo origin.txt en copia1.txt usando generarCopia.
 * - Encripta la copia en copia1_encriptada.txt usando encriptarArchivo.
 * - Desencripta el archivo encriptado en d_copia1.txt usando desencriptarArchivo.
 * - Muestra el contenido y el hash SHA-256 de origin.txt y copia1_encriptada.txt usando
 *   devolverContenidoArchivo y generarHashArchivo.
 * - Compara los hashes de origin.txt y copia1_encriptada.txt con compararString para verificar
 *   diferencias.
 * - Compara el contenido de origin.txt y d_copia1.txt con compararArchivos para confirmar
 *   que la desencriptación recupera el contenido original.
 *
 * @return int Retorna 0 si la prueba se ejecuta correctamente.
 */

const string workspace_root = "test_file_workspace/";

int main()
{
     string archivoEntrada = "origin.txt";
     string archivoCopia = "copia1.txt";
     string archivoEncriptado = "copia1_encriptada.txt";
     string archivoDesencriptado = "d_copia1.txt";

     // Generar una copia del archivo original
     generarCopia(workspace_root + archivoEntrada, workspace_root + archivoCopia);

     // Encriptar la copia del archivo
     encriptarArchivo(workspace_root + archivoCopia, workspace_root + archivoEncriptado);

     // Desencriptar el archivo encriptado
     desencriptarArchivo(workspace_root + archivoEncriptado, workspace_root + archivoDesencriptado);

     // Mostrar el contenido y los hashes de los archivos
     cout << "\n- Contenido del archivo: \n"
          << devolverContenidoArchivo(workspace_root + archivoEntrada) << endl;
     string hashEntrada = generarHashArchivo(workspace_root + archivoEntrada);
     cout << "\n- Hash del archivo: " << hashEntrada << endl;

     // Mostrar el contenido y el hash del archivo encriptado
     cout << "\n- Contenido de la copia encriptada: \n"
          << devolverContenidoArchivo(workspace_root + archivoEncriptado) << endl;
     string hashCopiaEncriptado = generarHashArchivo(workspace_root + archivoEncriptado);
     cout << "\n- Hash de la copia encriptada: " << hashCopiaEncriptado << endl;

     // Comparar los hashes
     bool sonIgualesHash = compararString(hashEntrada, hashCopiaEncriptado);
     cout << "\n- La copia y el archivo encriptado son iguales: " << (sonIgualesHash ? "Sí" : "No") << endl;

     // Comparar el contenido del archivo original y el desencriptado
     bool sonIgualesContenido = compararArchivos(workspace_root + archivoEntrada, workspace_root + archivoDesencriptado);
     cout << "\n- El contenido del archivo original y el desencriptado son iguales: " << (sonIgualesContenido ? "Sí" : "No") << endl;

     return 0;
}