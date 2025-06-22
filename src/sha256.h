/**
 * @file sha256.h
 * @brief Cabecera para la implementación del algoritmo SHA-256.
 *
 * Este archivo contiene las definiciones, macros y clases necesarias para implementar
 * el algoritmo de hash seguro SHA-256, que genera un hash de 256 bits (32 bytes) a partir
 * de un mensaje de entrada.
 *
 * @note Los operadores lógicos bit a bit utilizados en esta librería son:
 * - **&**: Conjunción (AND) – retorna 1 solo si ambos bits son 1.
 * - **|**: Disyunción (OR) – retorna 1 si al menos un bit es 1.
 * - **^**: Disyunción exclusiva (XOR) – retorna 1 si los bits son diferentes.
 * - **~**: Negación (NOT) – invierte todos los bits.
 * - **<<**: Desplazamiento a la izquierda – mueve bits y agrega ceros por la derecha.
 * - **>>**: Desplazamiento a la derecha – mueve bits a la derecha y completa con ceros.
 */

#ifndef SHA256_H
#define SHA256_H
#include <memory.h>

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
 * Rota los bits de la palabra `a` hacia la izquierda por `b` posiciones, moviendo los bits
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
 * Rota los bits de la palabra `a` hacia la derecha por `b` posiciones, moviendo los bits
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
 * Selecciona entre `y` y `z` según el valor de `x`. Si el bit de `x` es 1, retorna el bit
 * correspondiente de `y`; de lo contrario, retorna el bit de `z`.
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
 * Retorna el bit que representa la mayoría entre los bits correspondientes de `x`, `y` y `z`.
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
 * introducir entropía controlada en la palabra `x`. Utilizada en la transformación SHA-256.
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
 * introducir entropía controlada en la palabra `x`. Utilizada en la transformación SHA-256.
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
	r8 = 0x5be0cd19	 // Valor inicial del registro 8.
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
	BYTE data[64];			   // Arreglo para almacenar un bloque de 64 bytes (512 bits) de datos del mensaje.
	WORD datalen;			   // Cantidad de bytes almacenados en el arreglo `data`.
	unsigned long long bitlen; // Longitud total del mensaje en bits.
	WORD state[8];			   // Arreglo para almacenar los ocho registros de estado intermedio (32 bits cada uno).
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
	 * Configura los valores iniciales de la estructura `message` con los registros
	 * predefinidos (`r1` a `r8`) y establece las longitudes de datoslen y bitlen a cero.
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
	 * actualizando el estado intermedio almacenado en `message.state`.
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

public:
	/**
	 * @brief Constructor de la clase SHA256.
	 *
	 * Inicializa el objeto SHA256 llamando a `sha_init()` para configurar el estado inicial.
	 */

	sha256()
	{
		sha_init();
	}

	/**
	 * @brief Obtiene la estructura de mensaje actual.
	 *
	 * Retorna una copia de la estructura `message` que contiene los datos y el estado actual del mensaje.
	 *
	 * @return Estructura `sha256_message` con el estado actual.
	 */

	sha256_message getMessage() const
	{
		return message;
	}

	/**
	 * @brief Actualiza el estado de SHA-256 con nuevos datos.
	 *
	 * Procesa un bloque de datos de entrada y actualiza el estado intermedio del algoritmo.
	 * Los datos se acumulan en bloques de 64 bytes, y se aplica `sha_transform()` cuando
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
};

#endif // SHA256_H