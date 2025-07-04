#include <iostream>
#include <string>
#include <fstream>
#include <iomanip> // para std::setw y std::setfill
#include <sstream> // para std::ostringstream
#include <memory.h>
#include <chrono>
#include <mutex>
#include <vector>
#include <thread>
#include <utility>
using namespace std;

/**
 * @typedef BYTE
 * @brief Tipo de dato para un byte (8 bits).
 *
 * Representa un valor sin signo de 8 bits, utilizado para manipular datos a nivel de byte.
 */

typedef unsigned char BYTE;

/**
 * @typedef WORD
 * @brief Tipo de dato para una palabra de 32 bits.
 *
 * Representa un valor sin signo de 32 bits, utilizado para operaciones en el algoritmo SHA-256.
 */

typedef unsigned int WORD;

/**
 * @def SHA256_SIZE
 * @brief Tamaño del hash generado por SHA-256.
 *
 * Define el tamaño del hash resultante en bytes (32 bytes = 256 bits).
 */

#define SHA256_SIZE 32 // definir como constante el tamaño de 32 bits (4 bytes = 1 int)

/**
 * @def ROTLEFT(a, b)
 * @brief Realiza una rotación circular a la izquierda de una palabra de 32 bits.
 *
 * Rota los bits de la palabra a hacia la izquierda por b posiciones, moviendo los bits
 * que salen por la izquierda hacia la derecha.
 *
 * @param a Palabra de 32 bits a rotar.
 * @param b Número de posiciones para rotar.
 * @return Resultado de la rotación.
 */

#define ROTLEFT(a, b) (((a) << (b)) | ((a) >> (32 - (b))))

/**
 * @def ROTRIGHT(a, b)
 * @brief Realiza una rotación circular a la derecha de una palabra de 32 bits.
 *
 * Rota los bits de la palabra a hacia la derecha por b posiciones, moviendo los bits
 * que salen por la derecha hacia la izquierda.
 *
 * @param a Palabra de 32 bits a rotar.
 * @param b Número de posiciones para rotar.
 * @return Resultado de la rotación.
 */

#define ROTRIGHT(a, b) (((a) >> (b)) | ((a) << (32 - (b))))

/**
 * @def CHOISE(x, y, z)
 * @brief Función lógica de selección condicional.
 *
 * Selecciona entre y y z según el valor de x. Si el bit de x es 1, retorna el bit
 * correspondiente de y; de lo contrario, retorna el bit de z.
 *
 * @param x Bit de control para la selección.
 * @param y Primer valor de entrada.
 * @param z Segundo valor de entrada.
 * @return Resultado de la selección condicional.
 */

#define CHOISE(x, y, z) (((x) & (y)) ^ (~(x) & (z))) // Si el bit "x" es 1 elijo "y", sino elijo "z"

/**
 * @def MAJORITY(x, y, z)
 * @brief Función lógica de mayoría.
 *
 * Retorna el bit que representa la mayoría entre los bits correspondientes de x, y y z.
 * Es decir, retorna 1 si al menos dos de los tres bits son 1.
 *
 * @param x Primer valor de entrada.
 * @param y Segundo valor de entrada.
 * @param z Tercer valor de entrada.
 * @return Bit de mayoría.
 */

#define MAJORITY(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z))) // Se retorna el estado de bit con mayoria entre los bits "x, y, z"

/**
 * @def ENTROPY01(x)
 * @brief Función de entropía para mezclar bits (Σ0).
 *
 * Aplica una combinación de rotaciones circulares a la derecha y operaciones XOR para
 * introducir entropía controlada en la palabra x. Utilizada en la transformación SHA-256.
 *
 * @param x Palabra de 32 bits a procesar.
 * @return Resultado de la mezcla.
 */

#define ENTROPY01(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22)) // Mezcla el resultado de varias rotaciones para crear un desorden controlado

/**
 * @def ENTROPY02(x)
 * @brief Función de entropía para mezclar bits (Σ1).
 *
 * Similar a ENTROPY01, aplica rotaciones circulares a la derecha y operaciones XOR para
 * introducir entropía controlada en la palabra x. Utilizada en la transformación SHA-256.
 *
 * @param x Palabra de 32 bits a procesar.
 * @return Resultado de la mezcla.
 */

