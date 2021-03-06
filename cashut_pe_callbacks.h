#ifndef CASHUT_PE_CALLBACKS_H
#define CASHUT_PE_CALLBACKS_H

#include <gtk/gtk.h>

// Encaissement

void init_treeview_lists_chaine();

void maj_treeview_liste_chaine(); 
void maj_tb_taux_tva();
void maj_footer_total();
void maj_facture();
void gen_facture(int with_fact);
char *align(char *text, int taille);
char *padding_char(char *text, int taille, int type);
void maj_modif_liste_produit(short int maj_treeview);

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
G_MODULE_EXPORT void pe_facture_no_fact(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void pe_facture_fact(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void pe_qte_produit_retire(GtkWidget *widget, gpointer   data);
G_MODULE_EXPORT void pe_qte_produit_ajout(GtkWidget *widget, gpointer   data);





#endif /* CASHUT_PE_CALLBACKS_H */
