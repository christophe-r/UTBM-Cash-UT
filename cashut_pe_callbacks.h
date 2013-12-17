#ifndef CASHUT_PE_CALLBACKS_H
#define CASHUT_PE_CALLBACKS_H

#include <gtk/gtk.h>

// Encaissement


// gestion de la tva
float tva_code2taux(int code);

// gestion des produits
void init_treeview_lists_chaine();
void maj_treeview_liste_chaine(); 
void maj_tb_taux_tva();
void maj_footer_total();
void maj_facture();


// action de l'UI
G_MODULE_EXPORT void pe_ajouter_produit(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void pe_annuler(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void pe_supprimer_produit(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void pe_verif_caractere(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void pe_btn_cheque(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void pe_btn_espece(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void pe_btn_carte(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void pe_annuler_paiement(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void testfonction (GtkWidget *widget, gpointer   data);





#endif /* CASHUT_PE_CALLBACKS_H */
