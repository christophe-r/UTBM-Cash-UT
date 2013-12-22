#include <my_global.h>
#include <mysql.h>

#include <string.h>

#include <glib.h>

#include "cashut_st_callbacks.h"
#include "main_mysql.h"

extern GtkBuilder 	*builder_cashut;

extern const gchar *nom_utilisateur;
extern const gchar *num_utilisateur;

// Accueil
void on_notebook_change_page0(){
	st_afficher_stats_ac();
}


void st_afficher_stats_ac()
{
	int caisse_nombre_vente;
	float caisse_somme_total;
	float caisse_somme_montant_moyen;

	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "ac_label_nom_utilisateur")), g_strconcat("Utilisateur : ", nom_utilisateur, NULL));

	caisse_nombre_vente = mysql_nombre_ventes_aujourdhui(num_utilisateur);
	char ac_caisse_monbre_vente[10];
	snprintf(ac_caisse_monbre_vente,10,"%d",caisse_nombre_vente);	
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "ac_label_total_vente")), ac_caisse_monbre_vente);
	
	caisse_somme_total = mysql_total_encaisse_aujourdhui(num_utilisateur);
	char ac_caisse_somme_total[10];
	snprintf(ac_caisse_somme_total,10,"%.2f",caisse_somme_total);
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "ac_label_somme_total")), g_strconcat(ac_caisse_somme_total," €",NULL));


	if( caisse_nombre_vente > 0 ){
		caisse_somme_montant_moyen = caisse_somme_total / caisse_nombre_vente;
		char ac_caisse_somme_montant_moyen[10];
		snprintf(ac_caisse_somme_montant_moyen,10,"%.2f",caisse_somme_montant_moyen);
		gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "ac_label_montant_moyen")), g_strconcat(ac_caisse_somme_montant_moyen," €",NULL));
	} else {
		gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "ac_label_montant_moyen")), "0.00 €");
	}
}


// Statistiques
void on_notebook_change_page4(){
	st_afficher_stats_stats();
}

void st_afficher_stats_stats()
{
	int magasin_nombre_vente;
	int nombre_produit;
	float magasin_somme_totale;
	float magasin_montant_moyen;

	magasin_nombre_vente = mysql_nombre_ventes_aujourdhui("0");
	char st_magasin_monbre_vente[10];
	snprintf(st_magasin_monbre_vente,10,"%d",magasin_nombre_vente);		
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_total_vente")), st_magasin_monbre_vente);

	magasin_somme_totale = mysql_total_encaisse_aujourdhui("0");
	char st_magasin_somme_totale[10];
	snprintf(st_magasin_somme_totale,10,"%.2f",magasin_somme_totale);
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_somme_total")), g_strconcat(st_magasin_somme_totale," €",NULL));


	if( magasin_nombre_vente > 0 ){
		magasin_montant_moyen = magasin_somme_totale / magasin_nombre_vente;
		char st_magasin_montant_moyen[10];
		snprintf(st_magasin_montant_moyen,10,"%.2f",magasin_montant_moyen);
		gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_montant_moyen")), g_strconcat(st_magasin_montant_moyen," €",NULL));
	} else {
		gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_montant_moyen")), "0.00 €");
	}

	nombre_produit = mysql_nombre_produits();
	char st_magasin_nombre_produit[10];
	snprintf(st_magasin_nombre_produit,10,"%d",nombre_produit);	
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_nb_produit")), st_magasin_nombre_produit);
	
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_dernier_encaissement")), mysql_derniere_vente());
	
	}


