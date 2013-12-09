#include <my_global.h>
#include <mysql.h>

#include <string.h>

#include <glib.h>

#include "cashut_gu_callbacks.h"
#include "main_mysql.h"

#include "structures.h"



extern GtkBuilder 	*builder_cashut;




/**** Gestion des utilisateurs **********************************/
void on_notebook_change_page6(void){
	liste_utilisateurs_create_liststore();

	gu_reinitialiser_form_modifier();
	gu_reinitialiser_form_supprimer();
}






extern int nombre_chargement_liststore_utilisateurs;
LISTEUTILISATEURS *data_utilisateurs;


void liste_utilisateurs_create_liststore(void){

	int nombre_utilisateurs;
	nombre_utilisateurs = mysql_nombre_utilisateurs();


	data_utilisateurs = (LISTEUTILISATEURS *) malloc( sizeof(LISTEUTILISATEURS) * nombre_utilisateurs);

	data_utilisateurs = mysql_recuperer_utilisateurs();


	GtkTreeView *treeview_utilisateurs;
	treeview_utilisateurs = GTK_TREE_VIEW(gtk_builder_get_object(builder_cashut, "treeview_utilisateurs"));

	// Supprime les colonnes si elles ont déjà été chargées
	if( nombre_chargement_liststore_utilisateurs >= 1 ){
		//g_print("On supprime les colonnes\n");
		gtk_tree_view_remove_column (treeview_utilisateurs, gtk_tree_view_get_column(treeview_utilisateurs, 0));
		gtk_tree_view_remove_column (treeview_utilisateurs, gtk_tree_view_get_column(treeview_utilisateurs, 0));
		gtk_tree_view_remove_column (treeview_utilisateurs, gtk_tree_view_get_column(treeview_utilisateurs, 0));
		gtk_tree_view_remove_column (treeview_utilisateurs, gtk_tree_view_get_column(treeview_utilisateurs, 0));
	}

	nombre_chargement_liststore_utilisateurs++;

	//gtk_tree_view_append_column (treeview_utilisateurs, (GtkTreeViewColumn *)0);


	gint i = 0;
	GtkListStore *liste_utilisateurs_model;
	GtkTreeIter iter;

	/* create list store */
	liste_utilisateurs_model = gtk_list_store_new (4,
												  G_TYPE_STRING,
												  G_TYPE_STRING,
										          G_TYPE_STRING,
										          G_TYPE_STRING);
	/* add data to the list store */
	// G_N_ELEMENTS(data_utilisateurs)
	for (i = 0; i <= (nombre_utilisateurs-1); i++)
    {
		gtk_list_store_append (liste_utilisateurs_model, &iter);
		gtk_list_store_set (liste_utilisateurs_model, &iter,
						  0, data_utilisateurs[i].id,
                          1, data_utilisateurs[i].utilisateur,
                          2, data_utilisateurs[i].motdepasse,
                          3, data_utilisateurs[i].niveau,
                          -1);
    }


	gtk_tree_view_set_model(treeview_utilisateurs, GTK_TREE_MODEL(liste_utilisateurs_model));

	if( data_utilisateurs != NULL ){
		free(data_utilisateurs);
	}

	g_object_unref (liste_utilisateurs_model);

	GtkCellRenderer *renderer;
  	GtkTreeViewColumn *column;

	// column 1
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("ID",
		                                             renderer,
		                                             "text", 0,
		                                             NULL);
	gtk_tree_view_column_set_sort_column_id (column, 0);
	gtk_tree_view_append_column (treeview_utilisateurs, column);

	// column 2
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Nom d'utilisateur",
		                                             renderer,
		                                             "text", 1,
		                                             NULL);
	gtk_tree_view_column_set_sort_column_id (column, 1);
	gtk_tree_view_append_column (treeview_utilisateurs, column);

	// column 3
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Mot de passe",
		                                             renderer,
		                                             "text", 2,
		                                             NULL);
	gtk_tree_view_column_set_sort_column_id (column, 2);
	gtk_tree_view_append_column (treeview_utilisateurs, column);

	// column 4
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Niveau",
		                                             renderer,
		                                             "text", 3,
		                                             NULL);
	gtk_tree_view_column_set_sort_column_id (column, 3);
	gtk_tree_view_append_column (treeview_utilisateurs, column);


}



void gu_treeview_rowactivated(GtkWidget *widget, gpointer user_data){

	// Activer form modifier
	gu_activer_form_modifier();
	// Activer btn supprimer
	gu_activer_form_supprimer();



	GtkTreeView *treeview_utilisateurs;
	treeview_utilisateurs = GTK_TREE_VIEW(gtk_builder_get_object(builder_cashut, "treeview_utilisateurs"));


	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;

	/* This will only work in single or browse selection mode! */

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_utilisateurs));

	gtk_tree_selection_get_selected(selection, &model, &iter);
	
	gchar *nom_utilisateur;
	gchar *mdp_utilisateur;
	gchar *niveau_utilisateur;
	gtk_tree_model_get (model, &iter, 1, &nom_utilisateur,
									  2, &mdp_utilisateur,
									  3, &niveau_utilisateur,
									-1);

	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder_cashut, "gu_entry_nom_utilisateur1")), nom_utilisateur);
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder_cashut, "gu_entry_motdepasse1")), mdp_utilisateur);

	if( g_strcmp0(niveau_utilisateur, "2") == 0 ){ // Si niveau 2
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_1")), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_2")), TRUE);
	} else {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_2")), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_1")), TRUE);
	}






	g_free(nom_utilisateur);
	g_free(mdp_utilisateur);
	g_free(niveau_utilisateur);


}



