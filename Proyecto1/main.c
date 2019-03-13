#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include <stdbool.h>
#include "piApproximation.c"
#include "gui.h"

#define NUM_THREADS 5
#define MINIMUN_WORK_UNIT 50

typedef struct{
    int id;
    int mode;               /* 0 expropiatory, 1 Non expropiatory */
    int result;             /* pi calculation result/progress */
    int executed;           /* To know if the thread has been executed yet */
    int workUnits;          /* Amount of work units */
    int workPercentage;     /* Between 0-100% */
    int numTickets;         /* Number of assigned tickets */
    int* tickets;           /* Lottery tickets */
    int quantum;            /* Lottery tickets*/
    sigjmp_buf buffer;      /* Thread buffer */
} Thread;

/* Configuration data retrieved from the GUI */
struct thread_configuration{
    int mode; /* 0 expropiatory, 1 Non expropiatory */
    int threads_number;
    int number_tickets;
    int amount_work;
    int quantum;
};

int NUM_TICKETS = 0;            /* Total ammount of tickets to be assigned */
Thread* THREADS[NUM_THREADS];
Thread* runningThread;
sigjmp_buf parent;
struct thread_configuration config;
GuiObjects  *gui=NULL;

void scheduler();

G_MODULE_EXPORT void
create_about_page (GtkButton *button)
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
    g_print( "Thread scheduling started\n" );
    g_print("Number of tickets: %d\n", config.number_tickets);
    g_print("Amount of work: %d\n", config.amount_work);
    g_print("Quantum: %d\n", config.quantum);
    g_print("Mode: %d\n", config.mode);

    /* Initialize all the threads */
    for(int thread = 0; thread < NUM_THREADS; ++thread){
        THREADS[thread] = (Thread*) malloc(sizeof(Thread));
        THREADS[thread]->id = thread;
        THREADS[thread]->mode = config.mode;
        THREADS[thread]->result = 0;
        THREADS[thread]->executed = 0;
        THREADS[thread]->workPercentage = 0;
        THREADS[thread]->workUnits = config.amount_work*MINIMUN_WORK_UNIT;
        THREADS[thread]->numTickets = config.number_tickets;
        THREADS[thread]->tickets = malloc(config.number_tickets * sizeof(int));
        THREADS[thread]->quantum = config.quantum;
    }

    /*Disable execute button after execution taking effect*/
    gtk_widget_set_sensitive (GTK_WIDGET(button), FALSE);

    // Call the scheduler to start the program
    scheduler();
}

G_MODULE_EXPORT void
entry_activate_number_tickets (GtkEntry *entry)
{
    config.number_tickets = atoi(gtk_entry_get_text (entry));
}

G_MODULE_EXPORT void
entry_activate_amount_work (GtkEntry *entry)
{
    config.amount_work = atoi(gtk_entry_get_text (entry));
}

G_MODULE_EXPORT void
entry_activate_quantum (GtkEntry *entry)
{
    config.quantum = atoi(gtk_entry_get_text (entry));
}

G_MODULE_EXPORT void
activate_combo_box (GtkComboBox *combo_box)
{
    config.mode = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_box));
}