#define ENTROPY02(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25)) // [same]

/**
 * @def SIGMA01(x)
 * @brief Función de expansión de mensaje (σ0).
 *
 * Aplica rotaciones y desplazamientos a la derecha combinados con XOR para expandir el
 * mensaje durante la preparación del bloque en SHA-256.
 *
 * @param x Palabra de 32 bits a procesar.
 * @return Resultado de la expansión.
 */

#define SIGMA01(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3)) // Expandir el mensaje rotandolo

/**
 * @def SIGMA02(x)
 * @brief Función de expansión de mensaje (σ1).
 *
 * Similar a SIGMA01, aplica rotaciones y desplazamientos a la derecha combinados con XOR
 * para expandir el mensaje durante la preparación del bloque en SHA-256.
 *
 * @param x Palabra de 32 bits a procesar.
 * @return Resultado de la expansión.
 */

#define SIGMA02(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10)) // [same]

/**
 * @var k
 * @brief Tabla de constantes para el algoritmo SHA-256.
 *
 * Arreglo de 64 palabras de 32 bits que contiene las constantes predefinidas utilizadas
 * en la función de transformación SHA-256. Estas constantes son derivadas de las raíces
 * cúbicas de los primeros 64 números primos.
 */

static const WORD k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

/**
 * @enum registers
 * @brief Enumeración de valores iniciales para los registros de estado en SHA-256.
 *
 * Define los valores iniciales de los ocho registros utilizados para inicializar el estado
 * del algoritmo SHA-256. Estos valores son derivados de las raíces cuadradas de los
 * primeros ocho números primos.
 */

enum registers
{
    r1 = 0x6a09e667, // Valor inicial del registro 1.
    r2 = 0xbb67ae85, // Valor inicial del registro 2.
    r3 = 0x3c6ef372, // Valor inicial del registro 3.
    r4 = 0xa54ff53a, // Valor inicial del registro 4.
    r5 = 0x510e527f, // Valor inicial del registro 5.
    r6 = 0x9b05688c, // Valor inicial del registro 6.
    r7 = 0x1f83d9ab, // Valor inicial del registro 7.
    r8 = 0x5be0cd19  // Valor inicial del registro 8.
};

/**
 * @struct sha256_message
 * @brief Estructura para almacenar el estado y datos del mensaje en SHA-256.
 *
 * Contiene los datos del mensaje, su longitud y el estado intermedio del procesamiento
 * para la generación del hash SHA-256.
 */

struct sha256_message
{
    BYTE data[64];             // Arreglo para almacenar un bloque de 64 bytes (512 bits) de datos del mensaje.
    WORD datalen;              // Cantidad de bytes almacenados en el arreglo data.
    unsigned long long bitlen; // Longitud total del mensaje en bits.
    WORD state[8];             // Arreglo para almacenar los ocho registros de estado intermedio (32 bits cada uno).
};

/**
 * @class SHA256
 * @brief Clase que implementa el algoritmo de hash SHA-256.
 *
 * Proporciona métodos para inicializar, actualizar y finalizar el proceso de generación
 * del hash SHA-256, que produce un hash de 32 bytes a partir de un mensaje de entrada.
 */

class sha256
{
private:
    /** @brief Estructura que almacena el estado y datos del mensaje. */
    sha256_message message;

    /**
     * @brief Inicializa el estado del algoritmo SHA256.
     *
     * Configura los valores iniciales de la estructura message con los registros
     * predefinidos (r1 a r8) y establece las longitudes de datoslen y bitlen a cero.
     */
    void sha_init()
    {
        message.state[0] = r1;
        message.state[1] = r2;
        message.state[2] = r3;
        message.state[3] = r4;
        message.state[4] = r5;
        message.state[5] = r6;
        message.state[6] = r7;
        message.state[7] = r8;
        message.datalen = 0;
        message.bitlen = 0;
    }

