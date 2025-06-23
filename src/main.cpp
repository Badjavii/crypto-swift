#include <iostream>
#include <string>
#include "sha256.h"
using namespace std;

int main()
{
    const char *mensaje = "Hola Mundo";

    sha256 contexto;
    string hash = contexto.sha_return(mensaje);

    cout << "Mensaje: " << mensaje << endl;
    cout << "Hash SHA-256: " << hash << endl;

    return 0;
}
