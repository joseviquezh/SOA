#include "main.h"

ThreadInfo *thread_list[MAX_THREADS];
GtkWidget *main_hbox;
GtkWidget *main_vbox[4];
GtkWidget *thread_menu_header;
GtkWidget *combo_box1=NULL,*combo_box=NULL;
GtkWidget *entry=NULL, *entry1=NULL, *entry2=NULL, *entry3=NULL, *entry4=NULL;
int timer=0;
gulong handler_id = 0;

gboolean modoExpropiativo = TRUE;
gint number_of_threads=MAX_THREADS;
gint porcionTiempo = 100;
gint porcentajeTrabajo = 10;
gint cantidadTrabajo = 10000000;
gint cantidadBoletos = 1000;

int main (int argc,
          char *argv[])
{
  GtkWidget *assistant, *label, *button, *summit, *hbox, *label1, *hbox1, *hbox2, *vbox1, *label2, *hbox3, *vbox2;
  GtkWidget *label3, *label4, *hbox4, *hbox5, *vbox3, *label5, *label6, *hbox6, *hbox7, *summit1, *hbox9;
  GtkWidget *labelPicker, *picker;
  GtkWidget *labelPicker2, *summit2, *hbox8;
  GtkWidget *label7,*label8,*label9,*label10;
  guint i;
  char str[3];
  unsigned int iseed = (unsigned int)time(NULL);
  srand (iseed);
	
  PageInfo page[5] = {
    { NULL, -1, "Instrucciones",                     GTK_ASSISTANT_PAGE_INTRO,    TRUE},
    { NULL, -1, "Parámetros Generales",              GTK_ASSISTANT_PAGE_CONTENT,  FALSE},
    { NULL, -1, "Parámetros Específicos Para Hilos", GTK_ASSISTANT_PAGE_CONTENT,  FALSE},
    { NULL, -1, "Sorteo y cálculo de Pi",            GTK_ASSISTANT_PAGE_PROGRESS, FALSE},
    { NULL, -1, "Confirmación",                      GTK_ASSISTANT_PAGE_CONFIRM,  TRUE},
  };
  
  gtk_init (&argc, &argv);
 
  /* Create a new assistant widget with no pages. */
  assistant = gtk_assistant_new ();
  gtk_widget_set_size_request (assistant, 1080, 720);
  gtk_window_set_title (GTK_WINDOW (assistant), "SOA proyecto 1");
  g_signal_connect (G_OBJECT (assistant), "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);
  
  page[0].widget = gtk_label_new ("Proyecto 1 del curso Sistemas Operativos Avanzados.\n\n"\
                                  "Estudiantes: \n"\
				  "   Carlos Esteban Gómez Víquez \n"\
				  "   Jose Manuel Jiménez Chavarria \n"\
				  "   Carlos Luis Mena Mora \n\n"\
				  "Intrucciones de uso: \n"\
				  "   1) Presione el botón de Adelante\n"
				  "   2) Edite los parámetros de configuración general o carguelos desde un archivo\n"\
				  "   3) Presione el botón Set para confirmar los datos y continue presionando Adelante\n"\
				  "   4) Configure la cantidad de boletos y de trabajo para cada hilo\n"\
				  "   5) Presione Set para confirmar los datos de cada hilo y continue presionando Adelante\n"\
				  "   6) Presione Iniciar Sorteo para activar la ejecución de los hilos\n");

  page[1].widget = gtk_alignment_new (1, 0.5, 0.5, 0.1);
  page[2].widget = gtk_alignment_new (0.5, 0.5, 0.1, 0.1);
  page[3].widget = gtk_alignment_new (0.5, 0.5, 1, 0.05);
  page[4].widget = gtk_label_new ("Todos los threads han terminado \n");
  
  /* Create the necessary widgets for the second page. */
  vbox1 = gtk_vbox_new (FALSE, 5);
  label = gtk_label_new ("Número de threads: ");
  label1 = gtk_label_new ("Modo de operación: ");
  label5 = gtk_label_new ("PorcionTiempo (ms): ");
  label6 = gtk_label_new ("Porcentaje de trabajo (%): ");
  combo_box = gtk_combo_box_text_new ();
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), "Expropiativo");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), "No Expropiativo");
  gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), 0);
  entry = gtk_entry_new ();
  entry3 = gtk_entry_new ();
  entry4 = gtk_entry_new ();
  sprintf(str,"%d", MAX_THREADS);
  gtk_entry_set_text(GTK_ENTRY(entry),str);
  sprintf(str,"%d", porcionTiempo);
  gtk_entry_set_text(GTK_ENTRY(entry3),str);
  sprintf(str,"%d", porcentajeTrabajo);
  gtk_entry_set_text(GTK_ENTRY(entry4),str);
  hbox = gtk_hbox_new (FALSE, 5);
  hbox1 = gtk_hbox_new (FALSE, 5);
  hbox2 = gtk_hbox_new (FALSE, 5);
  hbox6 = gtk_hbox_new (FALSE, 5);
  hbox7 = gtk_hbox_new (FALSE, 5);
  hbox8 = gtk_hbox_new (FALSE, 5);
  summit = gtk_button_new_with_label ("Set");
  summit1 = gtk_button_new_with_label ("Set");
  labelPicker = gtk_label_new ("Archivo a Cargar:");
  picker = gtk_file_chooser_button_new ("Escoja Archivo a Cargar", 
                                         GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_box_pack_start (GTK_BOX (hbox), labelPicker, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox), picker, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox), summit, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox1), label, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox1), entry, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox2), label1, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox2), combo_box, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox6), label5, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox6), entry3, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox7), label6, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox7), entry4, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox2, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox6, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox7, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox, FALSE, FALSE, 5);
  gtk_container_add (GTK_CONTAINER (page[1].widget), vbox1);

  /* Create the necessary widgets for the third page. */
  hbox3 = gtk_hbox_new (FALSE, 5);
  vbox3 = gtk_vbox_new (FALSE, 5);
  label2 = gtk_label_new ("Hilo: ");
  combo_box1 = gtk_combo_box_text_new ();
  vbox2 = gtk_vbox_new (FALSE, 5);
  label3 = gtk_label_new ("Cantidad de Trabajo: ");
  label4 = gtk_label_new ("Cantidad de Boletos: ");
  entry1 = gtk_entry_new ();
  entry2 = gtk_entry_new ();
  hbox4 = gtk_hbox_new (FALSE, 5);
  hbox5 = gtk_hbox_new (FALSE, 5);
  labelPicker2 = gtk_label_new ("Archivo a Salvar:");
  
  summit2 = gtk_button_new_with_label ("Save");
  gtk_box_pack_start (GTK_BOX (hbox4), label3, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox4), entry1, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox5), label4, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox5), entry2, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox8), labelPicker2, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox8), summit2, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox4, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox5, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox8, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox3), label2, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox3), combo_box1, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox3), vbox3, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox3), vbox2, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox3), summit1, FALSE, FALSE, 5);
  gtk_container_add (GTK_CONTAINER (page[2].widget), hbox3);

  /* Create the necessary widgets for the fourth page. */
  button = gtk_button_new_with_label ("Iniciar sorteo");
  main_hbox= gtk_hbox_new (FALSE, 10);
  for( i=0;i<=4;i++){
     main_vbox[i]= gtk_vbox_new (FALSE, 17);
     gtk_box_pack_start (GTK_BOX (main_hbox),  main_vbox[i], TRUE,TRUE , 4); 
  }
  label7 = gtk_label_new ("#No");
  label8 = gtk_label_new ("Actividad");
  label9 = gtk_label_new ("Progreso ");
  label10 = gtk_label_new ("Valor Calculado de Pi");
  hbox9 = gtk_hbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX ( hbox9), label8, FALSE ,TRUE , 4);
  gtk_box_pack_start (GTK_BOX ( main_vbox[0]), label7, FALSE ,TRUE , 4);
  gtk_box_pack_start (GTK_BOX ( main_vbox[1]), hbox9, FALSE ,TRUE , 4);
  gtk_box_pack_start (GTK_BOX ( main_vbox[2]), label9, FALSE ,TRUE , 4);
  gtk_box_pack_start (GTK_BOX ( main_vbox[3]), label10, FALSE ,TRUE , 4);
 
  /**/
  for (i=0;i<MAX_THREADS;i++){
    
    thread_list[i] = ( ThreadInfo *) malloc(sizeof(ThreadInfo));
    thread_list[i]->checkbox = gtk_check_button_new (); /*inicializo checkbox*/
    thread_list[i]->pbar = gtk_progress_bar_new (); /*inicializo barra */
    thread_list[i]->checkbox_active=0; 
    thread_list[i]->textbox = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(thread_list[i]->textbox),FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET (thread_list[i]->checkbox), FALSE);
    gtk_button_set_alignment(GTK_BUTTON(thread_list[i]->checkbox),1,1);
    thread_list[i]->ThreadNumber=gtk_label_new (g_strdup_printf ("%d",i));
    thread_list[i]->identificador = i;
    thread_list[i]->ejecutado = false;
    thread_list[i]->finalizado = false;
    thread_list[i]->cantidadTrabajo = cantidadTrabajo;
    thread_list[i]->trabajoRealizado = 0;
    thread_list[i]->num = 0;
    thread_list[i]->den = 0;
    thread_list[i]->prev = 0;
    thread_list[i]->term = 0;
    thread_list[i]->result = 0;
    thread_list[i]->workPercentage = 0;
    thread_list[i]->cantidadBoletos = 1000;
    gtk_box_pack_start (GTK_BOX (main_vbox[0]), thread_list[i]->ThreadNumber, TRUE,TRUE , 0);					
    gtk_box_pack_start (GTK_BOX (main_vbox[1]), thread_list[i]->checkbox, TRUE,TRUE , 0);
    gtk_box_pack_start (GTK_BOX (main_vbox[2]), thread_list[i]->pbar, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (main_vbox[3]), thread_list[i]->textbox, TRUE, TRUE, 0);
    // g_object_set_data (G_OBJECT (page[3].widget), "pbar", (gpointer) progress);
  }
  gtk_box_pack_start (GTK_BOX (main_vbox[4]), button, FALSE, FALSE, 5);
  gtk_container_add (GTK_CONTAINER (page[3].widget), main_hbox);
  /* Add the five pages to the GtkAssistant dialog. */
  for (i = 0; i < 5; i++)
  {
    page[i].index = gtk_assistant_append_page (GTK_ASSISTANT (assistant),
                                               page[i].widget);
    gtk_assistant_set_page_title (GTK_ASSISTANT (assistant),
                                  page[i].widget, page[i].title);
    gtk_assistant_set_page_type (GTK_ASSISTANT (assistant),
                                  page[i].widget, page[i].type);

    /* Set the introduction and conclusion pages as complete so they can be
     * incremented or closed. */
    gtk_assistant_set_page_complete (GTK_ASSISTANT (assistant),
                                     page[i].widget, page[i].complete);
  }
  
  /* Signals */
  handler_id = g_signal_connect (G_OBJECT (combo_box1), "changed",
                    G_CALLBACK (combo_box1_changed), (gpointer) assistant);
  g_signal_connect (G_OBJECT (picker), "file-set",
                    G_CALLBACK (picker_seted), (gpointer) assistant);
  g_signal_connect (G_OBJECT (button), "clicked",
                    G_CALLBACK (button_clicked), (gpointer) assistant);
  g_signal_connect (G_OBJECT (summit), "clicked",
                    G_CALLBACK (disable_unused), (gpointer) assistant);
  g_signal_connect (G_OBJECT (summit1), "clicked",
                    G_CALLBACK (set_thread_info), (gpointer) assistant);
  g_signal_connect (G_OBJECT (summit2), "clicked",
                    G_CALLBACK (save_file), (gpointer) assistant);
  g_signal_connect (G_OBJECT (assistant), "cancel",
                    G_CALLBACK (assistant_cancel), NULL);
  g_signal_connect (G_OBJECT (assistant), "close",
                    G_CALLBACK (assistant_close), NULL);
  gtk_widget_show_all (assistant);
  for(i=0;i<MAX_THREADS;i++){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(thread_list[i]->checkbox),FALSE);
  }
  gtk_main ();
  return 0;
}

