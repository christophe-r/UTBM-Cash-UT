#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>


G_MODULE_EXPORT void on_window_destroy (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void on_button_connect_clicked (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void dialog_mysql_error();
G_MODULE_EXPORT void dialog_mauvais_identifiants();
G_MODULE_EXPORT void dialog_erreur_config_file();


#endif /* CALLBACKS_H */
