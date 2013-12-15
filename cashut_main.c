#include <my_global.h>
#include <mysql.h>
#include <gtk/gtk.h>

#include "structures.h"

#include "cashut_main.h"
#include "cashut_main_callbacks.h"

#include "cashut_gu_callbacks.h"
#include "cashut_pe_callbacks.h"
#include "cashut_cp_callbacks.h"

#include "main_mysql.h"

#include "main_gestion_liste.h"

#include "window_cashut.inc"


#include "resources/icon.xpm"
#include "resources/logo_645.xpm"



GtkBuilder 			*builder_cashut;
extern const gchar 	*niveau_utilisateur;

/******** EXTERN CONFIG ********/
extern const gchar 	*caisse_num;
/*******************************/

Liste *liste_course;

TauxTVA *tb_taux_tva;
int nombre_taux_tva;

GtkWidget *treeview_liste_chaine;  

int nombre_chargement_liststore_utilisateurs;


int
cashut_main_window(int argc, char *argv[])
{
	
    GtkWidget  *window_cashut;

    gtk_init (&argc, &argv);
	builder_cashut = gtk_builder_new ();

	gtk_builder_add_from_string(builder_cashut, window_cashut_glade, -1, NULL);
    window_cashut = GTK_WIDGET (gtk_builder_get_object (builder_cashut, "window_cashut"));

    gtk_builder_connect_signals (builder_cashut, NULL);

	// Icône
	GdkPixbuf *icon;
	icon = gdk_pixbuf_new_from_xpm_data((const char **)icon_png);
	gtk_window_set_icon(GTK_WINDOW(window_cashut), icon);

	GdkPixbuf *logo_big;
	logo_big = gdk_pixbuf_new_from_xpm_data((const char **)logo_645);
	
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_builder_get_object(builder_cashut, "image_logo_big_accueil")), logo_big);
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_builder_get_object(builder_cashut, "image_logo_big_apropos")), logo_big);

	
	// Signaux Fenêtre Cash'UT
 	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "window_cashut"),
            "delete-event", G_CALLBACK (on_window_cashut_destroy), NULL
	);

	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "window_cashut"),
            "show", G_CALLBACK(on_window_cashut_show), NULL
	);

	g_signal_connect_after (
            GTK_NOTEBOOK(gtk_builder_get_object (builder_cashut, "notebook")),
            "switch-page", G_CALLBACK (on_notebook_change_page), NULL
	);


	// Signaux Gestion des Utilisateurs
	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "gu_button_modifier"),
            "clicked", G_CALLBACK(gu_button_modifier_clicked), NULL
	);

	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "gu_button_supprimer"),
            "clicked", G_CALLBACK(gu_button_supprimer_clicked), NULL
	);

	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "gu_button_ajouter"),
            "clicked", G_CALLBACK(gu_button_ajouter_clicked), NULL
	);

	g_signal_connect (
            gtk_builder_get_object (builder_cashut, "treeview_utilisateurs"),
            "row-activated", G_CALLBACK(gu_treeview_rowactivated), NULL
	);


	// Signaux Partie Encaissement
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_ajouter")),
			"clicked", G_CALLBACK (pe_ajouter_produit), NULL
	);

	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_annuler")),
			"clicked", G_CALLBACK (pe_annuler), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_suprimer")),
			"clicked", G_CALLBACK (pe_suprimer_produit), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_quant_plus")),
			"clicked", G_CALLBACK (testfonction), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_quant_moins")),
			"clicked", G_CALLBACK (testfonction), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_cheque")),
			"clicked", G_CALLBACK (testfonction), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_espece")),
			"clicked", G_CALLBACK (testfonction), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_carte")),
			"clicked", G_CALLBACK (testfonction), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_terminer_nofac")),
			"clicked", G_CALLBACK (testfonction), NULL
	);
	g_signal_connect ( 
			GTK_WIDGET (gtk_builder_get_object (builder_cashut, "pe_btn_terminer_fac")),
			"clicked", G_CALLBACK (testfonction), NULL
	);

    // Signaux Catalogue Produit
    g_signal_connect (
            gtk_builder_get_object (builder_cashut, "cp_b_rechercher"),
            "clicked", G_CALLBACK(cp_b_rechercher_clicked), NULL
    );

    g_signal_connect (
            gtk_builder_get_object (builder_cashut, "cp_treeview"),
            "row-activated", G_CALLBACK(cp_treeview_rowactivated), NULL
    );

    g_signal_connect (
            gtk_builder_get_object (builder_cashut, "cp_b_ajouter"),
            "clicked", G_CALLBACK(cp_b_ajouter_clicked), NULL
    );

	// Initialisation pour la Gestion des Utilisateurs
	nombre_chargement_liststore_utilisateurs = 0;


	// Initialisation du tableau des correspondances Codes-Taux TVA
	nombre_taux_tva = mysql_nombre_taux_tva(); 
	tb_taux_tva = malloc( sizeof(TauxTVA) * nombre_taux_tva);
	tb_taux_tva = mysql_recuperer_taux_tva();


	// Initialisation pour la Partie Encaissement
	treeview_liste_chaine = GTK_TREE_VIEW (gtk_builder_get_object (builder_cashut, "pe_treeview_liste_chaine"));
	if (gtk_tree_view_get_model (treeview_liste_chaine) == NULL) // normalement exécuté une seule fois au démarrage
	{
		initialisation_liste_chaine();
		init_treeview_lists_chaine();
	}
	

	/*Produit *le_produit;
	le_produit = mysql_recuperer_produit("3147690051107");


	g_print(le_produit->code_barres);
	g_print(le_produit->marque);
	g_print(le_produit->libelle);
	g_print(le_produit->type_marque);*/

//	init_treeview_lists(list);
/*
	if( Ajouter_produit("3264420101830") == 1 ){
		g_print("Produit ajouté\n");
	} else {
		g_print("Probleme ajout\n");
	}

	if( Ajouter_produit("5410076769421") == 1 ){
		g_print("Produit ajouté\n");
	} else {
		g_print("Probleme ajout\n");
	}

	if( Ajouter_produit("4015400535737") == 1 ){
		g_print("Produit ajouté\n");
	} else {
		g_print("Probleme ajout\n");
	}
	//Supprimer_produit(2,1);
	//Supprimer_liste();
	afficherListe();

*/

	gtk_widget_show (window_cashut);                
	gtk_main();
	g_object_unref (G_OBJECT (builder_cashut));
	g_object_unref (icon);
	g_object_unref (logo_big);

	return 0;

}

