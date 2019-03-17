#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

/* Convenience macros for obtaining objects from GUI file */
#define CH_GET_OBJECT( builder, name, type, data ) \
    data->name = type( gtk_builder_get_object( builder, #name ) )
#define CH_GET_WIDGET( builder, name, data ) \
    CH_GET_OBJECT( builder, name, GTK_WIDGET, data )
#define GW( name ) CH_GET_WIDGET( builder, name, gui )

#define GUI_FILE "thread_scheduling.glade"

/* Main data structure definition of the GUI Objects*/
typedef struct _GuiObjects GuiObjects;
struct _GuiObjects
{
    /* Widgets */
    GtkWidget *main_window;  /* Main application window */
    GtkWidget *about_window;  /* Main application window */
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
    GtkWidget *combo_box0;
    GtkWidget *combo_box1;
    GtkWidget *entry_number_threads;
    GtkWidget *entry_number_tickets;
    GtkWidget *entry_amount_work;
    GtkWidget *entry_quantum;
    GtkWidget *button0;
    GtkWidget *menu_item_help;
};

G_MODULE_EXPORT void
create_about_page (GtkImageMenuItem *MenuItem);

G_MODULE_EXPORT void
button_clicked (GtkButton *button);

G_MODULE_EXPORT void
entry_activate_number_tickets (GtkEntry *entry, gpointer user_data);

G_MODULE_EXPORT void
entry_activate_amount_work (GtkEntry *entry, gpointer user_data);

G_MODULE_EXPORT void
entry_activate_quantum (GtkEntry *entry, gpointer user_data);

G_MODULE_EXPORT void
activate_combo_box0 (GtkComboBox *combo_box);

G_MODULE_EXPORT void
activate_combo_box1 (GtkComboBox *combo_box);

#endif
