#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/time.h>

#include "piApproximation.c"
#include "gui.h"
#include "timer/timer.h"

#define NUM_THREADS 5
#define MINIMUN_WORK_UNIT 50

void scheduler();

typedef struct{
    int id;
    int mode;               /* 0 expropiatory, 1 Non expropiatory */
    double result;          /* pi calculation result/progress */
    int executed;           /* To know if the thread has been executed yet */
    float workUnits;          /* Amount of work units:
                             * PI terms to calculate depending on the
                             * selected working units. 1 woking unit == 50 piTerms */
    double workPercentage;  /* Between 0-100% */
    int numTickets;         /* Individual number of assigned tickets */
    int* tickets;           /* Lottery tickets */
    sigjmp_buf buffer;      /* Thread buffer */
    int finnished;
    double denom;
    double numer;
    /* Widgets associated to the thread */
    GtkWidget *bar;
    GtkWidget *text_box;
    GtkWidget *spin;
    //--------
    float iterations;
} Thread;

/* Configuration data retrieved from the GUI */
struct thread_configuration{
    int mode; /* 0 expropiatory, 1 Non expropiatory */
    int threads_number;
    int number_tickets;
    int amount_work;
    int quantum;
    double work_percent;
    int current_selection;  /* To know which thread is being configured */
};

int QUANTUM_SIZE = 0;           /* Defined in miliseconds */
int NUM_TICKETS = 0;            /* Total amount of tickets to be assigned */
Thread* THREADS[NUM_THREADS];
Thread* runningThread;
sigjmp_buf parent;
int last_thread = 0;
sigjmp_buf last_buffer;
GuiObjects *gui=NULL;
struct thread_configuration* config;

G_MODULE_EXPORT void
create_about_page (GtkImageMenuItem *MenuItem)
{
    GtkWidget *about_window = NULL;
    GtkWidget *text = NULL;

    /* Create the window */
    about_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /* Edit the name of the window */
    gtk_window_set_title(GTK_WINDOW(about_window), "About");
    /* Set the size of the window */
    gtk_window_set_default_size(GTK_WINDOW(about_window), 640, 480);
    /* Text box shown in the window */
    text = gtk_label_new ("Instituto Tecnológico de Costa Rica\n\n"\
                            "MC-6004 - Sistemas Operativos Avanzados\n\n"\
                            "Proyecto I: Loterı́a de Threads (v0.1)\n\n"\
                            "Estudiantes:\n"\
                            "   Jose Víquez\n"\
                            "   Benjamin Lewis\n"\
                            "   Jafet Chaves\n"\
                            "   Víctor Arce\n\n"\
                            "Supervisado por: Phd. Francisco Torres Rojas\n\n\n\n");
    /* Add the text widget to the window */
    gtk_container_add(GTK_CONTAINER(about_window), text);
    /* Show the window */
    gtk_widget_show_all(about_window);
}

G_MODULE_EXPORT void
button_clicked (GtkButton *button)
{
    g_print("Thread scheduling started\n");
    g_print("Number of tickets: %d\n", config->number_tickets);
    g_print("Amount of work: %d\n", config->amount_work);
    g_print("Quantum: %d\n", config->quantum);
    g_print("Work Percent: %f\n", config->work_percent);
    g_print("Mode: %d\n", config->mode);

    /* Initialize all the threads */
    for(int thread = 0; thread < NUM_THREADS; ++thread){
        THREADS[thread]->id = thread;
        THREADS[thread]->mode = config->mode;
        THREADS[thread]->result = 0;
        THREADS[thread]->executed = 0;
        THREADS[thread]->workUnits = config->amount_work*MINIMUN_WORK_UNIT;
        THREADS[thread]->workPercentage = config->work_percent;
        THREADS[thread]->tickets = malloc(config->number_tickets * sizeof(int));
        THREADS[thread]->numer= 4.0;
        THREADS[thread]->denom= 0;
        THREADS[thread]->iterations= 0;

        /* Add the tickets of each individual thread */
        NUM_TICKETS += THREADS[thread]->numTickets;
    }
    QUANTUM_SIZE = config->quantum;

    /*Disable execute button after execution taking effect*/
    gtk_widget_set_sensitive (GTK_WIDGET(button), FALSE);

    /* Call the scheduler to start the program */
    scheduler();

    /* Free allocated memory */
    for(int thread = 0; thread < NUM_THREADS; ++thread){
        free(THREADS[thread]->tickets);
    }
}

