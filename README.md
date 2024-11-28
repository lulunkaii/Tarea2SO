# Tarea2SO
Tarea 2 de sistemas operativos en el que se desarrolla un simulador de un monitor con hebras y un simulador de memoria virtual.
## Integrantes:
- Sofia López Aguilera 
- Nicolas López Cid
- Benjamín Espinoza Henríquez
- Martin Tapia Medina
### Instrucciones para la compilacion y ejecucion
1. Compilar los simuladores:
    ```sh 
    make
    ```
2. Ejecutar el programa:
    
    Para la parte 1:
    ```sh
    ./simulapc -p <numero-productores> -c <numero-consumidores> -s <size-inicial-cola> -t <tiempo-espera-max-consumidor>
    ```
    Para la parte 2:
    ```sh
    ./mvirtual -m <num-marcos-iniciales> -a <alg-reemplazo> -f <archivo-referencias>
    ```
3. Si desea limpiar los archivos compilados:
    ```sh
    make clean
    ```
