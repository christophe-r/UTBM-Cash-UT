#ifndef CASHUT_GP_CALLBACKS_H
#define CASHUT_GP_CALLBACKS_H

#include <gtk/gtk.h>

// Gestion des produits


// Encaissement
float tva_code2taux(int code);

// gestion des produits
void init_treeview_lists(GtkWidget *list);
G_MODULE_EXPORT void Ajouter_liste(GtkWidget *widget, gpointer   data);
void add_to_list(GtkWidget *list, const gchar *str); 






#endif /* CASHUT_GP_CALLBACKS_H */
