#include <iostream>
#include <iomanip>  // para std::setw y std::setfill
#include <cstring>  // para strlen
#include "sha256.h" // tu archivo de cabecera

int main()
{
    const char *mensaje = "Hola Mundo";
    BYTE hash[SHA256_SIZE];

    sha256 contexto;
    contexto.sha_update(reinterpret_cast<const BYTE *>(mensaje), strlen(mensaje));
    contexto.sha_final(hash);

    std::cout << "Hash SHA-256 de \"" << mensaje << "\":" << std::endl;
    for (int i = 0; i < SHA256_SIZE; i++)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    std::cout << std::endl;

    return 0;
}