void swap (int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void calculatePi(){
    double progress=0.0;// percent
    long long int i,fractionValue,fractionValueAdjusted,totalWork;
    LookUp* ptrPiAproximationExpro=getInitState();
    LookUp* ptrPiAproximationNOExpro=getInitState();
    

    runningThread->executed = 1;
    if(runningThread->mode == 0){
        /*
            Expropiative: do work during a certaing amount of time

            sigsetjmp -> Saves a checkpoint (stored in runningThread->buffer)
            siglongjmp -> Moves to a checkpoint (stored in parent)
        */
        /*
            // Since the time could end at any point, need to continously save the progress
            while(time){
                // Do calculations
                runningThread->result = result;
                sigsetjmp(runningThread->buffer, 1);

                // Do calculations
                runningThread->result = result;
                sigsetjmp(runningThread->buffer, 1);

                // Do calculations
                */
            if(runningThread->workUnits>0){
                pi_gregory_pauseable(runningThread->workUnits*MIN_OF_WORK,ptrPiAproximationExpro);
                //The value of pi is saved in ptrPiAproximationExpro->piSoFar
                progress=ptrPiAproximationExpro->iterations*100/(runningThread->workUnits*MIN_OF_WORK);
            }else{
                printf("\nIncorrect parameters for: workunits: %d\n",runningThread->workUnits);
            }
               /*
            }
            runningThread->result = result;     // Save result in the object
            siglongjmp(parent, 1);      // Move back to the scheduler
        */
    }
    else{
        /*
            Non-expropiative: do a specific work percentage
        */
        if(validateParaetersNoExpropiatives(runningThread->workUnits,runningThread->workPercentage)){
            fractionValue=totalWork*runningThread->workPercentage/100;
            fractionValueAdjusted=fractionValue;
            i=(long long int)totalWork/fractionValue;
            printf("Total work %lld processing %lld per %lld iteration \n",totalWork,fractionValue,i);
            while(i-->=0&&fractionValueAdjusted>0){
                pi_gregory_pauseable(fractionValueAdjusted,ptrPiAproximationNOExpro);
                //TO PROGRESS PERCENTAGE UNTIL NOW     ->     i+1>0?(long long int)((totalWork/fractionValue-i)*runningThread->workPercentage):100
                //VALUE OF PI UNTIL NOW                ->     ptrPiAproximationNOExpro->piSoFar
                if(fractionValueAdjusted>totalWork-ptrPiAproximationNOExpro->iterations)
                fractionValueAdjusted=totalWork-ptrPiAproximationNOExpro->iterations;
                printf("This line has be replaced with the command required to release the thread.\n");
            }
            // TO CONSULT FINAL VULE OF PI             ->     ptrPiAproximationNOExpro->piSoFar
            // TO CONSULT THE TOTAL ITERATIONS DONE    ->     ptrPiAproximationNOExpro->iterations
        }else{
            printf("\nIncorrect parameters for: percentage: %d or workunits: %d\n",runningThread->workPercentage,runningThread->workUnits);
        }
        //The final value of pi is saved in ptrPiAproximationExpro->piSoFar
        /*
            sigsetjmp(runningThread->buffer, 1);    // Save chackpoint so we can return later
            while(runningThread->workPercentage > completedWorkPercentage){
                // Do calculations
            }
            runningThread->result = result;     // Save result in the object
            siglongjmp(parent, 1);      // Move back to the scheduler
        */
    }

    for(int i = 0; i < 10; ++i){
        if(sigsetjmp(runningThread->buffer, 1) == 0) siglongjmp(parent, 1);
    }
}

void scheduler(){
    printf("Entered scheduler\n");
    // Create an array with all the ticket numbers
    int tickets[NUM_TICKETS];
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
        printf("Selecting a wining ticket\n");
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
        printf("The winning ticket is %d and the winner is %d\n", ticket, threadId);
        runningThread = THREADS[threadId];
        if(runningThread->executed){
            if(sigsetjmp(parent, 1) == 0) siglongjmp(THREADS[threadId]->buffer, threadId);
        }
        else{
            if(sigsetjmp(parent, 1) == 0) calculatePi();
        }
    }
}

GuiObjects * init_gui(GuiObjects  *gui){

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
    GW( bar0 );
    GW( text_box0 );
    GW( spin0 );
    GW( bar1 );
    GW( text_box1 );
    GW( spin1 );
    GW( bar2 );
    GW( text_box2 );
    GW( spin2 );
    GW( bar3 );
    GW( text_box3 );
    GW( spin3 );
    GW( bar4 );
    GW( text_box4 );
    GW( spin4 );
    GW( combo_box0 );
    GW( entry_number_threads );
    GW( entry_number_tickets );
    GW( entry_amount_work );
    GW( entry_quantum );

    /* Connect signals */
    gtk_builder_connect_signals( builder, gui->main_window );

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );

    return gui;
}

int main(int argc, char *argv[]){

    /* Init GTK+ */
    gtk_init( &argc, &argv );

    /* Init GUI struct */
    gui = init_gui(gui);

    /*Set the GtkWindow to appear*/
    gtk_widget_show(gui->main_window);

    /* Get the configuration from the GUI */

    /* Main Gtk loop */
    gtk_main();

    /* Free allocated memory */
    for(int thread = 0; thread < NUM_THREADS; ++thread){
        free(THREADS[thread]->tickets);
    }

    /* Free any allocated data */
    g_slice_free(GuiObjects, gui);

    return(0);
}