static void
combo_box1_changed (GtkEditable *combo_box,
               GtkAssistant *assistant)
{
  const gchar *text = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));
  gint value=atoi(text) - 1;
  gchar str[15];
  sprintf(str,"%d", thread_list[value]->cantidadTrabajo);
  gtk_entry_set_text(GTK_ENTRY(entry1),str);
  sprintf(str,"%d", thread_list[value]->cantidadBoletos);
  gtk_entry_set_text(GTK_ENTRY(entry2),str);
}

static void
picker_seted(GtkFileChooserButton *picker,
               GtkAssistant *assistant)
{

	gchar *text = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(picker));
	if(cargarArchivo(text))
	{
		g_print("Archivo cargado: %s \n",text);	
	}
	else
	{
		g_print("Archivo NO cargado: %s \n",text);	
	}
}

static void
button_clicked (GtkButton *button,
                GtkAssistant *assistant)
{
 
  gtk_widget_set_sensitive (GTK_WIDGET (button), FALSE);
  while (gtk_events_pending())
    gtk_main_iteration();
  calendarizador();
  /* Add a timer callback to update the value of the progress bar */
  GtkWidget *page = gtk_assistant_get_nth_page (assistant, 3);
  gtk_assistant_set_page_complete (assistant, page, TRUE);
    
}