void 
gu_button_modifier_clicked (GtkWidget *widget, gpointer user_data)
{
	const gchar *nom_utilisateur;
	const gchar *mot_de_passe;
	const gchar *niveau;

	nom_utilisateur = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gu_entry_nom_utilisateur1")));
	mot_de_passe = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gu_entry_motdepasse1")));
	if( gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_2"))) == TRUE ){
		niveau = "2";
	}	else {
		niveau = "1";
	}
		

	GtkTreeView *treeview_utilisateurs;
	treeview_utilisateurs = GTK_TREE_VIEW(gtk_builder_get_object(builder_cashut, "treeview_utilisateurs"));


	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;

	/* This will only work in single or browse selection mode! */

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_utilisateurs));

	gtk_tree_selection_get_selected(selection, &model, &iter);
	
	gchar *id_utilisateur;

	gtk_tree_model_get (model, &iter, 0, &id_utilisateur,-1);


	if( strlen(nom_utilisateur) > 0 && strlen(mot_de_passe) > 0 ){
		if( mysql_utilisateur_modifier(id_utilisateur, nom_utilisateur, mot_de_passe, niveau) == 1 ){
			g_print("Edit user success\n");

			liste_utilisateurs_create_liststore();
			gu_reinitialiser_form_modifier();
			gu_reinitialiser_form_supprimer();

		}
	} else {
		gu_dialog_champs_incorrects();
	}


	g_free(id_utilisateur);

}


void gu_button_supprimer_clicked (GtkWidget *widget, gpointer user_data){

	GtkTreeView *treeview_utilisateurs;
	treeview_utilisateurs = GTK_TREE_VIEW(gtk_builder_get_object(builder_cashut, "treeview_utilisateurs"));

	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;

	/* This will only work in single or browse selection mode! */

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_utilisateurs));

	gtk_tree_selection_get_selected(selection, &model, &iter);
	
	gchar *id_utilisateur;

	gtk_tree_model_get (model, &iter, 0, &id_utilisateur, -1);

	if( mysql_utilisateur_supprimer(id_utilisateur) == 1 ){
		g_print("Delete user success\n");

		liste_utilisateurs_create_liststore();
		gu_reinitialiser_form_modifier();
		gu_reinitialiser_form_supprimer();

	}

	g_free(id_utilisateur);

}



void gu_button_ajouter_clicked (GtkWidget *widget, gpointer user_data){

	const gchar *nom_utilisateur;
	const gchar *mot_de_passe;
	const gchar *niveau;

	nom_utilisateur = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gu_entry_nom_utilisateur2")));
	mot_de_passe = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gu_entry_motdepasse2")));
	if( gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "gu_radiobutton_ajouter_2"))) == TRUE ){
		niveau = "2";
	}	else {
		niveau = "1";
	}

	if( strlen(nom_utilisateur) > 0 && strlen(mot_de_passe) > 0 ){
		if( mysql_utilisateur_ajouter(nom_utilisateur, mot_de_passe, niveau) == 1 ){
			g_print("Add user success\n");

			liste_utilisateurs_create_liststore();
			gu_reinitialiser_form_ajouter();
			gu_reinitialiser_form_modifier();
			gu_reinitialiser_form_supprimer();

		}
	} else {
		gu_dialog_champs_incorrects();
	}

}


void gu_reinitialiser_form_modifier(void){

	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gu_entry_nom_utilisateur1")), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gu_entry_motdepasse1")), "");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_2")), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_1")), TRUE);

	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_entry_nom_utilisateur1")), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_entry_motdepasse1")), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_1")), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_2")), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_button_modifier")), FALSE);
}

void gu_activer_form_modifier(void){

	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_entry_nom_utilisateur1")), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_entry_motdepasse1")), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_1")), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_radiobutton_modifier_2")), TRUE);
	
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_button_modifier")), TRUE);
}


void gu_reinitialiser_form_supprimer(void){
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_button_supprimer")), FALSE);
}

void gu_activer_form_supprimer(void){
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gu_button_supprimer")), TRUE);
}

void gu_reinitialiser_form_ajouter(void){
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gu_entry_nom_utilisateur2")), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gu_entry_motdepasse2")), "");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "gu_radiobutton_ajouter_2")), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object (builder_cashut, "gu_radiobutton_ajouter_1")), TRUE);
}

void gu_dialog_champs_incorrects(void){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new (GTK_WINDOW(gtk_builder_get_object (builder_cashut, "window_connexion")),
                                 1 << 1,
                                 GTK_MESSAGE_INFO,
                                 GTK_BUTTONS_OK,
                                 "Merci de remplir tous les champs correctement.");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

