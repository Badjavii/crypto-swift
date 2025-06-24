#include <iostream>
#include <string>
#include "sha256.h"
using namespace std;

// PARA COPIA N = 1
// 1- Copiar el contenido original.txt en copia1.txt
// 2- Encriptar copia1.txt
// 3- Sacar hash de copia1.txt
// 4- Sacar otro hash de copia1.txt
// 5- Comparar los hash
// 6- Desencriptar copia1.txt en otro archivo d_copia1.txt
// 7- Comparar el contenido de d_copia1.txt con original.txt

int main()
{
    const char *mensaje = "Hola Mundo";

    sha256 contexto;
    string hash = contexto.sha_return(mensaje);

    cout << "Mensaje: " << mensaje << endl;
    cout << "Hash SHA-256: " << hash << endl;

    return 0;
}
