#include <my_global.h>
#include <mysql.h>

#include <glib.h>

#include "cashut_main_callbacks.h"
#include "main_mysql.h"

#include "cashut_gu_callbacks.h"
#include "cashut_pe_callbacks.h"

#include "main_gestion_liste.h"

#include "structures.h"


extern GtkBuilder 	*builder_connexion;
extern GtkBuilder 	*builder_cashut;
extern const gchar 	*niveau_utilisateur;


extern TauxTVA *tb_taux_tva;
Produit *data_recherche_produits; 




void 
on_window_cashut_destroy (GtkWidget *widget, gpointer user_data)
{
	if( tb_taux_tva != NULL ){
		free(tb_taux_tva);
	}

	gtk_widget_hide(GTK_WIDGET (gtk_builder_get_object (builder_cashut, "window_cashut")));
	gtk_widget_show(GTK_WIDGET (gtk_builder_get_object (builder_connexion, "window_connexion")));
	gtk_entry_set_text (GTK_ENTRY(gtk_builder_get_object (builder_connexion, "username")), "");
	gtk_entry_set_text (GTK_ENTRY(gtk_builder_get_object (builder_connexion, "password")), "");
	gtk_widget_grab_focus(GTK_WIDGET(gtk_builder_get_object (builder_connexion, "username")));

}

void 
on_window_cashut_show (GtkWidget *widget, gpointer user_data)
{

	if( g_strcmp0(niveau_utilisateur, "1") == 0 ){ // Si le niveau == 1
			// le page_num (2nd parametre) vaut "4" trois fois car on enlève la 4ème tab (en comptant à partir de 0) en sachant que GTK reindexe le notebook à chaque suppression
			gtk_notebook_remove_page(GTK_NOTEBOOK (gtk_builder_get_object (builder_cashut, "notebook")), 4); // Stats
			gtk_notebook_remove_page(GTK_NOTEBOOK (gtk_builder_get_object (builder_cashut, "notebook")), 4); // Gestion prod
			gtk_notebook_remove_page(GTK_NOTEBOOK (gtk_builder_get_object (builder_cashut, "notebook")), 4); // Gestion utilisateurs
	}

}


void 
on_notebook_change_page (GtkWidget *widget, gpointer user_data)
{
	gint current_page;
	current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(gtk_builder_get_object(builder_cashut, "notebook")));

	switch( current_page ){
		case 0: // ACCUEIL
			//g_print("Tab0\n");
		break;

		case 1: // ENCAISSEMENT
			//g_print("Tab1\n");
		break;

		case 2: // CALCULATRICE
			//g_print("Tab2\n");
		break;

		case 3: // CATALOGUE
			//g_print("Tab3\n");
		break;

		case 4: // STATISTIQUES
			//g_print("Tab4\n");
		break;

		case 5: // GESTION PRODUITS
			//g_print("Tab5\n");
		break;

		case 6: // GESTION UTILISATEURS
			on_notebook_change_page6();
		break;

		case 7: // A PROPOS
			//g_print("Tab7\n");
		break;

		default:
			g_print("Error\n");
		break;
	}

}


