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

#endif