static void
disable_unused (GtkButton *button,
                GtkAssistant *assistant)
{
  gint value=0;
  gchar str[15];
  gint i=0;

  const gchar *text0 = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));
  if (!strcmp(text0,"Expropiativo")){
    modoExpropiativo = TRUE;
  } else {
    modoExpropiativo = FALSE;
  }
  
  const gchar *text1 = gtk_entry_get_text (GTK_ENTRY (entry));
  value=atoi(text1);
  if (value >= 5 && value <= MAX_THREADS){
    number_of_threads = value;
  } else {
    sprintf(str,"%d", number_of_threads);
    gtk_entry_set_text(GTK_ENTRY(entry),str);
  }

  const gchar *text2 = gtk_entry_get_text (GTK_ENTRY (entry3));
  value=atoi(text2);
  if (value >= 0){
    porcionTiempo = value;
  } else {
    sprintf(str,"%d", porcionTiempo);
    gtk_entry_set_text(GTK_ENTRY(entry3),str);
  }

  const gchar *text3 = gtk_entry_get_text (GTK_ENTRY (entry4));
  value=atoi(text3);
  if (value > 0 && value <= 100){
    porcentajeTrabajo = value;
  } else {
    sprintf(str,"%d", porcentajeTrabajo);
    gtk_entry_set_text(GTK_ENTRY(entry4),str);
  }

  for(i=number_of_threads;i<MAX_THREADS;i++){
    gtk_widget_hide(thread_list[i]->ThreadNumber);
    gtk_widget_hide(thread_list[i]->checkbox);
    gtk_widget_hide(thread_list[i]->pbar);
    gtk_widget_hide(thread_list[i]->textbox);
  }

  g_signal_handler_disconnect(G_OBJECT (combo_box1), handler_id);

  GtkListStore *store;
  store = GTK_LIST_STORE (gtk_combo_box_get_model (GTK_COMBO_BOX (combo_box1)));
  gtk_list_store_clear (store);

  for(i=0;i<number_of_threads;i++){
    sprintf(str,"%d",(i+1));
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box1), str);
    if (thread_list[i]->cantidadBoletos <= 0) thread_list[i]->cantidadBoletos = cantidadBoletos;
    if (thread_list[i]->cantidadTrabajo <= 0) thread_list[i]->cantidadTrabajo = cantidadTrabajo;
  }

  handler_id = g_signal_connect (G_OBJECT (combo_box1), "changed",
                    G_CALLBACK (combo_box1_changed), (gpointer) assistant);

  gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box1), 0);

  gint num = gtk_assistant_get_current_page (assistant);
  GtkWidget *page = gtk_assistant_get_nth_page (assistant, num);
  gtk_assistant_set_page_complete (assistant, page, TRUE);
}

