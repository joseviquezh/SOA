// System includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>

#include "task.h"
#include "algorithms/RM.h"
#include "test/test.h"
#include "gui.h"

#define NUM_ALGS 3

struct algorithms_data{
    int comp_time;
    int period;
    int current_selection;
    int result_format; //0: separate slides, 1: one slide
    bool select[NUM_ALGS];
    bool pass;
};

GuiObjects *gui=NULL;
struct algorithms_data* config;

G_MODULE_EXPORT void
activate_entry_comp_time (GtkEntry *entry, gpointer user_data){
    config->comp_time = atoi(gtk_entry_get_text (entry));
    g_print( "Current selected task: %d\n",  config->current_selection);
    g_print( "Computation time: %d\n", config->comp_time);
}

G_MODULE_EXPORT void
activate_entry_period (GtkEntry *entry, gpointer user_data){
    config->period = atoi(gtk_entry_get_text (entry));
    g_print( "Current selected task: %d\n",  config->current_selection);
    g_print( "Computation time: %d\n", config->period);
}

G_MODULE_EXPORT void
activate_combo_box_tasks (GtkComboBox *combo_box){
    config->current_selection = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_box));
    g_print( "Current selected task: %d\n",  config->current_selection);
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

    //TODO: Parse config file and set everything
    g_print( "Selecting config file\n");

}

G_MODULE_EXPORT void
toggle_check_select_rm (GtkToggleButton *toggle_button){

    config->select[0] = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(toggle_button));
    if (!config->select[0]){
        g_print( "Deselected Rate Monotonic\n");
    }
    else {
        //TODO: Parse config file and set everything
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
        //TODO: Parse config file and set everything
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
        //TODO: Parse config file and set everything
        g_print( "Selected Least Laxity First\n");
    }
}

G_MODULE_EXPORT void
execute_button_clicked (GtkButton *button)
{
    //TODO: Add function body to start the algorithms execution according
    //to GUI configuration

    g_print( "Executing Selected Algorithms\n");

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

//----------------------------------------------------
    config = malloc(sizeof(*config));

    /* Init GTK+ */
    gtk_init( &argc, &argv );

    /* Init GUI struct */
    gui = init_gui(gui);

    /*Set the GtkWindow to appear*/
    gtk_widget_show(gui->main_window);

    g_print( "DEBUG\n");

    /* Main Gtk loop */
    gtk_main();

    /* Free any allocated data */
    g_slice_free(GuiObjects, gui);
//----------------------------------------------------

    /*int tasks_length = 6;
    Task * tasks = calloc(tasks_length, sizeof(Task));

    for (int i = 0; i < tasks_length; i++) {
        tasks[i] = (Task) { i + 1, (i + 2) * 2 };;
    }*/

    int tasks_length = 3;
    Task * tasks = calloc(tasks_length, sizeof(Task));

    tasks[0] = (Task) { 1, 1, 2 };
    tasks[1] = (Task) { 2, 1, 3 };
    tasks[2] = (Task) { 3, 1, 6 };


    printf("\n==========\n");
    printf("i | c | p\n----------\n");
    for (int i = 0; i < tasks_length; i++) {
        Task task = tasks[i];
        printf("%i | %i | %i\n", task.id, task.period, task.computation_time);
    }
    printf("==========\n\n\n");

    int test_result = TestRM(tasks, tasks_length);
    printf("test_result = %i\n\n\n", test_result);

    RM_Init(tasks_length, tasks);
    RM_Run();

    return 0;
}
