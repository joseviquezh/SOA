// System includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>

// Custom includes
#include "task.h"
#include "queue-item.h"
#include "test/test.h"
#include "scheduler/scheduler.h"
#include "gui.h"
#include "writer.h"

#define NUM_ALGS 3
#define NUM_TASKS 6

static const int MAX_DISPLAY_UNITS=40;


struct algorithms_conf{
    gint task_number;
    int current_selection;
    int result_format; //0: separate slides, 1: one slide
    bool select[NUM_ALGS];
    bool pass;
    gchar *config_file;

};

GuiObjects *gui=NULL;
struct algorithms_conf* config;
Task * tasks;

/*Wrapper: Executes particular algorithm, returns history of execution*/
QueueItem * execute_algorithm(int algorithm, int * size, int *mcm) {
    InitScheduler(config->task_number, tasks, algorithm);
    RunScheduling();
    *size = GetHistorySize ();
    *mcm = GetMcm ();

    return GetHistory ();
}

void set_algorithm_select(GtkWidget * wigdet, int value){

    if(value == false){
        config->select[0] = false;
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wigdet), 0);
    }
    else if (value == 1){
        config->select[0] = true;
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wigdet), true);
    }
    /*Set check boxes sensitivity to false (not able to interact after)*/
    gtk_widget_set_sensitive (GTK_WIDGET(wigdet), false);
}

bool load_config_file(char* file)
{
    FILE *fin = fopen(file, "r");
    int i,j;
    if (fin!=NULL){
        int value;
        fscanf(fin,"ResultFormat=%d\n", &value);
        config->result_format = value;
        if(value == 0){
            gtk_combo_box_set_active (GTK_COMBO_BOX (gui->combo_box_result), 0);
        }
        else if (value == 1){
            gtk_combo_box_set_active (GTK_COMBO_BOX (gui->combo_box_result), 1);
        }

        fscanf(fin,"RM=%d\n", &value);
        set_algorithm_select(gui->check_select_rm, value);
        config->select[0] = value;

        fscanf(fin,"EDF=%d\n", &value);
        set_algorithm_select(gui->check_select_edf, value);
        config->select[1] = value;

        fscanf(fin,"LLF=%d\n", &value);
        set_algorithm_select(gui->check_select_llf, value);
        config->select[2] = value;

        fscanf(fin,"TaskNumber=%d\n", &value);
        config->task_number = value;

        for(j=0; j<config->task_number; j++)
        {
            fscanf(fin,"{ComputationTime=%d, Period=%d}\n", &tasks[j].computation_time, &tasks[j].period);
        }

        fclose(fin);
        sleep(3);
        return true;
    }
    fclose(fin);
    return false;
}

G_MODULE_EXPORT void
activate_entry_number_tasks (GtkEntry *entry, gpointer user_data){
    config->task_number = atoi(gtk_entry_get_text (entry));
    g_print( "Number of tasks: %d\n", config->task_number);
}

G_MODULE_EXPORT void
activate_entry_comp_time (GtkEntry *entry, gpointer user_data){
    tasks[config->current_selection].computation_time = atoi(gtk_entry_get_text (entry));
    g_print( "Computation time: %d\n", tasks[config->current_selection].computation_time);
}

G_MODULE_EXPORT void
activate_entry_period (GtkEntry *entry, gpointer user_data){
    tasks[config->current_selection].period = atoi(gtk_entry_get_text (entry));
    g_print( "Computation time: %d\n", tasks[config->current_selection].period);
}

G_MODULE_EXPORT void
activate_combo_box_tasks (GtkComboBox *combo_box){
    config->current_selection = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_box));
    g_print( "Current selection: %d\n", config->current_selection);
}

G_MODULE_EXPORT void
activate_combo_box_result (GtkComboBox *combo_box){
    config->result_format = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_box));
    if(config->result_format == 1){
        g_print( "Current selected result format: Everything in one slide\n");
    }
    else {
        g_print( "Current selected result form: Independent slides\n");
    }
}

G_MODULE_EXPORT void
activate_config_file_chooser (GtkFileChooserButton *file_chooser_button){

    config->config_file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(file_chooser_button));
    if(load_config_file(config->config_file))
    {
        g_print("File has been loaded: %s \n", config->config_file);
    }
    else
    {
        g_print("Archivo NO cargado: %s \n", config->config_file);
}
}

