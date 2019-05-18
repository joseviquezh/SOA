#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

/* Convenience macros for obtaining objects from GUI file */
#define CH_GET_OBJECT( builder, name, type, data ) \
    data->name = type( gtk_builder_get_object( builder, #name ) )
#define CH_GET_WIDGET( builder, name, data ) \
    CH_GET_OBJECT( builder, name, GTK_WIDGET, data )
#define GW( name ) CH_GET_WIDGET( builder, name, gui )

#define CH_GET_OBJECT_THREAD( builder, name, type, data, index, widget) \
data[index]->widget = type( gtk_builder_get_object( builder, #name ) )

#define GUI_FILE "rt_scheduling.glade"

/* Main data structure definition of the GUI Objects*/
typedef struct _GuiObjects GuiObjects;
struct _GuiObjects
{
    /* Widgets */
    GtkWidget *main_window;  /* Main application window */
    GtkWidget *about_window;  /* Main application window */
    GtkWidget *entry_number_tasks;
    GtkWidget *entry_comp_time;
    GtkWidget *entry_period;
    GtkWidget *combo_box_tasks;
    GtkWidget *combo_box_result;
    GtkWidget *config_file_chooser;
    GtkWidget *check_select_rm;
    GtkWidget *check_select_edf;
    GtkWidget *check_select_llf;
    GtkWidget *check_test_rm;
    GtkWidget *check_test_edf;
    GtkWidget *check_test_llf;
    GtkWidget *execute_button;
    GtkWidget *menu_item_help;
};

G_MODULE_EXPORT void
activate_entry_comp_time (GtkEntry *entry, gpointer user_data);

G_MODULE_EXPORT void
activate_entry_period (GtkEntry *entry, gpointer user_data);

G_MODULE_EXPORT void
activate_combo_box_tasks (GtkComboBox *combo_box);

G_MODULE_EXPORT void
activate_combo_box_result (GtkComboBox *combo_box);

G_MODULE_EXPORT void
activate_config_file_chooser (GtkFileChooserButton *file_chooser_button);

G_MODULE_EXPORT void
toggle_check_select_rm (GtkToggleButton *toggle_button);

G_MODULE_EXPORT void
toggle_check_select_edf (GtkToggleButton *toggle_button);

G_MODULE_EXPORT void
toggle_check_select_llf (GtkToggleButton *toggle_button);

G_MODULE_EXPORT void
execute_button_clicked (GtkButton *button);

G_MODULE_EXPORT void
create_about_page (GtkImageMenuItem *MenuItem);

#endif
