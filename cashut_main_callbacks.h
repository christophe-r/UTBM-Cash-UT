#ifndef CASHUT_MAIN_CALLBACKS_H
#define CASHUT_MAIN_CALLBACKS_H

#include <gtk/gtk.h>


G_MODULE_EXPORT void on_window_cashut_destroy (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void on_window_cashut_show (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void on_notebook_change_page (GtkWidget *widget, gpointer user_data);


// Gestion des utilisateurs
G_MODULE_EXPORT void on_notebook_change_page6(void);
void liste_utilisateurs_create_liststore(void);
G_MODULE_EXPORT void gu_treeview_rowactivated(GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void gu_button_modifier_clicked (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void gu_button_supprimer_clicked (GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void gu_button_ajouter_clicked (GtkWidget *widget, gpointer user_data);
void gu_reinitialiser_form_modifier(void);
void gu_activer_form_modifier(void);
void gu_reinitialiser_form_supprimer(void);
void gu_activer_form_supprimer(void);
void gu_reinitialiser_form_ajouter(void);
void gu_dialog_champs_incorrects(void);

// Encaissement
float tva_code2taux(int code);


// gestion des produits
void Ajouter_liste (GtkWidget *widget, gpointer   data);
void init_list(GtkWidget *list);
void add_to_list(GtkWidget *list, const gchar *str); 



#endif /* CASHUT_MAIN_CALLBACKS_H */