static void
set_thread_info (GtkButton *button,
                GtkAssistant *assistant)
{
  const gchar *text = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box1));
  gint value=atoi(text) - 1;

  const gchar *text0 = gtk_entry_get_text (GTK_ENTRY (entry1));
  gint cantTrabajo=atoi(text0);
  if (cantTrabajo > 0){
    thread_list[value]->cantidadTrabajo= cantTrabajo;
  }

  const gchar *text1 = gtk_entry_get_text (GTK_ENTRY (entry2));
  gint cantBoletos=atoi(text1);
  if (cantBoletos > 0){
    thread_list[value]->cantidadBoletos= cantBoletos;
  }

  gint num = gtk_assistant_get_current_page (assistant);
  GtkWidget *page = gtk_assistant_get_nth_page (assistant, num);
  gtk_assistant_set_page_complete (assistant, page, TRUE);
}

static void
save_file (GtkButton *button,
                GtkAssistant *assistant)
{
  	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new ("Save File",
					      NULL,
					      GTK_FILE_CHOOSER_ACTION_SAVE,
					      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
					      NULL);
	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), FALSE);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
		char *filename;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		guardarArchivo(filename);
		g_free (filename);
	}
	gtk_widget_destroy (dialog);
}

/* If the dialog is cancelled, delete it from memory and then clean up 
 * the Assistant structure. */
