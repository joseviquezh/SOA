Lunes 18 de Marzo

I. DESCRIPCION ́

El proposito de este proyecto es crear y controlar nuestros propios  ́
threads a nivel de codigo de usuario (  ́ i.e., sin intervencion del kernel).  ́
Toda la programacion debe realizarse en C sobre Linux, no se puede  ́
usar de manera directa o indirecta ninguna biblioteca que implemente
threads (e.g., no se puede usar para nada Pthreads).

II. LOS DATOS

La primera parte de su programa leera los par  ́ ametros de la  ́
ejecucion. Aunque el despliegue final ser  ́ a de tipo gr  ́ afico, si les  ́
resulta conveniente los datos originales pueden ser le ́ıdos de un
archivo, o de teclado de manera texto. Los datos necesarios son:
Modo de operacion (expropiativo o no expropiativo).  ́
Numero de  ́ threads (m ́ınimo 5, maximo definido por el  ́
despliegue mas grande que puedan hacer).  ́
Cantidad de boletos para cada thread (numero entero).  ́
Cantidad de trabajo para cada thread.
Tamano del  ̃ quantum en milisegundos (para la version ́
expropiativa) o porcentaje del trabajo a realizar antes de
ceder voluntariamente el procesador (para la version no  ́
expropiativa). Por ejemplo, se podr ́ıa indicar que el quantum
es de 100 milisegundos, o que se quiere que se realice un
3 % del trabajo antes de ceder el CPU.
Con esa informacion, el programa crear  ́ a los  ́ threads de forma
ad hoc para este proyecto, y lanzara un peque  ́ no ̃ scheduler que
usara ́ Lottery Scheduling 123 para seleccionar el siguiente thread a
ejecutar (ya sea que el scheduler haya obtenido el control en forma
expropiativa o no expropiativa).
III. LOS Threads

Se creara una versi  ́ on simplificada de  ́ threads para este proyecto.
Estos se construiran con el uso adecuado de las funciones est  ́ andar  ́
de biblioteca de C sigsetjmp() y siglongjmp(). Consultando

previamente con el profesor se podr ́ıan usar otras funciones equiva-
lentes.

1C. A. Waldspurger, W. E. Weihl, “Lottery Scheduling: Flexible
Proportional-Share Resource Management”, Operating Systems Design and
Implementation (OSDI), 1994.
2D. Petrou, J, Milford, G. Gibson, “Implementing Lottery Scheduling:
Matching the Specializations in Traditional Schedulers”, Proc. USENIX, June,
1999.

3C. A. Waldspurger, W. E. Weihl, “Stride Scheduling: Determinis-
tic Proportional-Share Resource Management”, Technical Memorandum

MIT/LCS/TM-528, June 1995.

Cada thread hara alg  ́ un c  ́ alculo predeterminado que sea muy  ́
intensivo en uso de CPU, y actualizara un despliegue gr  ́ afico sencillo  ́
que muestra el avance hecho en el trabajo y el resultado parcial
obtenido. Al terminar, cada thread desplegara en la interfaz gr  ́ afica  ́
los resultados finales del calculo realizado.  ́
En el modo no expropiativo, los threads cederan voluntariamente  ́
el procesador despues de realizar una fracci  ́ on de su trabajo. En  ́
el modo expropiativo los threads no ceden el procesador, pero al
agotarse el quantum, el scheduler seleccionara otro  ́ thread para que
corra.

IV. TRABAJO
Notese que  ́ arctan(1) = π
4
, por lo que 4 arctan(1) = π. Usando
este hecho, nuestros threads calcularan el valor de  ́ π. La funcion ́
arctan sera aproximada usando una serie geom  ́ etrica (no usar Taylor)  ́
programada de manera iterativa4

. Entre mas t  ́ erminos tenga dicha  ́

serie, mas exacto ser  ́ a el valor de  ́ π.
La unidad de trabajo m ́ınima seran 50 t  ́ erminos de esta serie. As  ́  ́ı,
si indicamos que un thread tendra que hacer 4 unidades de trabajo,  ́
significa que debe calcular y acumular 200 terminos de la serie, o  ́
si a otro thread se le asignan 100 unidades de trabajo, este deber  ́ a ́
calcular y acumular 5000 terminos de esta serie.  ́
En el caso expropiativo cada thread trabajara continuamente,  ́
sin notar que el scheduler (activado por el timer) les expropia el
CPU cada cierto tiempo. En el caso no expropiativo, cada thread
calculara el porcentaje de t  ́ erminos que se le haya indicado y, de no  ́
haber terminado con todo su trabajo asignado, cedera voluntariamente  ́
el procesador al scheduler para que otro thread sea escogido (el
scheduler podr ́ıa escogerlo de nuevo).
V. DESPLIEGUE

Durante su trabajo, cada thread actualizara una barra que indique  ́
el porcentaje de su trabajo que ya haya terminado. Tambien cada  ́
thread mostrara el valor acumulado de la funci  ́ on requerida para este  ́
punto.
El scheduler indicara de alguna manera apropiada cu  ́ al ́ thread
esta activo en cada momento. Usar GTK o alg  ́ un equivalente gr  ́ afico.  ́

VI. FECHA DE ENTREGA

Demostraciones en clase el Lunes 18 de Marzo
del 2019. Tambien mandar por correo electr  ́ onico a  ́
torresrojas.cursos@gmail.com antes de la clase. Coloque
todo lo necesario para compilar, documentar y ejecutar su proyecto
en un directorio cuyo nombre este formado por los apellidos de  ́
los miembros de cada grupo separados por guiones. Compacte este
directorio en la forma de un .tgz llamado igual que el directorio
4Ya que el objetivo del ejercicio es calcular el valor de π, no use ninguna
serie que requiera π como dato de entrada.

(e.g., torres-venegas-castro-smith.tgz) y env ́ıelo por
correo. Identifique claramente su correo con el siguiente subject:
[SOA] Proyecto Programado 1 - Apellido 1 -
Apellido 2 - Apellido 3
Buena Suerte.