G_MODULE_EXPORT void
toggle_check_select_rm (GtkToggleButton *toggle_button){

    config->select[0] = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(toggle_button));
    if (config->select[0] == FALSE){
        g_print( "Deselected Rate Monotonic\n");
    }
    else {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(toggle_button), 1);
        g_print( "Selected Rate Monotonic\n");
    }
}

G_MODULE_EXPORT void
toggle_check_select_edf (GtkToggleButton *toggle_button){

    config->select[1] = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(toggle_button));
    if (!config->select[1]){
        g_print( "Deselected Earliest Deadline First\n");
    }
    else {
        g_print( "Selected Earliest Deadline First\n");
    }
}

G_MODULE_EXPORT void
toggle_check_select_llf (GtkToggleButton *toggle_button){

    config->select[2] = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(toggle_button));
    if (!config->select[2]){
        g_print( "Deselected Least Laxity First\n");
    }
    else {
        g_print( "Selected Least Laxity First\n");
    }
}

G_MODULE_EXPORT void
execute_button_clicked (GtkButton *button)
{
    BeamerPresentation* presentation=beamerBuilder();
    char * new_str;
    char str[100];
    //char task_list[6][8]={"Task 1","Task 2","Task 3","Task 4","Task 5","Task 6"};
    int n = config->task_number;

    g_print( "\nExecuting Selected Algorithms\n");

    printf("\n==========\n");
    printf("i | c | p\n----------\n");
    for (int i = 0; i < n; i++) {
        Task task = tasks[i];
        printf("%i | %i | %i\n", task.id, task.computation_time, task.period);
    }
    printf("==========\n\n\n");

    for(int j=0; j<n; j++)
    {
        tasks[j].id = j+1;
    }

    gboolean execute_rm = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui->check_select_rm));
    gboolean execute_edf = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui->check_select_edf));
    gboolean execute_llf = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui->check_select_llf));

    if (config->result_format == 0){
        if (execute_rm == TRUE){
            int size_result_rm;
            int mcm;
            int test_result = TestRM(tasks, n);
            printf("test_result = %i\n\n\n", test_result);
            if (test_result == 1){
                gtk_label_set_text (GTK_LABEL(gui->check_test_rm),"PASSED");
            }
            else{
                gtk_label_set_text (GTK_LABEL(gui->check_test_rm),"FAILED");
            }
            QueueItem * result_rm = execute_algorithm(RM, &size_result_rm, &mcm);

            insertNewAlgorithm(presentation,"Results RM", MAX_DISPLAY_UNITS);

            for (int i = 0; i < n; i++) {
                sprintf(str,"Task %d",i);
                insertNewTask(presentation,"Results RM", str);
            }

            for (int i = 0; i < size_result_rm; i++) {
                printf("\n============= Time elapsed %i ==============\n", i);
                if (!result_rm[i].null) {
                    printf("Running task: %i in elapsed_time = %i\n", result_rm[i].task.id, i+1);
                    int index;
                    index = result_rm[i].task.id-1;
                    printf("index: %d\n", index);
                    sprintf(str,"Task %d",index);
                    printf("\n-------------------------------------------------------Results RM T %d\n",index);
                    insertNewRange(presentation,"Results RM", str, i+1, i+1);
                }
                else {
                    printf("Free period in elapsed_time = %i\n", i+1);
                }
                printf("===========================================\n\n\n");
            }
        }

        if (execute_edf == TRUE){
            int size_result_edf;
            int mcm;
            int test_result = TestEDF(tasks, n);
            printf("test_result = %i\n\n\n", test_result);
            if (test_result == 1){
                gtk_label_set_text (GTK_LABEL(gui->check_test_edf),"PASSED");
            }
            else{
                gtk_label_set_text (GTK_LABEL(gui->check_test_edf),"FAILED");
            }
            QueueItem * result_edf = execute_algorithm(EDF, &size_result_edf, &mcm);

            insertNewAlgorithm(presentation,"Results EDF",MAX_DISPLAY_UNITS);

            for (int i = 0; i < n; i++) {
                sprintf(str,"Task %d",i);
                insertNewTask(presentation,"Results EDF", str);
            }

            for (int i = 0; i < size_result_edf; i++) {
                printf("\n============= Time elapsed %i ==============\n", i);
                if (!result_edf[i].null) {
                    printf("Running task: %i in elapsed_time = %i\n", result_edf[i].task.id, i+1);
                    int index;
                    index = result_edf[i].task.id-1;
                    printf("index: %d\n", index);
                    sprintf(str,"Task %d",index);
                    printf("\n-------------------------------------------------------Results EDF T %d\n",index);
                    insertNewRange(presentation,"Results EDF", str, i+1, i+1);
                }
                else {
                    printf("Free period in elapsed_time = %i\n", i+1);
                }
                printf("===========================================\n\n\n");
            }
        }

        if (execute_llf == TRUE){
            int size_result_llf;
            int mcm;
            int test_result = TestLLF(tasks, n);
            printf("test_result = %i\n\n\n", test_result);
            if (test_result == 1){
                gtk_label_set_text (GTK_LABEL(gui->check_test_llf),"PASSED");
            }
            else{
                gtk_label_set_text (GTK_LABEL(gui->check_test_llf),"FAILED");
            }
            QueueItem * result_llf = execute_algorithm(LLF, &size_result_llf, &mcm);

            insertNewAlgorithm(presentation,"Results LLF",MAX_DISPLAY_UNITS);

            for (int i = 0; i < n; i++) {
                sprintf(str,"Task %d",i);
                insertNewTask(presentation,"Results LLF", str);
            }

            for (int i = 0; i < size_result_llf; i++) {
                printf("\n============= Time elapsed %i ==============\n", i);
                if (!result_llf[i].null) {
                    printf("Running task: %i in elapsed_time = %i\n", result_llf[i].task.id, i+1);
                    int index;
                    index = result_llf[i].task.id-1;
                    printf("index: %d\n", index);
                    sprintf(str,"Task %d",index);
                    printf("\n-------------------------------------------------------Results LLF T %d\n",index);
                    insertNewRange(presentation,"Results LLF", str, i+1, i+1);
                }
                else {
                    printf("Free period in elapsed_time = %i\n", i+1);
                }
                printf("===========================================\n\n\n");
            }
        }

        generateLatexForBeamer(presentation);
        system("pdflatex main.tex");
        system("evince main.pdf");
        //free(presentation);
    }

    else {
        int mcm;
        ///////////////////////////////////////////////////////
        printf("\n-------------------------------------------------------All in one\n");
        if (execute_rm == TRUE){
            int size_result_rm;
            int test_result = TestRM(tasks, n);
            printf("test_result = %i\n\n\n", test_result);
            if (test_result == 1){
                gtk_label_set_text (GTK_LABEL(gui->check_test_rm),"PASSED");
            }
            else{
                gtk_label_set_text (GTK_LABEL(gui->check_test_rm),"FAILED");
            }
            QueueItem * result_rm = execute_algorithm(RM, &size_result_rm,&mcm);

        printf("\n-------------------------------------------------------All algorithms\n");
            insertNewAlgorithm(presentation,"All algorithms",MAX_DISPLAY_UNITS);

            for (int i = 0; i < n; i++) {
                sprintf(str,"RM T %d",i);
                insertNewTask(presentation,"All algorithms", str);
            }
            
            for (int i = 0; i < size_result_rm; i++) {
                printf("\n============= Time elapsed %i ==============\n", i);
                if (!result_rm[i].null) {
                    printf("Running task: %i in elapsed_time = %i\n", result_rm[i].task.id, i+1);
                    int index;
                    index = result_rm[i].task.id-1;
                    printf("index: %d\n", index);
                    sprintf(str,"RM T %d",index);

                    printf("\n-------------------------------------------------------RM T %d\n",index);
                    insertNewRange(presentation,"All algorithms", str, i+1, i+1);
                }
                else {
                    printf("Free period in elapsed_time = %i\n", i+1);
                }
                printf("===========================================\n\n\n");
            }
        }

        if (execute_edf == TRUE){
            int size_result_edf;
            int test_result = TestEDF(tasks, n);
            printf("test_result = %i\n\n\n", test_result);
            if (test_result == 1){
                gtk_label_set_text (GTK_LABEL(gui->check_test_edf),"PASSED");
            }
            else{
                gtk_label_set_text (GTK_LABEL(gui->check_test_edf),"FAILED");
            }
            QueueItem * result_edf = execute_algorithm(EDF, &size_result_edf,&mcm);

            //insertNewAlgorithm(presentation,"Results EDF",MAX_DISPLAY_UNITS);

            for (int i = 0; i < n; i++) {
                sprintf(str,"EDF T %d",i);
                insertNewTask(presentation,"All algorithms", str);
            }

            for (int i = 0; i < size_result_edf; i++) {
                printf("\n============= Time elapsed %i ==============\n", i);
                if (!result_edf[i].null) {
                    printf("Running task: %i in elapsed_time = %i\n", result_edf[i].task.id, i+1);
                    int index;
                    index = result_edf[i].task.id-1;
                    printf("index: %d\n", index);
                    sprintf(str,"EDF T %d",index);
                    printf("\n-------------------------------------------------------EDF T %d\n",index);
                    insertNewRange(presentation,"All algorithms", str, i+1, i+1);
                }
                else {
                    printf("Free period in elapsed_time = %i\n", i+1);
                }
                printf("===========================================\n\n\n");
            }
        }

        if (execute_llf == TRUE){
            int size_result_llf;
            int test_result = TestLLF(tasks, n);
            printf("test_result = %i\n\n\n", test_result);
            if (test_result == 1){
                gtk_label_set_text (GTK_LABEL(gui->check_test_llf),"PASSED");
            }
            else{
                gtk_label_set_text (GTK_LABEL(gui->check_test_llf),"FAILED");
            }
            QueueItem * result_llf = execute_algorithm(LLF, &size_result_llf,&mcm);

            //insertNewAlgorithm(presentation,"Results LLF",MAX_DISPLAY_UNITS);

            for (int i = 0; i < n; i++) {
                sprintf(str,"LLF T %d",i);
                insertNewTask(presentation,"All algorithms", str);
            }

            for (int i = 0; i < size_result_llf; i++) {
                printf("\n============= Time elapsed %i ==============\n", i);
                if (!result_llf[i].null) {
                    printf("Running task: %i in elapsed_time = %i\n", result_llf[i].task.id, i+1);
                    int index;
                    index = result_llf[i].task.id-1;
                    printf("index: %d\n", index);
                    sprintf(str,"LLF T %d",index);
                    printf("\n-------------------------------------------------------LLF T %d\n ",index);
                    insertNewRange(presentation,"All algorithms", str, i+1, i+1);
                }
                else {
                    printf("Free period in elapsed_time = %i\n", i+1);
                }
                printf("===========================================\n\n\n");
            }
        }

        generateLatexForBeamer(presentation);
        system("pdflatex main.tex");
        system("evince main.pdf");
        //free(presentation);
        ////////////////////////////////////////////////////////////////////////



    }
}

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
                            "Proyecto III: Scheduling en Tiempo Real (v0.1)\n\n"\
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