static void
assistant_cancel (GtkAssistant *assistant,
                  gpointer data)
{
  gtk_widget_destroy (GTK_WIDGET (assistant));
}

/* Destroy the assistant. */
static void
assistant_close (GtkAssistant *assistant,
                 gpointer data)
{
  gtk_widget_destroy (GTK_WIDGET (assistant));
}

bool cargarArchivo(char* nombreArchivo)
{
  FILE *fin = fopen(nombreArchivo, "r");
  char str[3];
  if (fin!=NULL){
    char modo[5];
    fscanf(fin,"ModoExpropiativo=%s\n",modo);
    if(strcmp (modo, "true") == 0){
      modoExpropiativo = true;
      gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), 0);
    }
    else{
      modoExpropiativo = false;
      gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), 1);
    }
    fscanf(fin,"CantidadHilos=%d\n",&number_of_threads);
    sprintf(str,"%d", number_of_threads);
    gtk_entry_set_text(GTK_ENTRY(entry),str);
    fscanf(fin,"PorcionTiempo=%d\n",&porcionTiempo);
    sprintf(str,"%d", porcionTiempo);
    gtk_entry_set_text(GTK_ENTRY(entry3),str);
    fscanf(fin,"PorcentajeTrabajo=%d\n",&porcentajeTrabajo);
    sprintf(str,"%d", porcentajeTrabajo);
    gtk_entry_set_text(GTK_ENTRY(entry4),str);
    int i;
    for(i=0; i<number_of_threads; i++)
      {
	fscanf(fin,"{CantidadBoletos=%d, CantidadTrabajo=%d}\n",&thread_list[i]->cantidadBoletos, &thread_list[i]->cantidadTrabajo);
      }
    fclose(fin);
    return true;
  }
  return false;
}