G_MODULE_EXPORT void
entry_activate_number_tickets (GtkEntry *entry, gpointer user_data)
{
    config->number_tickets = atoi(gtk_entry_get_text (entry));
    g_print( "Current selected thread: %d\n",  config->current_selection);
    g_print( "Number of tickets: %d\n",  config->number_tickets);
    THREADS[config->current_selection]->numTickets = config->number_tickets;
}

G_MODULE_EXPORT void
entry_activate_amount_work (GtkEntry *entry, gpointer user_data)
{
    config->amount_work = atoi(gtk_entry_get_text (entry));
    g_print( "Current selected thread: %d\n",  config->current_selection);
    g_print("Amount of work: %d\n", config->amount_work);
}

G_MODULE_EXPORT void
entry_activate_quantum (GtkEntry *entry, gpointer user_data)
{
    struct Thread *d = user_data;
    if(config->mode == 1){
        config->work_percent = atoi(gtk_entry_get_text (entry));
        THREADS[config->current_selection]->workPercentage = config->work_percent;
    }
    else {
        config->quantum = atoi(gtk_entry_get_text (entry));
        QUANTUM_SIZE = config->quantum;
    }
    g_print( "Current selected thread: %d\n",  config->current_selection);
    g_print("config->work_percent: %f\n", config->work_percent);
    g_print("config->quantum: %d\n", config->quantum);
}

G_MODULE_EXPORT void
activate_combo_box0 (GtkComboBox *combo_box)
{
    config->mode = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_box));
    if(config->mode == 1){
        gtk_label_set_text (GTK_LABEL(gui->quantum_label),"Work Percentage");
    }
    else {
        gtk_label_set_text (GTK_LABEL(gui->quantum_label),"Quantum");
    }
    g_print( "Current selected thread: %d\n",  config->mode);
}

G_MODULE_EXPORT void
activate_combo_box1 (GtkComboBox *combo_box)
{
    config->current_selection = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_box));
    g_print( "Current selected thread: %d\n",  config->current_selection);
}

/* Callback that is fired when the timer's time is up */
void timerHandler(int pSig)
{
    printf("DEBUG: Time is up!!! \n");
    siglongjmp(parent, 1);
}

void swap (int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void updateUI(){
    float progress=runningThread->iterations/runningThread->workUnits;
    gchar* message = g_strdup_printf ("PID: %d/ %f", runningThread->id, progress);
    gtk_progress_bar_set_text (GTK_PROGRESS_BAR(THREADS[runningThread->id]->bar), message);
    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(THREADS[runningThread->id]->bar), progress);
    gtk_entry_set_text(GTK_ENTRY(THREADS[runningThread->id]->text_box),g_strdup_printf ("%1.16lf",runningThread->result));
    while (gtk_events_pending()){
        gtk_main_iteration();
    }
}

