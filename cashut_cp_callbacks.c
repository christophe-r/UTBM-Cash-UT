#include <my_global.h>
#include <mysql.h>

#include <string.h>

#include <glib.h>

#include "cashut_cp_callbacks.h"
#include "cashut_pe_callbacks.h"
#include "main_mysql.h"

#include "structures.h"


extern GtkBuilder         *builder_cashut;


/**** Catalogue Produits **********************************/


Produit *cp_liste_recherche;


void catalogue_produits_create_liststore(const gchar *critere, const gchar *recherche){


        int nombre_resultats;
        nombre_resultats = mysql_nombre_rechercher_produits(critere, recherche);


        if( nombre_resultats > 40 ){ // Si trop de résultats
                dialog_trop_de_resultats();
        } else {

				// Gestion du pluriel
                char c_nombre_resultats[2];
                sprintf(c_nombre_resultats, "%d", nombre_resultats);

                char *pluriel_resultats;
                if( nombre_resultats >= 2 ){
                        pluriel_resultats = "s";
                } else {
                        pluriel_resultats = "";
                }

				// Il y a XX résultat(s)
                gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder_cashut, "cp_l_nombre_resultats")), g_strconcat("Il y a ", c_nombre_resultats, " résultat", pluriel_resultats, NULL));

                cp_liste_recherche = (Produit *) malloc( sizeof(Produit) * nombre_resultats);

                cp_liste_recherche = mysql_rechercher_produits(critere, recherche);


                GtkTreeView *cp_treeview;
                cp_treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder_cashut, "cp_treeview"));

                // Supprime systématiquement les colonnes déjà été chargées
                gtk_tree_view_remove_column (cp_treeview, gtk_tree_view_get_column(cp_treeview, 0));
                gtk_tree_view_remove_column (cp_treeview, gtk_tree_view_get_column(cp_treeview, 0));
                gtk_tree_view_remove_column (cp_treeview, gtk_tree_view_get_column(cp_treeview, 0));
                gtk_tree_view_remove_column (cp_treeview, gtk_tree_view_get_column(cp_treeview, 0));
                gtk_tree_view_remove_column (cp_treeview, gtk_tree_view_get_column(cp_treeview, 0));


                gint i = 0;
                GtkListStore *liste_cp_model;
                GtkTreeIter iter;

                /* create list store */
                liste_cp_model = gtk_list_store_new (5, G_TYPE_STRING,
                                                        G_TYPE_STRING,
                                                        G_TYPE_STRING,
                                                        G_TYPE_STRING,
                                                        G_TYPE_STRING);
                /* add data to the list store */
                // G_N_ELEMENTS(data_utilisateurs)
                for (i = 0; i <= (nombre_resultats-1); i++)
                {

                        char prix_resultat[10];
                        snprintf(prix_resultat, 10, "%.2f", cp_liste_recherche[i].prix);

                        gtk_list_store_append (liste_cp_model, &iter);
                        gtk_list_store_set (liste_cp_model, &iter,
                                                          0, cp_liste_recherche[i].code_barres,
										                  1, cp_liste_recherche[i].marque,
										                  2, cp_liste_recherche[i].libelle,
										                  3, cp_liste_recherche[i].conditionnement,
                                                          4, g_strconcat(prix_resultat, " €", NULL),
                                      -1);
                }


                gtk_tree_view_set_model(cp_treeview, GTK_TREE_MODEL(liste_cp_model));

                if( cp_liste_recherche != NULL ){
                        free(cp_liste_recherche);
                }

                g_object_unref (liste_cp_model);

                GtkCellRenderer *renderer;
                  GtkTreeViewColumn *column;

                // column 1
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes ("Code-barres",
                                                                         renderer,
                                                                         "text", 0,
                                                                         NULL);
                gtk_tree_view_column_set_sort_column_id (column, 0);
                gtk_tree_view_append_column (cp_treeview, column);

                // column 2
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes ("Marque",
                                                                         renderer,
                                                                         "text", 1,
                                                                         NULL);
                gtk_tree_view_column_set_sort_column_id (column, 1);
                gtk_tree_view_append_column (cp_treeview, column);

                // column 3
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes ("Libellé",
                                                                         renderer,
                                                                         "text", 2,
                                                                         NULL);
                gtk_tree_view_column_set_sort_column_id (column, 2);
                gtk_tree_view_append_column (cp_treeview, column);

                // column 4
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes ("Conditionnement",
                                                                         renderer,
                                                                         "text", 3,
                                                                         NULL);
                gtk_tree_view_column_set_sort_column_id (column, 3);
                gtk_tree_view_append_column (cp_treeview, column);

                // column 5
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes ("Prix",
                                                                         renderer,
                                                                         "text", 4,
                                                                         NULL);
                gtk_tree_view_column_set_sort_column_id (column, 4);
                gtk_tree_view_append_column (cp_treeview, column);

        }

}


void cp_b_rechercher_clicked(GtkWidget *widget, gpointer user_data){

        const gchar *champ_recherche;
        const gchar *critere;

		// Récupération du radiobox sélectionné
        if( gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "cp_rb_codebarres"))) == TRUE ){
                critere = "ean13";
        } else if( gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "cp_rb_marque"))) == TRUE ){
                critere = "marque";
        } else {
                critere = "libelle";
        }


        champ_recherche = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "cp_t_recherche")));

		// Met à jour le liststore avec la recherche
        catalogue_produits_create_liststore(critere, champ_recherche);

}


void cp_treeview_rowactivated(GtkWidget *widget, gpointer user_data){
	// Active le bouton Ajouter
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "cp_b_ajouter")), TRUE);
}


void cp_b_ajouter_clicked(GtkWidget *widget, gpointer user_data){

        GtkTreeView *cp_treeview;
        cp_treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder_cashut, "cp_treeview"));


        GtkTreeSelection *selection;
        GtkTreeModel     *model;
        GtkTreeIter       iter;

        /* This will only work in single or browse selection mode! */

        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(cp_treeview));

        gtk_tree_selection_get_selected(selection, &model, &iter);
        
        gchar *code_barres;

        gtk_tree_model_get (model, &iter, 0, &code_barres,-1);

		// Rempli le entry dans la partie encaissement
		gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "pe_entry_codebarre")), code_barres);
		// Change de Tab sur la partie Encaissement
		gtk_notebook_set_current_page(GTK_NOTEBOOK(gtk_builder_get_object(builder_cashut, "notebook")), 1);
		// Simule un clique sur le bouton Ajouter produit dans la partie Encaissement
		pe_ajouter_produit(GTK_WIDGET(gtk_builder_get_object(builder_cashut, "pe_entry_codebarre")), NULL);
		// Désactive le bouton ajouter produit dans la partie Catalogue Produits
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "cp_b_ajouter")), FALSE);

		g_free(code_barres);

}


void dialog_trop_de_resultats(){
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new (GTK_WINDOW(gtk_builder_get_object (builder_cashut, "window_cashut")),
                                 1 << 1,
                                 GTK_MESSAGE_INFO,
                                 GTK_BUTTONS_OK,
                                 "Il y a trop de résultats pour votre recherche.\nVeuillez affiner votre recherche...");
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);

}


