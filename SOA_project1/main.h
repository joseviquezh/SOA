#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

static void combo_box1_changed    (GtkEditable*, GtkAssistant*);
static void picker_seted    (GtkFileChooserButton*, GtkAssistant*);
static void save_file (GtkButton *button, GtkAssistant *assistant);
static void button_clicked   (GtkButton*, GtkAssistant*);
static void assistant_cancel (GtkAssistant*, gpointer);
static void assistant_close  (GtkAssistant*, gpointer);
static void disable_unused (GtkButton *,GtkAssistant *);
static void set_thread_info (GtkButton *,GtkAssistant *);
void procesarHiloNoExpropiativo();
void procesarHiloExpropiativo();



void sigalrm_handler(int sig);
int random_in_range (unsigned int min, unsigned int max);
int sig_hilo(unsigned int boleto, unsigned int PosCola);

void calendarizador();

void desplegar_Cola();
void procesarHiloNoExpropiativo();
void procesarHiloExpropiativo();

static sigjmp_buf jmpbufPri;
int hiloEjecutar=0;

bool cargarArchivo(char* nombreArchivo);
bool guardarArchivo(char* nombreArchivo);
	
#define MAX_THREADS 20
#define UNIDAD_TRABAJO 50

typedef struct {
  GtkWidget *widget;
  gint index;
  const gchar *title;
  GtkAssistantPageType type;
  gboolean complete;
} PageInfo;

typedef struct{
	int identificador;
	GtkWidget *checkbox; /*Cada thread tiene asociado un checkbox*/
	gboolean checkbox_active; /* Actividad del checkbox */
	GtkWidget *pbar; /* Cada hilo tiene asociado una barra*/
	GtkWidget *textbox; /* Cada hilo tiene asociado una barra*/
	GtkWidget *ThreadNumber;
	gboolean ejecutado; /*true = si ya ha sido ejecutado al menos una vez, false = si no*/
	gboolean finalizado;
	gint cantidadTrabajo;         /*cantidad de trabajo del hilo*/
	gint trabajoRealizado; /*cantidad de trabajo realizado del hilo*/
	gint trabajoRealizadotmp;
	gdouble num; /* Numerador para los terminos*/
	gdouble den; /* Denominador para los terminos*/
	gdouble prev; /* Termino previo */
	gdouble term; /* Termino temporal, caso Expropiativo */
	gdouble result; /* Resultado parcial de pi */
	gdouble numtmp; /* Numerador para los terminos*/
	gdouble dentmp; /* Denominador para los terminos*/
	gdouble resulttmp; /* Resultado parcial de pi */
	gdouble workPercentage; //Between 0-100%
	gint cantidadBoletos; /*Cantidad de boletos del hilo*/
	sigjmp_buf jmpbufHilo; /*buffer de salto del hilo*/
} ThreadInfo; 