void calculatePi(){
    gtk_spinner_start(GTK_SPINNER(THREADS[runningThread->id]->spin));

    runningThread->executed = 1;
    sigsetjmp(runningThread->buffer, 1);

    float progress; // percent
    sigsetjmp(runningThread->buffer, 1);

    long long int calculatedTerms,termsToCalculate;
    sigsetjmp(runningThread->buffer, 1);

    double term = 0.0;
    sigsetjmp(runningThread->buffer, 1);

    if(runningThread->mode == 1){
        calculatedTerms = 0;
        termsToCalculate = (runningThread->workUnits * 100 )/runningThread->workPercentage;
    }

    /*
        Expropiative: do work during a certaing amount of time
        Non-expropiative: do a specific work percentage
    */

    for(int piTerm = 0; piTerm < runningThread->workUnits; ++piTerm){
        runningThread->denom = (2 * piTerm + 1);
        sigsetjmp(runningThread->buffer, 1);
        double term = runningThread->numer/runningThread->denom;
        sigsetjmp(runningThread->buffer, 1);
        if(piTerm % 2 == 0){
            runningThread->result += term;
            sigsetjmp(runningThread->buffer, 1);
        }
        else{
            runningThread->result -= term;
            sigsetjmp(runningThread->buffer, 1);
        }

        if(runningThread->mode == 1 && calculatedTerms++ == termsToCalculate){
            if(sigsetjmp(runningThread->buffer, 1) == 0) siglongjmp(parent, 1);
            calculatedTerms = 0;
        }

        runningThread->iterations++;
        updateUI();
    }

    printf("DEBUG: Process %d ended its execution\n", runningThread->id);
    sigsetjmp(runningThread->buffer, 1);
    runningThread->finnished = 1;
    sigsetjmp(runningThread->buffer, 1);
}

void scheduler(){
    // Create an array with all the ticket numbers
    int tickets[NUM_TICKETS];
    gchar *message = NULL;
    for(int i = 0; i < NUM_TICKETS; ++i){
        // Choose a random and unique number
        tickets[i] = i + 1;
    }

    // Shuffle the tickts array
    srand(time(NULL));
    for (int i = NUM_TICKETS-1; i > 0; i--){
        int j = rand() % (i+1);
        swap(&tickets[i], &tickets[j]);
    }

    // Assign the corresponding amount of tickets to each thread
    int startIndex = 0, endIndex = 0;
    for(int threadId = 0; threadId < NUM_THREADS; ++threadId){
        startIndex = endIndex;
        endIndex += THREADS[threadId]->numTickets;
        for(int i = 0; i < THREADS[threadId]->numTickets; ++i){
            THREADS[threadId]->tickets[i] = tickets[startIndex++];
        }
    }

    // Shuffle the tickts array
    for (int i = NUM_TICKETS-1; i > 0; i--){
        int j = rand() % (i+1);
        swap(&tickets[i], &tickets[j]);
    }

    // Randomly select a winning ticket until there are no tickets
    for(int i = 0; i < NUM_TICKETS; ++i){
        printf("\nDEBUG: Selecting a wining ticket\n");
        while (gtk_events_pending()){
            gtk_main_iteration();
        }
        int ticket = tickets[i];
        int threadId;
        int winnerFound = 0;
        for(threadId = 0; threadId < NUM_THREADS; ++threadId){
            for(int j = 0; j < THREADS[threadId]->numTickets; ++j){
                if (THREADS[threadId]->tickets[j] == ticket){
                    winnerFound = 1;
                    break;
                }
            }
            if(winnerFound) break;
        }
        if(threadId < NUM_THREADS){
            printf("DEBUG: The winning ticket is %d and the winner is %d\n", ticket, threadId);
            runningThread = THREADS[threadId];
            if(runningThread->finnished == 0){
                if(sigsetjmp(parent, 1) == 0){
                    if(runningThread->mode == 0){
                        setUpTimer(timerHandler);
                        setTimer(QUANTUM_SIZE);
                    }
                    if(runningThread->executed){
                        printf("DEBUG: The proces was already executed\n");
                        siglongjmp(runningThread->buffer, threadId);
                    }
                    else{
                        printf("DEBUG: The proces wasn't already executed\n");
                        calculatePi();
                    }
                }
                gtk_spinner_stop(GTK_SPINNER(THREADS[runningThread->id]->spin));
            }
            else{
                updateUI();
                printf("DEBUG: The proces already finnished its calculations\n");
            }
        }
        else{
            --i;
        }
    }

    printf("\nAll the tickets were chosen, here are the results:\n");
    for(int threadId = 0; threadId < NUM_THREADS; ++threadId){
        printf("    %d: %lf\n",threadId, THREADS[threadId]->result);
    }
}

