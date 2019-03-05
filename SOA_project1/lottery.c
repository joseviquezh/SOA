#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <setjmp.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>       /* for setitimer */
#include <string.h>
#include <gtk/gtk.h>

typedef struct _hilo hilo;

void sigalrm_handler(int sig);
int random_in_range (unsigned int min, unsigned int max);
hilo * sig_hilo(unsigned int boleto, hilo * colaHilos);
hilo * llenar_Cola(unsigned int MaxCantTrabajo, unsigned int MaxCantBoletos, unsigned int CantHilos, unsigned int ContHilos);
void desplegar_Cola(hilo * colaHilos);
void procesarHiloNoExpropiativo();
void procesarHiloExpropiativo();

hilo * colaHilosPri;
hilo * hiloEjecutar;
static sigjmp_buf jmpbufPri;
gdouble porcCalculo;
bool modoExpropiativo;
int porcionTiempo;

struct _hilo {
	int identificador;
 	bool ejecutado; /*true = si ya ha sido ejecutado al menos una vez, false = si no*/
 	gdouble trabajo; /*cantidad de trabajo del hilo*/
 	gdouble trabajoRealizado; /*cantidad de trabajo realizado del hilo*/
        gdouble num; /* Numerador para los terminos*/
        gdouble den; /* Denominador para los terminos*/
        gdouble prev; /* Termino previo */
        gdouble result; /* Resultado parcial de pi */
        gdouble workPercentage; //Between 0-100%
	int cantidadBoletos; /*Cantidad de boletos del hilo*/
	bool finalizado;
	sigjmp_buf jmpbufHilo; /*buffer de salto del hilo*/
 	hilo *siguienteHilo;
};

typedef struct _ConfigData {
  gint opMode; //0: Not expropriative 1:Expropriative
  gint threadsNum; //5 minimum
  gint ticketsNum; 
  gdouble workQuantity; //One unit is equivalent to 50 terms
  gint quantum; //In miliseconds
  gint workPercentage; //Between 0-100%
} ConfigData;

int random_in_range (unsigned int min, unsigned int max)
{
	int base_random = rand(); /* in [0, RAND_MAX] */
	if (RAND_MAX == base_random) return random_in_range(min, max);
	/* now guaranteed to be in [0, RAND_MAX) */
	int range       = max - min,
	remainder   = RAND_MAX % range,
	bucket      = RAND_MAX / range;
	/* There are range buckets, plus one smaller interval
		within remainder of RAND_MAX */
	if (base_random < RAND_MAX - remainder) {
    		return min + base_random/bucket;
  	} else {
    		return random_in_range (min, max);
  	}
}

hilo * sig_hilo(unsigned int boleto, hilo * colaHilos)
{
	hilo * hiloTemp = colaHilos;
	if (hiloTemp == NULL) return NULL;
	if (hiloTemp->cantidadBoletos > 0 && boleto <= hiloTemp->cantidadBoletos) return hiloTemp;
	if (hiloTemp->cantidadBoletos <= 0 || boleto > hiloTemp->cantidadBoletos) return sig_hilo(boleto - hiloTemp->cantidadBoletos, hiloTemp->siguienteHilo);
	return NULL;
}



int contarBoletos(hilo * colaHilos)
{
	int conteo;
	hilo * hiloTemp = colaHilos;
	if (hiloTemp == NULL)
	{
		conteo = 0;
	}
	else{
		conteo = hiloTemp->cantidadBoletos + contarBoletos(hiloTemp->siguienteHilo);
	}
	return conteo;
}

void calendarizador()
{
	int cantBoletos;
	printf("------------------------------------------------------------------------------------------------------------------\n");
	printf("------------------------------------------------ESTADO INICIAL----------------------------------------------------\n");
	desplegar_Cola(colaHilosPri);
	printf("------------------------------------------------------------------------------------------------------------------\n");
	cantBoletos = contarBoletos(colaHilosPri);
	printf("Cantidad Boletos: %d\n", cantBoletos);
	while(cantBoletos > 0)
	{
		if(sigsetjmp(jmpbufPri, 1) != 0)
		{
			
			printf("Salto con SETJUMP\n");
			desplegar_Cola(colaHilosPri);
			printf("------------------------------------------------------------------------------------------------------------------\n");
			cantBoletos = contarBoletos(colaHilosPri);
			printf("Cantidad Boletos: %d\n", cantBoletos);
		}
		if (cantBoletos > 0){
			int boletoGanador = random_in_range (1, cantBoletos);
			printf("Boleto ganador: %d\n", boletoGanador);
			hiloEjecutar = sig_hilo(boletoGanador, colaHilosPri);
			printf("Hilo ganador: %d\n", hiloEjecutar->identificador);
			if(modoExpropiativo){
				signal(SIGALRM, sigalrm_handler);
				struct itimerval tout_val;
				tout_val.it_interval.tv_sec = 0;
				tout_val.it_interval.tv_usec = 0;
				tout_val.it_value.tv_sec = 0;
				tout_val.it_value.tv_usec = porcionTiempo *1000;
				setitimer(ITIMER_REAL, &tout_val,0);
				if(hiloEjecutar->ejecutado){
					printf("Saltar a hilo %d\n", hiloEjecutar->identificador);
					siglongjmp(hiloEjecutar->jmpbufHilo, 1);
				}
				else{
					procesarHiloExpropiativo();
				}
			}else if(!modoExpropiativo){
				if(hiloEjecutar->ejecutado){
					printf("Saltar a hilo %d\n", hiloEjecutar->identificador);
					siglongjmp(hiloEjecutar->jmpbufHilo, 1);
				}
				else{
					procesarHiloNoExpropiativo();
				}
			}
		}
	}
}


