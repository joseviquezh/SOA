/*Author: Jafet Chaves Barrantes <jafet.a15@gmail.com>*/

#include <gtk/gtk.h>

/* Convenience macros for obtaining objects from GUI file */
#define CH_GET_OBJECT( builder, name, type, data ) \
    data->name = type( gtk_builder_get_object( builder, #name ) )
#define CH_GET_WIDGET( builder, name, data ) \
    CH_GET_OBJECT( builder, name, GTK_WIDGET, data )
#define GW( name ) CH_GET_WIDGET( builder, name, data )

#define GUI_FILE "thread_scheduling.glade"

/* Main data structure definition */
typedef struct _GuiObjects GuiObjects;
struct _GuiObjects
{
    /* Widgets */
    GtkWidget *main_window;  /* Main application window */
    GtkWidget *bar0;
    GtkWidget *text_box0;
    GtkWidget *spin0;
    GtkWidget *bar1;
    GtkWidget *text_box1;
    GtkWidget *spin1;
    GtkWidget *bar2;
    GtkWidget *text_box2;
    GtkWidget *spin2;
    GtkWidget *bar3;
    GtkWidget *text_box3;
    GtkWidget *spin3;
    GtkWidget *bar4;
    GtkWidget *text_box4;
    GtkWidget *spin4;
};

int
main( int    argc,
      char **argv )
{
    GtkBuilder  *builder;
    GuiObjects  *data;
    GError  *error = NULL;
    gchar   *message = NULL;

    /* Init GTK+ */
    gtk_init( &argc, &argv );

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.*/
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

    /*Edit some objects in the GUI*/
    /*Edit the name of the window*/
    gtk_window_set_title (GTK_WINDOW (data->main_window), "SOA-Thread Lottery");
    /*Edit the progress bars*/
    message = g_strdup_printf ("PID: 2019/%.0f%%", 0.75*100);
    gtk_progress_bar_set_text (GTK_PROGRESS_BAR(data->bar0), message);
    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(data->bar0), 0.75 );
    /*Edit the estimate entry boxes*/
    gtk_entry_set_text(GTK_ENTRY(data->text_box0),g_strdup_printf ("%1.16lf",3.1410880271317847));
    /*Edit the status check boxes*/
    gtk_spinner_start(GTK_SPINNER(data->spin0));

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