bool guardarArchivo(char* nombreArchivo)
{
  FILE *fin = fopen(nombreArchivo, "r");
  if (fin==NULL) {
    FILE *fout = fopen(nombreArchivo, "w");
    if (fout!=NULL) {
      fprintf(fout,"ModoExpropiativo=%s\n", modoExpropiativo ? "true" : "false");
      fprintf(fout,"CantidadHilos=%d\n",number_of_threads);
      fprintf(fout,"PorcionTiempo=%d\n",porcionTiempo);
      fprintf(fout,"PorcentajeTrabajo=%d\n",porcentajeTrabajo);
      int i;
      for(i=0; i<number_of_threads; i++)
	{
	  fprintf(fout,"{CantidadBoletos=%d, CantidadTrabajo=%d}\n",thread_list[i]->cantidadBoletos, thread_list[i]->cantidadTrabajo);
	}
      fclose(fout);
      return true;
    }
    else
      {
	return false;
      }
  }
  else {
    fclose(fin);
    return false;
  }
}

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

int sig_hilo(unsigned int boleto, unsigned int PosCola)
{
  if (PosCola >= number_of_threads)
    return 0;
  if (thread_list[PosCola]->cantidadBoletos > 0 && boleto <= thread_list[PosCola]->cantidadBoletos)
    return PosCola;
  if (thread_list[PosCola]->cantidadBoletos <= 0 || boleto > thread_list[PosCola]->cantidadBoletos)
    return sig_hilo(boleto - thread_list[PosCola]->cantidadBoletos, PosCola + 1);
  return 0;
}

int contarBoletos(unsigned int PosCola)
{
  int conteo;
  if (PosCola == number_of_threads)
    {
      conteo = 0;
    }
  else{
    conteo = thread_list[PosCola]->cantidadBoletos + contarBoletos(PosCola + 1);
  }
  return conteo;
}

void calendarizador()
{
  int cantBoletos;
  printf("------------------------------------------------------------------------------------------------------------------\n");
	printf("------------------------------------------------ESTADO INICIAL----------------------------------------------------\n");
	desplegar_Cola();
	printf("------------------------------------------------------------------------------------------------------------------\n");
	cantBoletos = contarBoletos(0);
  g_print("Cantidad Boletos: %d\n", cantBoletos);
  while(cantBoletos > 0)
    {
	 		
      if(sigsetjmp(jmpbufPri, 1) != 0)
	{

	  desplegar_Cola();
	  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(thread_list[hiloEjecutar]->checkbox),FALSE);
	  gtk_widget_set_sensitive (GTK_WIDGET (thread_list[hiloEjecutar]->checkbox), FALSE);
	  while (gtk_events_pending())
	     gtk_main_iteration(); 
	  printf("------------------------------------------------------------------------------------------------------------------\n");
	  cantBoletos = contarBoletos(0);
	  g_print("Cantidad Boletos: %d\n", cantBoletos);
	 
	}
      
      g_print("Cantidad Boletos: %d\n", cantBoletos);
      if (cantBoletos > 0){
	int boletoGanador = random_in_range (1, cantBoletos);
	g_print("Boleto ganador: %d\n", boletoGanador);
	hiloEjecutar = sig_hilo(boletoGanador, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(thread_list[hiloEjecutar]->checkbox),TRUE);
	gtk_widget_set_sensitive (GTK_WIDGET (thread_list[hiloEjecutar]->checkbox), TRUE);
	while (gtk_events_pending())
		gtk_main_iteration(); 
	g_print("Hilo ejecutar: %d\n", hiloEjecutar);
	g_print("Hilo ganador: %d\n", thread_list[hiloEjecutar]->identificador);
	if(modoExpropiativo){
	  g_print("Expropiativo\n");
	  signal(SIGALRM, sigalrm_handler);
	  struct itimerval tout_val;
	  tout_val.it_interval.tv_sec = 0;
	  tout_val.it_interval.tv_usec = 0;
	  tout_val.it_value.tv_sec = 0;
	  tout_val.it_value.tv_usec = porcionTiempo *1000;
	  setitimer(ITIMER_REAL, &tout_val,0);
	  if(thread_list[hiloEjecutar]->ejecutado){
	    g_print("Saltar a hilo %d\n", thread_list[hiloEjecutar]->identificador);
	    siglongjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
	  }
	  else{
	    procesarHiloExpropiativo();
	  }
	}else if(!modoExpropiativo){
	  
	  g_print("NO Expropiativo\n");
	  if(thread_list[hiloEjecutar]->ejecutado){
	    g_print("Saltar a hilo %d\n", thread_list[hiloEjecutar]->identificador);
	    siglongjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
	  }
	  else{
	    procesarHiloNoExpropiativo();
	  }
	}
      }
	else{g_print("NO HAY MAS BOLETOS %d\n", cantBoletos);}
    }
  sleep(1);
}