void sigalrm_handler(int sig)
{
	printf("SE LLAMA CALENDARIZADOR POR TIEMPO\n");
	siglongjmp(jmpbufPri, 1);
}

int main( int   argc,
          char *argv[])
{
	unsigned int iseed = (unsigned int)time(NULL);
	srand (iseed);
	colaHilosPri = llenar_Cola(1000000000, 8000, 5, 1);
	modoExpropiativo = true;
	porcCalculo = 5;
	porcionTiempo = 100;
	calendarizador();
	return 0;
}

// procedimientos de apoyo

hilo * llenar_Cola(unsigned int MaxCantTrabajo, unsigned int MaxCantBoletos, unsigned int CantHilos, unsigned int ContHilos)
{
	if(CantHilos == 0){
		return NULL;
	}
	else
	{
		hilo* hiloTemp = malloc(sizeof(hilo));
		hiloTemp->identificador = ContHilos;
		hiloTemp->ejecutado = false;
		hiloTemp->trabajo  = random_in_range (1, MaxCantTrabajo);
		hiloTemp->trabajoRealizado  = 0;
		hiloTemp->cantidadBoletos  = random_in_range (1, MaxCantBoletos);
		hiloTemp->workPercentage = 0.25;
		hiloTemp->finalizado = false;
		hiloTemp->siguienteHilo = llenar_Cola(MaxCantTrabajo, MaxCantBoletos, CantHilos - 1, ContHilos + 1);
		return hiloTemp;
	}
}


void desplegar_Cola(hilo * colaHilos)
{
	hilo * hiloTemp = colaHilos;
	if(hiloTemp == NULL){
		return ;
	}
	else
	{
		printf("HILO: {");
		printf("identificador: %d, ", hiloTemp->identificador);
		printf("ejecutado: %s, ", hiloTemp->ejecutado ? "true" : "false");
		printf("trabajo: %lf, ", hiloTemp->trabajo);
		printf("trabajo Realizado: %lf, ", hiloTemp->trabajoRealizado);
		printf("cantidad boletos: %d, ", hiloTemp->cantidadBoletos);
		printf("finalizado: %s", hiloTemp->finalizado ? "true" : "false");
		printf("resultado: %lf", hiloTemp->result);
		printf("}\n");
		desplegar_Cola(hiloTemp->siguienteHilo);
		return;
	}
}

