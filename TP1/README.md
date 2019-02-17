Domingo 24 de Febrero

# I. TAREA 1: RASTREADOR DE “System Calls”
El proposito de esta tarea individual es darle familiaridad con el ambiente Linux y aplicaciones de Sistemas Operativos. Toda la programacion debe realizarse en C sobre Linux. 

Su programa tendra la mision de poner a ejecutar a otro  programa (digamosle Prog), pasarle los argumentos seleccionados por el usuario y rastrear todos los system calls utilizados por Prog.

La sintaxis de ejecucion desde linea de comando es:
`rastreador [opciones rastreador] Prog
[opciones de Prog]`

Las [opciones rastreador] podrian no venir del todo o aparecer en cualquiier orden o combinacion valida. 

En todo caso, al final de la ejecucion de Prog, rastreador siempre desplegara en la salida estandar una  tabla acumulativa que muestre todos los System Calls utilizados por Prog, asi como el nmero de veces que fue utilizado cada uno.

Las [opciones de Prog] no seran analizadas ni consideradas por rastreador, sino que simplemente seran pasadas a Prog al iniciar su ejecucion. 

Las opciones validas para rastreador son: 
- v desplegara un mensaje cada vez que detecte un System Call de Prog . Se debe desplegar la mayor cantidad posible de detalles respecto a cada System Call.
- V sera identico a la opcion -v , pero hara una pausa  hasta que el usuario presione cualquier tecla para continuar la ejecucion de Prog . 

Esta tarea corta se desarrollara en los grupos de  proyecto.

Enviar un .tgz a torresrojas.cursos@gmail.com antes de la medianoche del Domingo 24 de Febrero del 2019 cuyo nombre sea la concatenacion de los apellidos del grupo con mayusculas al inicio de cada uno (e.g., 
Torres-Venegas-Araya.tgz) que contenga el fuente, un makefile, un readme que explique como se usa su programa, especialmente cualquier opcion extra incluida, y mencionando  todo lo que NO funcione de su proyecto. Identifique claramente su correo con el siguiente subject: [SOA] Tarea Corta 1 - Apellido 1 - Apellido 2 - etc.
