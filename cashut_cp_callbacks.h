#ifndef CASHUT_CP_CALLBACKS_H
#define CASHUT_CP_CALLBACKS_H

#include <gtk/gtk.h>

// Catalogue Produits
void catalogue_produits_create_liststore(const gchar *critere, const gchar *recherche);

G_MODULE_EXPORT void cp_b_rechercher_clicked(GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void cp_treeview_rowactivated(GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void cp_b_ajouter_clicked(GtkWidget *widget, gpointer user_data);

void dialog_trop_de_resultats();



#endif /* CASHUT_CP_CALLBACKS_H */
