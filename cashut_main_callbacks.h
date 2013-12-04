#ifndef CASHUT_MAIN_CALLBACKS_H
#define CASHUT_MAIN_CALLBACKS_H

#include <gtk/gtk.h>


G_MODULE_EXPORT void on_window_cashut_destroy (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void on_window_cashut_show (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void on_notebook_change_page (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void on_notebook_change_page6(void);

float tva_code2taux(int code);

G_MODULE_EXPORT void liste_utilisateurs_create_liststore(void);


#endif /* CASHUT_MAIN_CALLBACKS_H */