void sigalrm_handler(int sig)
{
  signal(SIGALRM, SIG_IGN);
  siglongjmp(jmpbufPri, 1);
}



void procesarHiloNoExpropiativo( )
{
  gdouble porcTrabajo = 0.0;
  gdouble porcTrabPrev = 0.0;
  gdouble tmp1 , tmp2;

  if(thread_list[hiloEjecutar] == NULL){
    return;
  }
  
  thread_list[hiloEjecutar]->ejecutado = true;
  thread_list[hiloEjecutar]->trabajoRealizado = 0.0;
  thread_list[hiloEjecutar]->num=1.0;
  thread_list[hiloEjecutar]->den=2.0;
  thread_list[hiloEjecutar]->prev=2.0;
  thread_list[hiloEjecutar]->term=0.0;
  thread_list[hiloEjecutar]->result=2.0;
  
  for (thread_list[hiloEjecutar]->trabajoRealizado = 0;
       thread_list[hiloEjecutar]->trabajoRealizado < (thread_list[hiloEjecutar]->cantidadTrabajo*UNIDAD_TRABAJO);
       thread_list[hiloEjecutar]->trabajoRealizado++){
    thread_list[hiloEjecutar]->term = thread_list[hiloEjecutar]->prev*(thread_list[hiloEjecutar]->num/thread_list[hiloEjecutar]->den);
    thread_list[hiloEjecutar]->prev = thread_list[hiloEjecutar]->term;
    thread_list[hiloEjecutar]->term = thread_list[hiloEjecutar]->prev*(1/(thread_list[hiloEjecutar]->den+1));
    thread_list[hiloEjecutar]->num = thread_list[hiloEjecutar]->num + 2.0;
    thread_list[hiloEjecutar]->den = thread_list[hiloEjecutar]->den + 2.0;
    thread_list[hiloEjecutar]->result = thread_list[hiloEjecutar]->result + thread_list[hiloEjecutar]->term;
    tmp1 = thread_list[hiloEjecutar]->trabajoRealizado;
    tmp2 = thread_list[hiloEjecutar]->cantidadTrabajo*UNIDAD_TRABAJO;
    porcTrabajo = tmp1 / tmp2 - porcTrabPrev;
    if ( porcTrabajo *100 >= porcentajeTrabajo){
      if (sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1) != 0)
	{
	  tmp1 = thread_list[hiloEjecutar]->trabajoRealizado;
          tmp2 = thread_list[hiloEjecutar]->cantidadTrabajo*UNIDAD_TRABAJO;
          porcTrabPrev = tmp1 / tmp2;
	}
      else
	{
	  g_print("Salto a Calendarizador\n");
	  siglongjmp(jmpbufPri, 1);
	}
    }
  }
  
  thread_list[hiloEjecutar]->finalizado = true;
  thread_list[hiloEjecutar]->cantidadBoletos = 0;
  siglongjmp(jmpbufPri, 1);
}

