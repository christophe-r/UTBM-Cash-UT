#ifndef CASHUT_MAIN_CALLBACKS_H
#define CASHUT_MAIN_CALLBACKS_H

#include <gtk/gtk.h>


G_MODULE_EXPORT void on_window_cashut_destroy (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void on_window_cashut_show (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void on_notebook_change_page (GtkWidget *widget, gpointer user_data);


// Encaissement
float tva_code2taux(int code);
void Ajouter_liste (GtkWidget *widget, gpointer   data);


// gestion des produits
void init_treeview_lists(GtkWidget *list);
void Ajouter_liste (GtkWidget *widget, gpointer   data);
void add_to_list(GtkWidget *list, const gchar *str); 



#endif /* CASHUT_MAIN_CALLBACKS_H */