    /**
     * @brief Transforma un bloque de 64 bytes en el proceso SHA-256.
     *
     * Realiza la transformación de compresión SHA-256 en un bloque de 64 bytes de datos,
     * actualizando el estado intermedio almacenado en message.state.
     *
     * @param[in] datos[] Arreglo de 64 bytes que contiene el bloque de datos a transformar.
     */

    void sha_transform(const BYTE data[])
    {

        WORD a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

        for (i = 0; i < 16; i++)
        {
            int byteIndex = i * 4;

            // Cada grupo de 4 bytes se convierte en una palabra de 32 bits (big endian)
            m[i] = ROTLEFT(data[byteIndex], 24); // Byte más significativo
            m[i] |= ROTLEFT(data[byteIndex + 1], 16);
            m[i] |= ROTLEFT(data[byteIndex + 2], 8);
            m[i] |= (data[byteIndex + 3]); // Byte menos significativo
        }

        /*
         * Conversión de bytes a palabras (big endian):
         * Cada grupo de 4 bytes consecutivos de 'data[]' se convierte en una palabra de 32 bits (WORD),
         * colocando cada byte en su posición correspondiente mediante desplazamiento.
         *
         * Aquí usamos ROTLEFT(x, n) como un alias de (x << n) porque estamos desplazando bytes de 8 bits
         * hacia su posición dentro de una palabra de 32 bits. Dado que los bytes no tienen más de 8 bits,
         * ROTLEFT(x, n) no provoca rotación circular (el comportamiento es idéntico al del operador <<).
         *
         * Ejemplo:
         *   data[0] = 0x12;
         *   data[1] = 0x34;
         *   data[2] = 0x56;
         *   data[3] = 0x78;
         *
         * Resultado:
         *   m[0] = ROTLEFT(0x12, 24) |
         *          ROTLEFT(0x34, 16) |
         *          ROTLEFT(0x56, 8)  |
         *          0x78;
         *        = 0x12345678;
         */

        for (; i < 64; ++i)
        {
            m[i] = SIGMA02(m[i - 2]);
            m[i] += m[i - 7];
            m[i] += SIGMA01(m[i - 15]);
            m[i] += m[i - 16];
        }

        a = message.state[0];
        b = message.state[1];
        c = message.state[2];
        d = message.state[3];
        e = message.state[4];
        f = message.state[5];
        g = message.state[6];
        h = message.state[7];

        for (i = 0; i < 64; ++i)
        {
            t1 = h + ENTROPY02(e) + CHOISE(e, f, g) + k[i] + m[i];
            t2 = ENTROPY01(a) + MAJORITY(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        message.state[0] += a;
        message.state[1] += b;
        message.state[2] += c;
        message.state[3] += d;
        message.state[4] += e;
        message.state[5] += f;
        message.state[6] += g;
        message.state[7] += h;
    }

    /**
     * @brief Actualiza el estado de SHA-256 con nuevos datos.
     *
     * Procesa un bloque de datos de entrada y actualiza el estado intermedio del algoritmo.
     * Los datos se acumulan en bloques de 64 bytes, y se aplica sha_transform() cuando
     * completa un bloque.
     *
     * @param[in] data[] Arreglo de bytes con los datos a procesar.
     * @param[in] len Tamaño de los datos en bytes.
     */

    void sha_update(const BYTE data[], size_t len)
    {
        WORD i;

        for (int i = 0; i < len; i++)
        {
            message.data[message.datalen] = data[i];
            message.datalen++;
            if (message.datalen == 64)
            {
                sha_transform(message.data);
                message.bitlen += 512;
                message.datalen = 0;
            }
        }
    }

    /**
     * @brief Finaliza el cálculo del hash SHA-256 y produce el hash final.
     *
     * Completa el procesamiento del mensaje, agrega el relleno necesario y genera el
     * de 32 bytes en el arreglo proporcionado.
     *
     * @param[out] hash Arreglo donde se almacena el hash final de 32 bytes.
     */

    void sha_final(BYTE hash[])
    {
        WORD i;

        i = message.datalen;

        // Se rellena con ceros hasta tener 56 bytes (para dejar espacio para la longitud)
        if (message.datalen < 56)
        {
            message.data[i++] = 0x80;
            while (i < 56)
                message.data[i++] = 0x00;
        }
        else
        {
            message.data[i++] = 0x80;
            while (i < 64)
                message.data[i++] = 0x00;
            sha_transform(message.data);
            memset(message.data, 0, 56);
        }

        // Se agrega la longitud total del mensaje (en bits) al final del bloque
        message.bitlen += message.datalen * 8;

        // La longitud ocupa 8 bytes (big endian), desde message.data[56] hasta [63]
        for (int j = 0; j < 8; j++)
        {
            message.data[63 - j] = (BYTE)(message.bitlen >> (j * 8));
        }
        sha_transform(message.data);

        //  Se convierten los 8 valores del estado en bytes individuales (big endian)
        for (i = 0; i < 4; ++i)
        {
            hash[i] = (message.state[0] >> (24 - i * 8)) & 0x000000ff;
            hash[i + 4] = (message.state[1] >> (24 - i * 8)) & 0x000000ff;
            hash[i + 8] = (message.state[2] >> (24 - i * 8)) & 0x000000ff;
            hash[i + 12] = (message.state[3] >> (24 - i * 8)) & 0x000000ff;
            hash[i + 16] = (message.state[4] >> (24 - i * 8)) & 0x000000ff;
            hash[i + 20] = (message.state[5] >> (24 - i * 8)) & 0x000000ff;
            hash[i + 24] = (message.state[6] >> (24 - i * 8)) & 0x000000ff;
            hash[i + 28] = (message.state[7] >> (24 - i * 8)) & 0x000000ff;
        }
    }

public:
    /**
     * @brief Constructor de la clase SHA256.
     *
     * Inicializa el objeto SHA256 llamando a sha_init() para configurar el estado inicial.
     */

    sha256()
    {
        sha_init();
    }

    /**
     * @brief Obtiene la estructura de mensaje actual.
     *
     * Retorna una copia de la estructura message que contiene los datos y el estado actual del mensaje.
     *
     * @return Estructura sha256_message con el estado actual.
     */

    sha256_message getMessage() const
    {
        return message;
    }

    /**
     * @brief Calcula el hash SHA-256 de la cadena de entrada dada y lo devuelve como una cadena hexadecimal.
     *
     * Esta función inicializa el contexto SHA-256, procesa la cadena de entrada,
     * finaliza el cálculo del hash y convierte los bytes del hash resultantes en
     * una representación de cadena hexadecimal.
     *
     * @param mensaje La cadena de entrada a hash.
     * @return string La representación hexadecimal del hash SHA-256.
     */

    string sha_return(const string &mensaje)
    {
        BYTE hash[SHA256_SIZE];
        sha_init();
        sha_update(reinterpret_cast<const BYTE *>(mensaje.c_str()), mensaje.size());
        sha_final(hash);

        ostringstream oss;
        for (int i = 0; i < SHA256_SIZE; i++)
            oss << hex << setw(2) << setfill('0') << (int)hash[i];
        return oss.str();
    }
};

// ################## ENCRIPTACION #######################

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

// ################## COMPARAR #######################

/**
 * @brief Compara dos cadenas de texto para determinar si son idénticas.
 *
 * Esta función compara las cadenas a y b utilizando el operador de igualdad de C++.
 * Devuelve true si las cadenas son idénticas (mismo contenido y longitud), y false
 * en caso contrario.
 *
 * @param a Primera cadena de texto a comparar.
 * @param b Segunda cadena de texto a comparar.
 * @return bool true si las cadenas son idénticas, false si son diferentes.
 */

bool compararString(string &a, string &b)
{
    return (a == b); // Compara dos cadenas de texto
}

// ################## ARCHIVO #######################

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

class Hora
{
private:
    int horas, minutos, segundos, milisegundos;

    void capturarTiempoActual()
    {
        auto ahora = chrono::system_clock::now();
        auto time_t_ahora = chrono::system_clock::to_time_t(ahora);
        auto tm = *localtime(&time_t_ahora);

        horas = tm.tm_hour;
        minutos = tm.tm_min;
        segundos = tm.tm_sec;

        auto ms = chrono::duration_cast<chrono::milliseconds>(
                      ahora.time_since_epoch()) %
                  1000;
        milisegundos = static_cast<int>(ms.count());
    }

public:
    Hora() { capturarTiempoActual(); }

    string formatoTexto() const
    {
        ostringstream oss;
        oss << setfill('0') << setw(2) << horas << ":"
            << setw(2) << minutos << ":"
            << setw(2) << segundos << ":"
            << setw(3) << milisegundos;
        return oss.str();
    }

    double tiempoEnSegundos() const
    {
        return horas * 3600.0 + minutos * 60.0 + segundos + milisegundos / 1000.0;
    }
};

class Temporizador
{
private:
    Hora inicio;
    Hora fin;
    bool detenido;
    vector<Hora> registro;

    string formatearDuracion(double t) const
    {
        if (t < 0)
            t += 86400.0;
        int h = static_cast<int>(t / 3600);
        t -= h * 3600;
        int m = static_cast<int>(t / 60);
        t -= m * 60;
        int s = static_cast<int>(t);
        int ms = static_cast<int>((t - s) * 1000);

        ostringstream oss;
        oss << setfill('0') << setw(2) << h << ":"
            << setw(2) << m << ":"
            << setw(2) << s << ":"
            << setw(3) << ms;
        return oss.str();
    }

public:
    //
    Temporizador()
    {
        inicio = Hora();
        detenido = false;
        registro.push_back(inicio);
    }

    void registrar()
    {
        registro.push_back(Hora());
    }

    void detener()
    {
        fin = Hora();
        detenido = true;
        registrar();
    }

    void reiniciar()
    {
        inicio = Hora();
        detenido = false;
        registro.clear();
        registro.push_back(inicio);
    }

    string duracionEntre(int i, int j) const
    {
        if (i >= 0 && j < static_cast<int>(registro.size()) && i < j)
        {
            double t = registro[j].tiempoEnSegundos() - registro[i].tiempoEnSegundos();
            return formatearDuracion(t);
        }
        return "00:00:00:000";
    }

    string tiempoTranscurrido() const
    {
        int i = static_cast<int>(registro.size()) - 1;
        return duracionEntre(0, i);
    }

    string formatoTextoInicio() const
    {
        return inicio.formatoTexto();
    }

    string formatoTextoFin() const
    {
        if (detenido)
        {
            return fin.formatoTexto();
        }
        else
        {
            Hora tempHora;
            return tempHora.formatoTexto();
        }
    }

    const vector<Hora> &getRegistro() const
    {
        return registro;
    }

    string promedioPorProceso() const
    {
        int n = static_cast<int>(registro.size());
        if (n <= 2)
            return "00:00:00:000";

        double total = 0.0;
        for (int i = 1; i < n - 1; ++i)
        {
            double t = registro[i].tiempoEnSegundos() - registro[i - 1].tiempoEnSegundos();
            total += t;
        }

        double promedio = total / (n - 2);
        return formatearDuracion(promedio);
    }

    double duracionSegundos() const
    {
        if (detenido)
        {
            return fin.tiempoEnSegundos() - inicio.tiempoEnSegundos();
        }
        else
        {
            Hora ahora = Hora();
            return ahora.tiempoEnSegundos() - inicio.tiempoEnSegundos();
        }
    }
};

void proceso(const string rutaTrabajo, int i)
{
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

Temporizador mainSecuencial(int copias)
{
    cout << endl;

    Temporizador temporizador_principal;
    const string rutaTrabajo = "secuencial/";

    cout << "================================" << endl;
    cout << "    INICIO PROCESO SECUENCIAL   " << endl;
    cout << "================================" << endl;
    cout << "Tiempo Inicial:     " << temporizador_principal.formatoTextoInicio() << endl;
    cout << "================================" << endl;
    cout << endl;

    for (int i = 1; i <= copias; i++)
    {
        proceso(rutaTrabajo, i);
        temporizador_principal.registrar();
        string duracion = temporizador_principal.duracionEntre(i - 1, i);
        if (i < 10)
        {
            cout << "TIEMPO PROCESO " << i << ":  " << duracion << endl;
        }
        else
        {
            cout << "TIEMPO PROCESO " << i << ": " << duracion << endl;
        }
        cout << "--------------------------------" << endl;
    }

    temporizador_principal.detener();

    cout << "================================" << endl;
    cout << "      FIN PROCESO SECUENCIAL    " << endl;
    cout << "================================" << endl;
    cout << "Tiempo Final:       " << temporizador_principal.formatoTextoFin() << endl;
    cout << "Tiempo Total:       " << temporizador_principal.tiempoTranscurrido() << endl;
    cout << "Tiempo promedio:    " << temporizador_principal.promedioPorProceso() << endl;
    cout << "================================" << endl;
    return temporizador_principal;
}

const string rutaTrabajo = "paralelo/";

// Función que crea y devuelve un hilo para ejecutar un proceso
thread crearHiloDeProceso(int id, Temporizador &temporizador, mutex &mutex_temporizador, vector<pair<int, string>> &tiempos_terminados)
{
    return thread([id, &temporizador, &mutex_temporizador, &tiempos_terminados]()
                  {
        // Ejecutar el proceso correspondiente
        proceso(rutaTrabajo, id);

        // Proteger acceso compartido
        lock_guard<mutex> lock(mutex_temporizador);

        // Registrar el tiempo de finalización del proceso
        temporizador.registrar();

        // Calcular la duración entre la última marca y la anterior
        int n = static_cast<int>(temporizador.getRegistro().size());
        if (n >= 2) {
            string duracion = temporizador.duracionEntre(n - 2, n - 1);
            tiempos_terminados.push_back({id, duracion});
        } });
}

Temporizador mainParalelo(int copias)
{
    cout << endl;

    Temporizador temporizador_principal;
    mutex mutex_temporizador;

    cout << "================================" << endl;
    cout << "    INICIO PROCESO PARALELO     " << endl;
    cout << "================================" << endl;
    cout << "Tiempo Inicial:     " << temporizador_principal.formatoTextoInicio() << endl;
    cout << "================================" << endl;
    cout << endl;

    vector<thread> hilos;
    vector<pair<int, string>> tiempos_terminados;

    for (int i = 1; i <= copias; ++i)
    {
        thread hilo = crearHiloDeProceso(i, temporizador_principal, mutex_temporizador, tiempos_terminados);
        hilos.push_back(move(hilo));
    }

    for (auto &hilo : hilos)
    {
        if (hilo.joinable())
            hilo.join();
    }

    temporizador_principal.detener();

    // Mostrar los tiempos en el orden en que terminaron los procesos
    for (const auto &par : tiempos_terminados)
    {
        int id = par.first;
        const string &duracion = par.second;

        cout << "TIEMPO PROCESO " << id << ": ";
        if (id < 10)
            cout << " ";
        cout << duracion << endl;
        cout << "--------------------------------" << endl;
    }

    cout << "================================" << endl;
    cout << "      FIN PROCESO PARALELO      " << endl;
    cout << "================================" << endl;
    cout << "Tiempo Final:       " << temporizador_principal.formatoTextoFin() << endl;
    cout << "Tiempo Total:       " << temporizador_principal.tiempoTranscurrido() << endl;
    cout << "Tiempo promedio:    " << temporizador_principal.promedioPorProceso() << endl;
    cout << "================================" << endl;
    return temporizador_principal;
}

int main()
{
    int N = 0;
    cout << "Indica el numero de copias a realizar (menor a 50): ";
    cin >> N;
    while (N < 1 || N > 50)
    {
        cout << "El numero debe ser entre 1 y 50. Intenta de nuevo: ";
        cin >> N;
    }

    Temporizador tiempoSecuencial = mainSecuencial(N);
    cout << endl;
    Temporizador tiempoParalelo = mainParalelo(N);

    double tiempoSec = tiempoSecuencial.duracionSegundos();
    double tiempoPar = tiempoParalelo.duracionSegundos();

    double mejora = ((tiempoSec - tiempoPar) / tiempoSec) * 100.0;

    cout << fixed << setprecision(2);
    cout << "================================" << endl;
    cout << "PORCENTAJE DE MEJORA: " << mejora << " %" << endl;
    cout << "================================" << endl;

    return 0;
}
