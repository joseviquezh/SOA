/*Author: Jafet Chaves Barrantes <jafet.a15@gmail.com>*/

#include "main_gui.h"

G_MODULE_EXPORT void
button_clicked (GtkButton *button, GuiObjects  *data)
{
    g_print( "Thread scheduling started\n" );
    /* TODO: Add logic to start thread scheduling */
}

G_MODULE_EXPORT void
create_about_page (GtkButton *button, GuiObjects  *data)
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

int
main( int    argc,
      char **argv )
{
    GtkBuilder  *builder;
    GuiObjects  *data;
    GError  *error = NULL;
    gchar   *message = NULL;
    gint combo_box_index;
    const gchar *threads_number;

    /* Init GTK+ */
    gtk_init( &argc, &argv );

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load GUI from file. If error occurs, report it and quit application.*/
    if( ! gtk_builder_add_from_file( builder, GUI_FILE, &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
        return( 1 );
    }

    /* Allocate data structure */
    data = g_slice_new( GuiObjects );

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

    /* Edit some objects in the GUI */
    /* Edit the name of the main window */
    gtk_window_set_title (GTK_WINDOW (data->main_window), "SOA-Thread Lottery");
    /* Edit the progress bars */
    message = g_strdup_printf ("PID: 2019/%.0f%%", 0.75*100);
    gtk_progress_bar_set_text (GTK_PROGRESS_BAR(data->bar0), message);
    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(data->bar0), 0.75 );
    /* Edit the estimate entry boxes */
    gtk_entry_set_text(GTK_ENTRY(data->text_box0),g_strdup_printf ("%1.16lf",3.1410880271317847));
    /* Edit the status check boxes */
    gtk_spinner_start(GTK_SPINNER(data->spin0));

    /* Get the active mode of the combo box widget */
    combo_box_index = gtk_combo_box_get_active (GTK_COMBO_BOX(data->combo_box0));
    g_print( "combo_box_index: %d\n", combo_box_index );

    /* Get the text in the entry */
    threads_number = gtk_entry_get_text (GTK_ENTRY(data->entry_number_threads));
    g_print( "threads_number: %s\n", threads_number );

    /* Connect signals */
    gtk_builder_connect_signals( builder, data->main_window );

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show( data->main_window );

    /* Start main loop */
    gtk_main();

    /* Free any allocated data */
    g_slice_free( GuiObjects, data );

    return( 0 );
}
