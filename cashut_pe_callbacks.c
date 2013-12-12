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
/* Partie encaisment */
/******************************/

void init_treeview_lists_chaine()
{

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;


  renderer = gtk_cell_renderer_text_new(); // type de donnée
  column = gtk_tree_view_column_new_with_attributes("Quantités",renderer, "text", QUANTITES, NULL); // definie la donnée
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);


  renderer = gtk_cell_renderer_text_new(); // type de donnée
  column = gtk_tree_view_column_new_with_attributes("Libellés",renderer, "text", LIBELLE, NULL); // definie la donnée
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);

  renderer = gtk_cell_renderer_text_new(); // type de donnée
  column = gtk_tree_view_column_new_with_attributes("Prix",renderer, "text", PRIX, NULL); // definie la donnée
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);

  store = gtk_list_store_new(N_COLUMNS,G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_liste_chaine), GTK_TREE_MODEL(store));

  g_object_unref(store);
}


void add_to_treeview_liste_chaine(const gchar *str)
{
  
  GtkTreeIter iter;
 GtkListStore *store;
  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_liste_chaine)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter,QUANTITES,1,LIBELLE, str,PRIX, "2", -1);
    // gtk_list_store_set(store, &iter, INT_PRIX, str2, -1);
}



void Ajouter_liste (GtkWidget *widget, gpointer   data)
{   
     //GtkEntry *textentry = GTK_ENTRY (gtk_builder_getl_object (builder_cashut, "entry1"));
	g_print("%s\n", gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_codebarre")))); 
	Ajouter_produit_liste_chaine(gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_codebarre"))));
    add_to_treeview_liste_chaine(gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_codebarre"))));

}
void testfonction (GtkWidget *widget, gpointer   data)
{   
   GtkTreeSelection *selection;
   selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_liste_chaine));
   gtk_tree_selection_set_mode(selection,GTK_SELECTION_MULTIPLE);
   fprintf(stdout, "%d\n", gtk_tree_selection_count_selected_rows(selection) );
   gtk_tree_selection_select_all(selection);
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



// static void fixed_toggled (GtkCellRendererToggle *cell, gchar *path_str,gpointer data)
// {
//   GtkTreeModel *model = (GtkTreeModel *)data;
//   GtkTreeIter  iter;
//   GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
//   gboolean fixed;
// g_print(path_str);
//   /* get toggled iter */
//   gtk_tree_model_get_iter (model, &iter, path);
//   gtk_tree_model_get (model, &iter, CHECKBOX, &fixed, -1);

//   /* do something with the value */
//   fixed ^= 1;

//   /* set new value */
//   gtk_list_store_set (GTK_LIST_STORE (model), &iter, CHECKBOX, fixed, -1);

//   /* clean up */
//   gtk_tree_path_free (path);
// }