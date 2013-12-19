#ifndef CASHUT_GP_CALLBACKS_H
#define CASHUT_GP_CALLBACKS_H

#include <gtk/gtk.h>

// Gestion des produits

void gp_button_valider_click (GtkWidget *widget, gpointer user_data);
void gp_afficher_champs();
void gp_cacher_vider_champs();
void gp_button_annuler_click();
void gp_button_modif_click();
void gp_button_ajout_click();
void gp_button_sup_click();
int gp_ajouter_produit();
void gp_dialog_button_annuler();
int gp_modifier_produit();
G_MODULE_EXPORT void gp_verif_entiers(GtkWidget *widget, gpointer user_data);
G_MODULE_EXPORT void gp_verif_floats(GtkWidget *widget, gpointer user_data);
void gp_dialog_erreur_db();
void gp_dialog_erreur_champs();


#endif /* CASHUT_GP_CALLBACKS_H */
