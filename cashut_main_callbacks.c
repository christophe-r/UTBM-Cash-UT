#include <my_global.h>
#include <mysql.h>

#include <glib.h>

#include "cashut_main_callbacks.h"
#include "main_mysql.h"

#include "cashut_gu_callbacks.h"

#include "main_gestion_liste.h"

#include "structures.h"


extern GtkBuilder 	*builder_connexion;
extern GtkBuilder 	*builder_cashut;
extern const gchar 	*niveau_utilisateur;

extern TauxTVA *taux_tva;
extern int nombre_taux_tva;


Produit *data_recherche_produits;


extern GtkWidget *list;   
extern GtkListStore *store;




void 
on_window_cashut_destroy (GtkWidget *widget, gpointer user_data)
{
    //gtk_main_quit ();
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
			g_print("Tab0\n");
		break;

		case 1: // ENCAISSEMENT
			g_print("Tab1\n");
		break;

		case 2: // CALCULATRICE
			g_print("Tab2\n");
		break;

		case 3: // CATALOGUE
			g_print("Tab3\n");
		break;

		case 4: // STATISTIQUES
			g_print("Tab4\n");
		break;

		case 5: // GESTION PRODUITS
			g_print("Tab5\n");
		break;

		case 6: // GESTION UTILISATEURS
			on_notebook_change_page6();
		break;

		case 7: // A PROPOS
			g_print("Tab7\n");
		break;

		default:
			g_print("Error\n");
		break;
	}

}




float tva_code2taux(int code){
	
	int i;
	for( i=0 ; i<=(nombre_taux_tva-1) ; i++ ){
		if( code == taux_tva[i].code ){
			return (float)taux_tva[i].taux;
		}
	}

	return 0.0;
}



/******************************/
/* Partie Gestion des produit */
/******************************/
void init_treeview_lists(GtkWidget *list)
{

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new(); // type de donnée
  column = gtk_tree_view_column_new_with_attributes("Code barres",renderer, "text", TEXT_LIBELLE, NULL); // definie la donnée
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

  renderer = gtk_cell_renderer_text_new(); // type de donnée
  column = gtk_tree_view_column_new_with_attributes("Prix",renderer, "text", INT_PRIX, NULL); // definie la donnée
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

  g_object_unref(store);
}


void add_to_list(GtkWidget *list, const gchar *str)
{
  
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, TEXT_LIBELLE, str, -1);
    // gtk_list_store_set(store, &iter, INT_PRIX, str2, -1);
}



void Ajouter_liste (GtkWidget *widget, gpointer   data)
{   
     //GtkEntry *textentry = GTK_ENTRY (gtk_builder_getl_object (builder_cashut, "entry1"));
	g_print("%s\n", gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "entry_codebarre")))); 
	Ajouter_produit(gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "entry_codebarre"))));
//  add_to_list(list, gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "entry_codebarre"))));

}