GuiObjects * init_gui(){

    GtkBuilder  *builder; /* To generate GtkWidgets from XML */
    GError  *error = NULL;

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
    GW( entry_number_tasks );
    GW( entry_comp_time );
    GW( entry_period );
    GW( combo_box_tasks );
    GW( combo_box_result );
    GW( config_file_chooser );
    GW( check_select_rm );
    GW( check_select_edf );
    GW( check_select_llf );
    GW( check_test_rm );
    GW( check_test_edf );
    GW( check_test_llf );
    GW( execute_button );
    GW( menu_item_help );

    /* Connect signals and callbacks */
    g_signal_connect (gui->entry_number_tasks, "activate", G_CALLBACK (activate_entry_number_tasks), config);
    g_signal_connect (gui->entry_comp_time, "activate", G_CALLBACK (activate_entry_comp_time), config);
    g_signal_connect (gui->entry_period, "activate", G_CALLBACK (activate_entry_period), config);
    g_signal_connect (gui->combo_box_tasks, "changed", G_CALLBACK (activate_combo_box_tasks), config);
    g_signal_connect (gui->combo_box_result, "changed", G_CALLBACK (activate_combo_box_result), config);
    g_signal_connect (gui->config_file_chooser, "file-set", G_CALLBACK (activate_config_file_chooser), config);
    g_signal_connect (gui->check_select_rm, "toggled", G_CALLBACK (toggle_check_select_rm), config);
    g_signal_connect (gui->check_select_edf, "toggled", G_CALLBACK (toggle_check_select_edf), config);
    g_signal_connect (gui->check_select_llf, "toggled", G_CALLBACK (toggle_check_select_llf), config);
    g_signal_connect (gui->execute_button, "clicked", G_CALLBACK (execute_button_clicked), config);
    g_signal_connect (gui->menu_item_help, "activate", G_CALLBACK (create_about_page), config);
    g_signal_connect (gui->main_window, "destroy", G_CALLBACK (gtk_main_quit), config);

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );

    return gui;
}

// Main
int main(int argc, char *argv[])
{

    tasks = calloc(NUM_TASKS, sizeof(Task));
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
    free(config);
    free(tasks);

    return 0;
}
