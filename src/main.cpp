#include "../resources.h"
#include "F06_main_secuencial.h"
#include "F07_main_paralelo.h"

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
