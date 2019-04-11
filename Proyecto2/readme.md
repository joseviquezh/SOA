# Proyecto 2: Memoria Compartida: Productor-Consumidor

### I. DESCRIPCION

El proposito de este proyecto es experimentar con el uso de memoria compartida entre procesos heavyweight. Toda la programacion debe realizarse en C sobre Linux.

### II. PRELIMINARES

En caso de no tener experiencia en este tema, es necesario investigar el uso de mmap() en Linux. Se debe investigar la sincronizacion de procesos heavyweight con semaforos. Tambien, se debe saber como procesar argumentos de la línea de comandos de un programa ejecutado desde consola.

### III. RELACION PRODUCTOR - CONSUMIDOR
El concepto de productor-consumidor es fundamental en Sistemas Operativos. Numerosos problemas pueden ser modelados y resueltos con variantes de esta relacion.

Para este proyecto, los elementos producidos y consumidos seran mensajes a ser depositados en un buffer o buzon circular de tamano finito. Dicho buffer es creado e inicializado por un programa independiente a los productores y consumidores, y sera finalmente liberado por otro programa que cancela, de manera elegante, a todo el sistema de procesos creados en este proyecto.

Así, tenemos 4 tipos de procesos: creador, productores, consumidores y finalizador.

### IV. CREADOR

Este programa sera responsable de crear el buffer compartido por todos los procesos y de inicializar todas las variables de control asociadas al mismo (semaforos, banderas, contador de productores, contador de consumidores, etc.). El nombre del buffer, el tamano en entradas para mensajes, y cualquier otro parametro que cada grupo considere conveniente ser an recibidos de la línea de comando al ser ejecutado desde consola.

### V. PRODUCTORES

Esta es una familia de procesos, todos usando exactamente el mismo codigo, que se vinculan al buffer reservado por el creador. Con tiempos de espera aleatorios generaran mensajes que colocaran en el buffer (administrado circularmente).

Cada productor recibe como argumentos de la línea de comandos el nombre del buffer compartido y un parametro que indique la media en segundos de los tiempos aleatorios, siguiendo una distribucion exponencial, que deben esperar  antes de agregar un nuevo mensaje en el buffer. Por supuesto, este acceso debe darse de manera sincronizada ya que el buffer es compartido por multiples procesos. El usuario puede  ejecutar cuantas veces lo desee este programa, creando un nuevo productor que compite por el buffer, aunque cada vez podría indicarse una media de espera diferente. Al crearse un productor, este incrementa el contador de productores vivos.

Estos procesos repiten un ciclo de espera aleatoria y fabricacion de mensajes hasta que alg  un tipo de bandera  global en memoria compartida indique que el sistema se debe suspender. En este caso, los productores terminan, decrementan el contador de productores vivos, y despliegan su identificacion y algunas estadísticas basicas (n  umero de mensajes producidos,  acumulado de tiempos esperados, acumulado de tiempo que estuvo bloqueado por semaforos, etc.).

El formato específico del mensaje puede ser definido por cada grupo de trabajo, pero debe incluir al menos:
- Identificacion del productor
- Fecha y hora de creacion
- Llave aleatoria entre 0 y 4.

Cada vez que un mensaje logra ser puesto en el buffer, se debe desplegar un mensaje a la consola describiendo la accion realizada incluyendo el índice de la entrada donde se dejo el  mensaje y la cantidad de productores y consumidores vivos al instante de este evento.

### VI. CONSUMIDORES

Esta es una familia de procesos, todos usando exactamente el mismo codigo, que se vinculan al  buffer reservado por el creador y que con tiempos de espera aleatorios consumen mensajes tomados del buffer.

Cada consumidor recibe como argumentos de la línea de comandos el nombre del buffer compartido y un parametro que indique la media en segundos de los tiempos aleatorios, siguiendo una distribucion exponencial, que deben esperar antes de consumir el siguiente mensaje del buffer administrado circularmente. Por supuesto, este acceso debe darse de manera sincronizada ya que el buffer es compartido por multiples procesos.

El usuario puede ejecutar cuantas veces lo desee este programa, creando un nuevo consumidor que compite por el buffer, aunque cada vez podría indicarse una media de espera diferente. Cuando se crea un consumidor, lo primero que este hace es incrementar el contador de consumidores activos.

Estos procesos repiten un ciclo de espera aleatoria y consumo de mensajes hasta que lean un mensaje especial que indique que el sistema se deba suspender, o cuando al leer un mensaje este incluya una llave (numero entero entre 0 y 4) que sea igual  al process id o PID del consumidor modulo 5. En cualquiera  de estos dos casos, el consumidor termina, decrementa el contador de consumidores activos, despliega su identificacion y  algunas estadísticas basicas (numero de mensajes consumidos,  acumulado de tiempos esperados, acumulado de tiempo que estuvo bloqueado por semaforos, etc.).

Cada vez que un mensaje logra ser leído del buffer, se debe desplegar un mensaje a la consola describiendo la accion realizada incluyendo el índice de la entrada de adonde se tomo el mensaje y la cantidad de productores y consumidores vivos al instante de este evento).

### VII. FINALIZADOR

Este programa se encarga de cancelar todo el sistema de procesos, enviando mensajes de finalizacion a cada consumidor  vivo usando el buzon dise  nado para este proyecto, e indicandole a los productores que cesen actividades con alguna bandera global en memoria compartida. Una vez que la cantidad de productores y consumidores llega a cero, el buffer compartido es liberado. El finalizador debera dar mensajes y todas las  estadísticas posibles de su gestion.

### VIII. GENERALIDADES

Los programas son corridos desde consola. El uso correcto de la línea de argumentos sera muy tomado en cuenta para  la evaluacion de este proyecto. Todos los componentes de  este proyecto deben ser muy robustos y considerar cualquier combinacion de eventos y cualquier valor en los par  ametros.  Si la ejecucion no es posible, cada programa dar  a un mensaje  apropiado y terminara de manera controlada.

### IX. FECHA DE ENTREGA

Demostraciones en clase el Lunes 22 de Abril del 2018. Tambien mandar por correo electr  onico a  torresrojas.cursos@gmail.com antes de la clase. Coloque todo lo necesario para compilar, documentar y ejecutar su proyecto en un directorio cuyo nombre este formado por los apellidos de los miembros de cada grupo separados por guiones. Compacte este directorio en la forma de un .tgz llamado igual que el directorio (e.g., torres-venegas-castro-smith.tgz) y envíelo por correo. Identifique claramente su correo con el siguiente sub-
ject:
[SOA] Proyecto Programado 2 - Apellido 1 - Apellido 2 - Apellido 3
Buena Suerte.