GuiObjects * init_gui(){

    GtkBuilder  *builder; /* To generate GtkWidgets from XML */
    GError  *error = NULL;
    gchar   *message = NULL;

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load GUI from file. If error occurs, report it and quit application */
    if( ! gtk_builder_add_from_file( builder, GUI_FILE, &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
    }

    /* Allocate data structure */
    gui = g_slice_new( GuiObjects );
    /* Get objects required to change dinamically from the GUI */

    GW( main_window );
    GW( about_window );
    GW( combo_box0 );
    GW( combo_box1 );
    GW( entry_number_threads );
    GW( entry_number_tickets );
    GW( entry_amount_work );
    GW( entry_quantum );
    GW( button0 );
    GW( menu_item_help );
    GW( quantum_label );

    CH_GET_OBJECT_THREAD(builder, bar0, GTK_WIDGET, THREADS, 0, bar);
    CH_GET_OBJECT_THREAD(builder, bar1, GTK_WIDGET, THREADS, 1, bar);
    CH_GET_OBJECT_THREAD(builder, bar2, GTK_WIDGET, THREADS, 2, bar);
    CH_GET_OBJECT_THREAD(builder, bar3, GTK_WIDGET, THREADS, 3, bar);
    CH_GET_OBJECT_THREAD(builder, bar4, GTK_WIDGET, THREADS, 4, bar);
    CH_GET_OBJECT_THREAD(builder, text_box0, GTK_WIDGET, THREADS, 0, text_box);
    CH_GET_OBJECT_THREAD(builder, text_box1, GTK_WIDGET, THREADS, 1, text_box);
    CH_GET_OBJECT_THREAD(builder, text_box2, GTK_WIDGET, THREADS, 2, text_box);
    CH_GET_OBJECT_THREAD(builder, text_box3, GTK_WIDGET, THREADS, 3, text_box);
    CH_GET_OBJECT_THREAD(builder, text_box4, GTK_WIDGET, THREADS, 4, text_box);
    CH_GET_OBJECT_THREAD(builder, spin0, GTK_WIDGET, THREADS, 0, spin);
    CH_GET_OBJECT_THREAD(builder, spin1, GTK_WIDGET, THREADS, 1, spin);
    CH_GET_OBJECT_THREAD(builder, spin2, GTK_WIDGET, THREADS, 2, spin);
    CH_GET_OBJECT_THREAD(builder, spin3, GTK_WIDGET, THREADS, 3, spin);
    CH_GET_OBJECT_THREAD(builder, spin4, GTK_WIDGET, THREADS, 4, spin);

    /* Connect signals and callbacks */
    g_signal_connect (gui->entry_number_tickets, "activate", G_CALLBACK (entry_activate_number_tickets), THREADS[NUM_THREADS]);
    g_signal_connect (gui->entry_amount_work, "activate", G_CALLBACK (entry_activate_amount_work), config);
    g_signal_connect (gui->entry_quantum, "activate", G_CALLBACK (entry_activate_quantum), THREADS[NUM_THREADS]);
    g_signal_connect (gui->combo_box0, "changed", G_CALLBACK (activate_combo_box0), config);
    g_signal_connect (gui->combo_box1, "changed", G_CALLBACK (activate_combo_box1), config);
    g_signal_connect (gui->button0, "clicked", G_CALLBACK (button_clicked), config);
    g_signal_connect (gui->menu_item_help, "activate", G_CALLBACK (create_about_page), config);
    g_signal_connect (gui->main_window, "destroy", G_CALLBACK (gtk_main_quit), config);

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );

    return gui;
}

void alloc_threads () {
    for(int thread = 0; thread < NUM_THREADS; ++thread){
        THREADS[thread] = (Thread*) malloc(sizeof(Thread));
    }
}

int main(int argc, char *argv[]){

    /* Allocate memory for pointers to Thread and thread_configuration structs */
    alloc_threads();
    config = malloc(sizeof(*config));

    /* Init GTK+ */
    gtk_init( &argc, &argv );

    /* Init GUI struct */
    gui = init_gui(gui);

    /*Set the GtkWindow to appear*/
    gtk_widget_show(gui->main_window);

    /* Main Gtk loop */
    gtk_main();

    /* Free any allocated data */
    g_slice_free(GuiObjects, gui);

    return(0);
}
