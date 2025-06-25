#ifndef HORA_H
#define HORA_H
#include "../resources.h"

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

#endif
