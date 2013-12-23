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
	int caisse_nombre_ventes;
	float caisse_somme_totale;
	float caisse_somme_montant_moyen;

	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "ac_label_nom_utilisateur")), g_strconcat("Utilisateur : ", nom_utilisateur, NULL));

	caisse_nombre_ventes = mysql_nombre_ventes_aujourdhui(num_utilisateur);
	char ac_caisse_monbre_vente[10];
	snprintf(ac_caisse_monbre_vente, 10, "%d", caisse_nombre_ventes);	
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "ac_label_total_ventes")), ac_caisse_monbre_vente);
	
	caisse_somme_totale = mysql_total_encaisse_aujourdhui(num_utilisateur);
	char ac_caisse_somme_totale[10];
	snprintf(ac_caisse_somme_totale, 10, "%.2f", caisse_somme_totale);
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "ac_label_somme_totale")), g_strconcat(ac_caisse_somme_totale, " €", NULL));


	if( caisse_nombre_ventes > 0 ){
		caisse_somme_montant_moyen = caisse_somme_totale / caisse_nombre_ventes;
		char ac_caisse_somme_montant_moyen[10];
		snprintf(ac_caisse_somme_montant_moyen, 10, "%.2f", caisse_somme_montant_moyen);
		gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "ac_label_montant_moyen")), g_strconcat(ac_caisse_somme_montant_moyen, " €", NULL));
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
	int magasin_nombre_ventes;
	int nombre_produits;
	float magasin_somme_totale;
	float magasin_montant_moyen;

	magasin_nombre_ventes = mysql_nombre_ventes_aujourdhui("0");
	char st_magasin_monbre_vente[10];
	snprintf(st_magasin_monbre_vente, 10, "%d", magasin_nombre_ventes);		
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_total_ventes")), st_magasin_monbre_vente);

	magasin_somme_totale = mysql_total_encaisse_aujourdhui("0");
	char st_magasin_somme_totale[10];
	snprintf(st_magasin_somme_totale, 10, "%.2f", magasin_somme_totale);
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_somme_totale")), g_strconcat(st_magasin_somme_totale, " €", NULL));


	if( magasin_nombre_ventes > 0 ){
		magasin_montant_moyen = magasin_somme_totale / magasin_nombre_ventes;
		char st_magasin_montant_moyen[10];
		snprintf(st_magasin_montant_moyen, 10, "%.2f", magasin_montant_moyen);
		gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_montant_moyen")), g_strconcat(st_magasin_montant_moyen, " €", NULL));
	} else {
		gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_montant_moyen")), "0.00 €");
	}

	nombre_produits = mysql_nombre_produits();
	char st_magasin_nombre_produits[10];
	snprintf(st_magasin_nombre_produits, 10, "%d", nombre_produits);	
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_nb_produits")), st_magasin_nombre_produits);
	
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "st_label_magasin_dernier_encaissement")), mysql_derniere_vente());
	
	}


