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

## Conclusión

La implementación de hilos para ejecutar procesos en paralelo permitió reducir significativamente el tiempo total de ejecución, pasando de 759 ms en modo secuencial a 320 ms en paralelo, con una mejora del 57.76%. La mayoría de los procesos individuales también se completaron más rápido, lo que demuestra la eficiencia de la concurrencia en tareas repetitivas e independientes.

Aunque algunos procesos mostraron variaciones por la naturaleza paralela, el resultado general confirma que el uso de hilos mejora el rendimiento del programa. Esta estrategia es especialmente útil en aplicaciones que requieren ejecutar múltiples operaciones similares de forma simultánea.