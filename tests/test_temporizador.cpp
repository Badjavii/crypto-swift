#include "../src/F08_temporizador.h"
#include "../src/F09_hora.h"
#include "../resources.h"
#include <iostream>
#include <cmath> // solo para hacer algo “costoso”

int main()
{
    cout << " Iniciando prueba temporal...\n";

    Temporizador t;

    // Tarea simulada que consuma tiempo
    volatile double acumulador = 0;
    for (long long i = 1; i <= 100'000'000; ++i)
    {
        acumulador += sqrt(i); // operación costosa
    }

    t.detener();

    cout << "\n Resultado\n";
    cout << "Inicio:    " << t.formatoTextoInicio() << "\n";
    cout << "Fin:       " << t.formatoTextoFin() << "\n";
    cout << "Duración:  " << t.formatoTextoDuracion() << "\n";
    cout << "Segundos:  " << t.duracionSegundos() << "s\n";

    cout << "\nResultado acumulador: " << acumulador << "\n";
    return 0;
}
