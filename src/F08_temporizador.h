#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

#include "F09_hora.h"
#include <optional>
#include <iomanip>
#include <sstream>
#include <vector>

class Temporizador
{
private:
    Hora inicio;
    Hora fin;
    bool detenido;
    vector<Hora> registro;

public:

    //
    Temporizador()
    {
        inicio = Hora();
        detenido = false;
        registro.push_back(inicio);
    }

    void registrar(){
        registro.push_back(Hora());
    }

    void detener()
    {
        fin = Hora();
        detenido = true;
    }

    void reiniciar()
    {
        inicio = Hora();
        detenido = false;
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

    string formatoTextoInicio()
    {
        return inicio.formatoTexto();
    }

    string formatoTextoFin()
    {
        if (detenido)
        {
            return fin.formatoTexto();
        }
        else
        {
            Hora ahora = Hora();
            return ahora.formatoTexto();
        }
    }

    vector<Hora> getRegistro(){
        return registro;
    }

    string formatoTextoDuracion() const
    {
        double t = duracionSegundos();
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
};

#endif
