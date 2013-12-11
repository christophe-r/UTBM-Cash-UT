#include <my_global.h>
#include <mysql.h>

#include <string.h>

#include <glib.h>

#include "cashut_pe_callbacks.h"
#include "main_mysql.h"
#include "main_gestion_liste.h"
#include "structures.h"



extern GtkBuilder 	*builder_cashut;

extern TauxTVA *taux_tva;
extern int nombre_taux_tva;

extern GtkWidget *treeview_liste_chaine;   
// extern GtkListStore *store;


/******************************/
/* Partie Gestion des produit */
/******************************/

void init_treeview_lists_chaine()
{

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new(); // type de donnée
  column = gtk_tree_view_column_new_with_attributes("Code barres",renderer, "text", TEXT_LIBELLE, NULL); // definie la donnée
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);

  renderer = gtk_cell_renderer_text_new(); // type de donnée
  column = gtk_tree_view_column_new_with_attributes("Prix",renderer, "text", INT_PRIX, NULL); // definie la donnée
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);

  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_liste_chaine), GTK_TREE_MODEL(store));

  g_object_unref(store);
}


void add_to_treeview_liste_chaine(const gchar *str)
{
  
  GtkTreeIter iter;
 GtkListStore *store;
  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_liste_chaine)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, TEXT_LIBELLE, str, -1);
    // gtk_list_store_set(store, &iter, INT_PRIX, str2, -1);
}



void Ajouter_liste (GtkWidget *widget, gpointer   data)
{   
     //GtkEntry *textentry = GTK_ENTRY (gtk_builder_getl_object (builder_cashut, "entry1"));
	g_print("%s\n", gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "entry_codebarre")))); 
	Ajouter_produit_liste_chaine(gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "entry_codebarre"))));
    add_to_treeview_liste_chaine(gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "entry_codebarre"))));

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



