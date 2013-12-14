#include <my_global.h>

#include <string.h>

#include <glib.h>

#include "cashut_pe_callbacks.h"
#include "main_mysql.h"
#include "main_gestion_liste.h"
#include "structures.h"



extern GtkBuilder 	*builder_cashut;

extern GtkWidget *treeview_liste_chaine;
extern Liste *liste_course;

extern TauxTVA *taux_tva;
extern nombre_taux_tva;

/******************************/
/* Partie encaisment */
/******************************/

void init_treeview_lists_chaine() /* fonction d'initialisation de la treeview*/
{

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeSelection *selection;
  GtkListStore *store;

  /* ajout des colonnes */
  // colonnes des quantitées
  renderer = gtk_cell_renderer_text_new(); // type de donnée
  column = gtk_tree_view_column_new_with_attributes("Quant",renderer, "text", QUANTITES, NULL); // definie la donnée
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);
  fprintf(stdout, "%d\n", gtk_tree_view_column_get_width (GTK_TREE_VIEW_COLUMN(column)) );

  // colonnes des quantitées
  renderer = gtk_cell_renderer_text_new(); 
  column = gtk_tree_view_column_new_with_attributes("Marque",renderer, "text", MARQUE, NULL); 
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);

  // colonnes des quantitées
  renderer = gtk_cell_renderer_text_new(); 
  column = gtk_tree_view_column_new_with_attributes("Libellés",renderer, "text", LIBELLE, NULL); 
  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (column),GTK_TREE_VIEW_COLUMN_FIXED); // fixe la taille de la largeur de la colonne
  gtk_tree_view_column_set_fixed_width(GTK_TREE_VIEW_COLUMN(column),150); 
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);

  // colonnes des quantitées
  renderer = gtk_cell_renderer_text_new(); 
  column = gtk_tree_view_column_new_with_attributes("Prix",renderer, "text", PRIX, NULL);
  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (column),GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_fixed_width(GTK_TREE_VIEW_COLUMN(column),20);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);

  // Crée le modéle de donnée
  store = gtk_list_store_new(N_COLUMNS,G_TYPE_UINT,G_TYPE_STRING , G_TYPE_STRING, G_TYPE_DOUBLE);

  // attache le modèle de donnée a notre treeview
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_liste_chaine), GTK_TREE_MODEL(store));

  // permet de pouvoir selectionner plusieur item dans le treeview
  selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_liste_chaine));
  gtk_tree_selection_set_mode(selection,GTK_SELECTION_MULTIPLE);

  g_object_unref(store);
}


void maj_treeview_liste_chaine() /* fonction qui met à jour les lignes de la treeview */
{
  GtkTreeIter iter;
 	GtkListStore *store;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_liste_chaine)));
  gtk_list_store_clear(store); /* supprésion des ligne du treeview */ 
  Element *actuel = liste_course->debut;
  while (actuel != NULL ) /* parcour de la liste chainnée en ajoutant les lignes dans le treeview */
  {
      gtk_list_store_append(store, &iter);
 	    gtk_list_store_set(store, &iter,
 						QUANTITES,actuel->quantitee,
            MARQUE,actuel->p_produit->marque,
 						LIBELLE,actuel->p_produit->libelle,
 						PRIX,actuel->p_produit->prix,
 						-1);
      actuel = actuel->suivant;
  }	
}

float tva_code2taux(int code){ /* fonction qui transforme le code en taux de tva*/ 
  
  int i;
  for( i=0 ; i<=(nombre_taux_tva-1) ; i++ ){ /* parcour le tableau de structure de taux_tva */
    if( code == taux_tva[i].code ){ // si sont code correspond à notre code on retourne sont taux
      return (float)taux_tva[i].taux;
    }
  }

  return 0.0;
}

void pe_ajouter_produit (GtkWidget *widget, gpointer   data) /* fonction pour ajouté un produit*/ 
{   
	if (Ajouter_produit_liste_chaine(gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_codebarre"))))) /* On essaye d'ajouté le produit à la liste chainnée */
	 {
	 	 maj_treeview_liste_chaine(); /* Mais à jour la treeview si le produit est valable*/
	 }else{
	 	g_print("erreur\n"); /* sinon on affiche erreur*/
	 }
}

void pe_annuler(GtkWidget *widget, gpointer   data)/* fonction pour annuler la commande*/
{
  GtkListStore *store;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_liste_chaine)));
  gtk_list_store_clear(store); /* supprésion des ligne du treeview */
  Supprimer_liste_chaine(); // vide la liste chainnée

}

void pe_suprimer_produit(GtkWidget *widget, gpointer   data)
{


}



void testfonction (GtkWidget *widget, gpointer   data)
{   
/* Fonction de test*/

//    GtkTreeSelection *selection;
//    GtkTreeIter *iter;
//     GtkListStore *store;
//     store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_liste_chaine)));
//     g_print("1");
//    selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_liste_chaine));
//    g_print("2");
//    gtk_tree_selection_get_selected (selection,store,iter);
//    g_print("3");
//    gtk_list_store_remove (store,iter);
// g_print("4");
//   // GList *testlist;
//   // testlist = gtk_tree_selection_get_selected_rows (selection,store);
//   // g_print(gtk_tree_path_to_string (testlist->data ));


//   // fprintf(stdout, "%d\n", gtk_tree_selection_count_selected_rows(selection) );
//   // gtk_tree_selection_select_all(selection);
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