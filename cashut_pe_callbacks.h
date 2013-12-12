#ifndef CASHUT_PE_CALLBACKS_H
#define CASHUT_PE_CALLBACKS_H

#include <gtk/gtk.h>

// Encaissement


// gestion de la tva
float tva_code2taux(int code);

// gestion des produits
void init_treeview_lists_chaine();
G_MODULE_EXPORT void Ajouter_liste(GtkWidget *widget, gpointer   data);
void add_to_treeview_liste_chaine(const gchar *str); 
static void fixed_toggled (GtkCellRendererToggle *cell, gchar *path_str,gpointer data);
G_MODULE_EXPORT void testfonction (GtkWidget *widget, gpointer   data);





#endif /* CASHUT_PE_CALLBACKS_H */
