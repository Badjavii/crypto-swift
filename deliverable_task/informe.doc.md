# Informe Proyecto SO: Optimización de Performance de Manejo de Archivos

- **Integrante:** *Otero Carrillo Javier Andrés*
- **Profesor:** *Hernández Omar*

## Breve explicación sobre la Estrategia Utilizada

Para la optimización del rendimiento en el manejo de archivos, se decidió implementar una metodología de ejecución concurrente mediante el uso de hilos.

Primero se definió lo que sería un "proceso" dentro del programa. Cada proceso N (donde N es el número del proceso) realiza el siguiente conjunto de tareas:

- Se parte de un archivo original llamado `original.txt`.
- Se realiza una copia de `original.txt` llamada `N.txt`.
- Se genera una versión encriptada de `N.txt` llamada `N.sha`.
- Se calculan dos hashes de `N.txt` y se comparan.
- Se desencripta `N.sha` en un archivo `N.des`.
- Se compara el contenido de `original.txt` con `N.des`.

Este conjunto de tareas se repite para cada proceso.

Como se mencionó, se utilizó concurrencia con hilos. A cada hilo se le asigna el número del proceso que debe ejecutar. Además, se utilizó un mutex para evitar condiciones de carrera al acceder a recursos compartidos.

Por lo tanto, si se requieren N procesos, se lanzan N hilos (uno por cada proceso), lo que permite que todos se ejecuten simultáneamente. Esta estrategia mejora significativamente el rendimiento al aprovechar la ejecución paralela.

## Comparación de Rendimiento

Se realizaron pruebas con N = 10 procesos. A continuación se presenta una tabla comparativa entre la ejecución secuencial y la paralela:

| Métrica                     | Secuencial   | Paralelo     |
| --------------------------- | ------------ | ------------ |
| Tiempo Inicial              | 00:49:41:351 | 00:49:42:112 |
| Tiempo Final                | 00:49:42:111 | 00:49:42:433 |
| Tiempo Total                | 00:00:00:759 | 00:00:00:320 |
| Tiempo Promedio por Proceso | 00:00:00:075 | 00:00:00:031 |
| Porcentaje de Mejora        | --           | 57.76 %      |

### Comparación de Tiempos Individuales por Proceso

| Proceso | Secuencial   | Paralelo     |
| ------- | ------------ | ------------ |
| 1       | 00:00:00:083 | 00:00:00:190 |
| 2       | 00:00:00:071 | 00:00:00:010 |
| 3       | 00:00:00:076 | 00:00:00:010 |
| 4       | 00:00:00:076 | 00:00:00:054 |
| 5       | 00:00:00:074 | 00:00:00:003 |
| 6       | 00:00:00:072 | 00:00:00:010 |
| 7       | 00:00:00:074 | 00:00:00:000 |
| 8       | 00:00:00:076 | 00:00:00:016 |
| 9       | 00:00:00:081 | 00:00:00:007 |
| 10      | 00:00:00:074 | 00:00:00:014 |

## Librerias Utilizadas

Durante el desarrollo del proyecto se utilizaron diversas librerias estándar de C++ para facilitar la concurrencia, el manejo de tiempo, la manipulación de archivos y la seguridad criptográfica. A continuación se describen las más relevantes:

### thread

La libreria `<thread>` permite crear y manejar hilos de ejecución en C++. En este proyecto, se utilizó para ejecutar múltiples procesos en paralelo, cada uno en su propio hilo. Esto permite que las tareas se realicen simultáneamente, aprovechando los núcleos del procesador y reduciendo el tiempo total de ejecución.

### mutex

La libreria <mutex> proporciona mecanismos de exclusión mutua para evitar condiciones de carrera. En este proyecto, se utilizó un mutex para proteger el acceso concurrente al objeto Temporizador, que es compartido por todos los hilos.

### chrono

La libreria `<chrono>` permite medir y manipular el tiempo con alta precisión. Se utilizó para capturar marcas de tiempo en milisegundos y microsegundos, calcular duraciones y formatear salidas como `HH:MM:SS:mmmm`.

Para facilitar su uso y encapsular la lógica de medición, se diseñaron dos clases auxiliares:

- **`Hora`**: captura la hora actual con precisión de hasta diezmilésimas de segundo y la formatea en texto legible.
- **`Temporizador`**: permite registrar múltiples marcas de tiempo, calcular duraciones entre eventos, medir el tiempo total de ejecución y obtener promedios por proceso.

Estas clases abstraen el uso directo de `chrono` y hacen que el código sea más legible, reutilizable y fácil de mantener.

### Otras librerias

- fstream:	Lectura y escritura de archivos.
- iomanip:	Formato de salida con setw, setfill, etc.
- sstream:	Construcción de cadenas con ostringstream.
- memory.h:	Manipulación de memoria (aunque no fue esencial en esta versión).
- vector:	Estructura de datos dinámica para almacenar hilos y registros de tiempo.
- utility:	Uso de std::pair para asociar ID de proceso con su duración.
- cctype:	Validación de caracteres con funciones como isdigit().
- direct.h:	Creación de directorios con _mkdir() en sistemas Windows.

## Configuracion previa del proyecto

A lo hora de probar el proyecto en las maquinas de los laboratorios de la universidad, se descubrió que los compiladores de dichas maquuionas no estan configuradas para usar versiones de c++ superiores a la v11. Para arreglarlo en DevC++ se hace lo siguiente

- Ir a herramientas > opciones de compilador > compilador
- una vez ahi, se escribe en el primer cuadro: -std=c++11
- seleccionar el cuadro que dice "añadir los siguientes comandos cuando llama al compilador"
- darle a aceptar para finalizar. 

Listo, con eso la maquina podra compilar el codigo fuente sin problemas.

## Conclusión

La implementación de hilos para ejecutar procesos en paralelo permitió reducir significativamente el tiempo total de ejecución, pasando de 759 ms en modo secuencial a 320 ms en paralelo, con una mejora del 57.76%. La mayoría de los procesos individuales también se completaron más rápido, lo que demuestra la eficiencia de la concurrencia en tareas repetitivas e independientes.

Aunque algunos procesos mostraron variaciones por la naturaleza paralela, el resultado general confirma que el uso de hilos mejora el rendimiento del programa. Esta estrategia es especialmente útil en aplicaciones que requieren ejecutar múltiples operaciones similares de forma simultánea.