void procesarHiloNoExpropiativo( )
{
	printf("ingreso al hilo\n");
	gdouble porcTrabajo = 0.0;
	gdouble porcTrabPrev = 0.0;
	gdouble term=0.0;

	if(hiloEjecutar == NULL){
		return;
	}

	hiloEjecutar->ejecutado = true;
	hiloEjecutar->trabajoRealizado = 0.0;
	hiloEjecutar->num=1.0;
	hiloEjecutar->den=2.0;
	hiloEjecutar->prev=2.0;
	hiloEjecutar->result=2.0;

	for (hiloEjecutar->trabajoRealizado = 0; hiloEjecutar->trabajoRealizado<hiloEjecutar->trabajo; hiloEjecutar->trabajoRealizado++){
	  term = hiloEjecutar->prev*(hiloEjecutar->num/hiloEjecutar->den);
	  hiloEjecutar->prev = term;
	  term = hiloEjecutar->prev*(1/(hiloEjecutar->den+1));
	  hiloEjecutar->num = hiloEjecutar->num + 2.0;
	  hiloEjecutar->den = hiloEjecutar->den + 2.0;
	  hiloEjecutar->result = hiloEjecutar->result + term;
	  //g_print("terms %lf, i %lf, num %lf, den %lf, result %32.30lf\n", terms, i, num, den, result);
	  
	  porcTrabajo = hiloEjecutar->trabajoRealizado/hiloEjecutar->trabajo - porcTrabPrev;
	  //if ( porcTrabajo >= cdata->workPercentage){
	  if ( porcTrabajo >= hiloEjecutar->workPercentage){
	    if (sigsetjmp(hiloEjecutar->jmpbufHilo, 1) != 0)
	      {
		printf("Salto con SETJUMP  HILO NO EXPROPIATIVO\n");
		porcTrabPrev = hiloEjecutar->trabajoRealizado/hiloEjecutar->trabajo;
	      }
	    else
	      {
		printf("Salto a Calendarizador\n");
		siglongjmp(jmpbufPri, 1);
	      }
	  }
	}

	hiloEjecutar->finalizado = true;
	hiloEjecutar->cantidadBoletos = 0;
	siglongjmp(jmpbufPri, 1);
	/*
	printf("hilo %d\n", hiloEjecutar->identificador);
	int inicioTrabajo = hiloEjecutar->trabajoRealizado;
	while(true)
	{
		if(hiloEjecutar->trabajoRealizado == hiloEjecutar->trabajo){
			siglongjmp(jmpbufPri, 1);
		}
		else
		{	hiloEjecutar->ejecutado = true;
			hiloEjecutar->trabajoRealizado++;
			if (hiloEjecutar->trabajoRealizado == hiloEjecutar->trabajo){
				printf("Trabajo Finalizado\n");
				hiloEjecutar->finalizado = true;
				hiloEjecutar->cantidadBoletos = 0;
				siglongjmp(jmpbufPri, 1);
			}
			else if(!modoExpropiativo)
			{
				//system("clear");
			        //printf("trabajo: %d\n", hiloEjecutar->trabajo);
			        //printf("Realizado: %d\n", hiloEjecutar->trabajoRealizado);
				//printf("Modo No Expropiativo\n");
				calcTMP =hiloEjecutar->trabajoRealizado;
			        //printf("TEMP 1: %lf\n", calcTMP);
				calcTMP =calcTMP/hiloEjecutar->trabajo;
			        //printf("TEMP 2: %lf\n", calcTMP);
				porcTrabajo = calcTMP;
				calcTMP =inicioTrabajo;
				calcTMP =calcTMP/hiloEjecutar->trabajo;
				porcTrabajo = porcTrabajo - calcTMP;
				//printf("Trabajo realizado: %d\n", hiloEjecutar->trabajoRealizado);
				//printf("Trabajo total: %d\n", hiloEjecutar->trabajo);
				//printf("Trabajo INICIO: %d\n", inicioTrabajo);
				//printf("Procentaje de Trabajo Actual: %f\n", porcTrabajo);
				//printf("Procentaje de Calculo: %d\n", porcCalculo);
				if(porcTrabajo >= porcCalculo/100)
				{
					if (sigsetjmp(hiloEjecutar->jmpbufHilo, 1) != 0)
					{
						printf("Salto con SETJUMP  HILO NO EXPROPIATIVO\n");
						inicioTrabajo = hiloEjecutar->trabajoRealizado;
					}
					else
					{
						printf("Salto a Calendarizador\n");
						siglongjmp(jmpbufPri, 1);
					}
				}
			}
		}
	}
	*/
}




void procesarHiloExpropiativo()
{
	printf("ingreso al hilo\n");
	gdouble term=0.0;

	if(hiloEjecutar == NULL){
		return;
	}

	hiloEjecutar->ejecutado = true;
	hiloEjecutar->trabajoRealizado = 0.0;
	hiloEjecutar->num=1.0;
	hiloEjecutar->den=2.0;
	hiloEjecutar->prev=2.0;
	hiloEjecutar->result=2.0;
	sigsetjmp(hiloEjecutar->jmpbufHilo, 1);

	for (hiloEjecutar->trabajoRealizado = 0; hiloEjecutar->trabajoRealizado<hiloEjecutar->trabajo; hiloEjecutar->trabajoRealizado++){
	  sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
	  term = hiloEjecutar->prev*(hiloEjecutar->num/hiloEjecutar->den);
	  sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
	  hiloEjecutar->prev = term;
	  sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
	  term = hiloEjecutar->prev*(1/(hiloEjecutar->den+1));
	  sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
	  hiloEjecutar->num = hiloEjecutar->num + 2.0;
	  sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
	  hiloEjecutar->den = hiloEjecutar->den + 2.0;
	  sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
	  hiloEjecutar->result = hiloEjecutar->result + term;
	  sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
	  //g_print("terms %lf, i %lf, num %lf, den %lf, result %32.30lf\n", terms, i, num, den, result);
	}

	sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
	hiloEjecutar->finalizado = true;
	sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
	hiloEjecutar->cantidadBoletos = 0;
	sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
	siglongjmp(jmpbufPri, 1);

  /*
	printf("ingreso al hilo\n");
	if(hiloEjecutar == NULL){
		return;
	}
	printf("hilo %d\n", hiloEjecutar->identificador);
	while(true)
	{
		sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
		if(hiloEjecutar->trabajoRealizado == hiloEjecutar->trabajo){
			sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
			siglongjmp(jmpbufPri, 1);
		}
		else
		{	sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
			hiloEjecutar->ejecutado = true;
			sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
			hiloEjecutar->trabajoRealizado++;
			if (hiloEjecutar->trabajoRealizado == hiloEjecutar->trabajo){
				sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
				hiloEjecutar->finalizado = true;
				sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
				hiloEjecutar->cantidadBoletos = 0;
				sigsetjmp(hiloEjecutar->jmpbufHilo, 1);
				siglongjmp(jmpbufPri, 1);
			}
		}
	}
  */
}