void procesarHiloExpropiativo()
{
  g_print("Ingreso al hilo\n");
  
  if(thread_list[hiloEjecutar] == NULL){
    return;
  }

  thread_list[hiloEjecutar]->ejecutado = true;
  sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
  thread_list[hiloEjecutar]->trabajoRealizado = 0.0;
  sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
  thread_list[hiloEjecutar]->num=1.0;
  sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
  thread_list[hiloEjecutar]->den=2.0;
  sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
  thread_list[hiloEjecutar]->prev=2.0;
  sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
  thread_list[hiloEjecutar]->term=0.0;
  sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
  thread_list[hiloEjecutar]->result=2.0;
  sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);

  for (thread_list[hiloEjecutar]->trabajoRealizado = 0;
       thread_list[hiloEjecutar]->trabajoRealizado < (thread_list[hiloEjecutar]->cantidadTrabajo*UNIDAD_TRABAJO);
       ){

    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
    thread_list[hiloEjecutar]->term = thread_list[hiloEjecutar]->prev*(thread_list[hiloEjecutar]->num/thread_list[hiloEjecutar]->den);
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
    thread_list[hiloEjecutar]->prev = thread_list[hiloEjecutar]->term;
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
    thread_list[hiloEjecutar]->term = thread_list[hiloEjecutar]->prev*(1/(thread_list[hiloEjecutar]->den+1));
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
    thread_list[hiloEjecutar]->numtmp = thread_list[hiloEjecutar]->num + 2.0;
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
    thread_list[hiloEjecutar]->dentmp = thread_list[hiloEjecutar]->den + 2.0;
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
    thread_list[hiloEjecutar]->num = thread_list[hiloEjecutar]->numtmp;
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
    thread_list[hiloEjecutar]->den = thread_list[hiloEjecutar]->dentmp;
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
    thread_list[hiloEjecutar]->resulttmp = thread_list[hiloEjecutar]->result + thread_list[hiloEjecutar]->term;
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
    thread_list[hiloEjecutar]->result = thread_list[hiloEjecutar]->resulttmp;
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);	
    //g_print("terms %lf, i %lf, num %lf, den %lf, result %32.30lf\n", terms, i, num, den, result);
    thread_list[hiloEjecutar]->trabajoRealizadotmp = thread_list[hiloEjecutar]->trabajoRealizado + 1;
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);	
    thread_list[hiloEjecutar]->trabajoRealizado = thread_list[hiloEjecutar]->trabajoRealizadotmp;
    sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);	

  }
  
  sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
  thread_list[hiloEjecutar]->finalizado = true;
  sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
  thread_list[hiloEjecutar]->cantidadBoletos = 0;
  sigsetjmp(thread_list[hiloEjecutar]->jmpbufHilo, 1);
  signal(SIGALRM, SIG_IGN);
  siglongjmp(jmpbufPri, 1);
}



void desplegar_Cola()
{
	int i;
	gdouble percent = 0.0;
	gchar *message=NULL;
	for(i= 0; i < number_of_threads; i++)
	{
		g_print("HILO:{");
		g_print("identificador:%d, ", thread_list[i]->identificador);
		g_print("ejecutado:%s, ", thread_list[i]->ejecutado ? "true" : "false");
		g_print("cantidad trabajo:%d, ", thread_list[i]->cantidadTrabajo);
		g_print("trabajo Realizado:%d, ", thread_list[i]->trabajoRealizado);
		g_print("cantidad boletos:%d, ", thread_list[i]->cantidadBoletos);
		g_print("finalizado:%s, ", thread_list[i]->finalizado ? "true" : "false");
		g_print("resultado:%lf, ", thread_list[i]->result);
		gtk_entry_set_text( GTK_ENTRY(thread_list[i]->textbox),g_strdup_printf ("%1.16lf",thread_list[i]->result));
		percent = ((gdouble)thread_list[i]->trabajoRealizado)/(((gdouble)thread_list[i]->cantidadTrabajo)*UNIDAD_TRABAJO);
		g_print("porcentaje:%lf", percent);
		if(percent<=1.000){
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(thread_list[i]->pbar), percent );
			message = g_strdup_printf ("%.0f%% Completo", percent*100);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR(thread_list[i]->pbar), message);
		}
		while (gtk_events_pending())
		gtk_main_iteration(); 
		g_print("}\n");
	}
}
