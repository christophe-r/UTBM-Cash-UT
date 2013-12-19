#include <my_global.h>
#include <mysql.h>

#include <string.h>

#include <glib.h>

#include "main_gestion_liste.h"

#include "cashut_gp_callbacks.h"
#include "main_mysql.h"
#include "structures.h"




extern GtkBuilder 	*builder_cashut;




/*******************************/
/* Partie Gestion des produits */
/*******************************/

void gp_button_valider_click (GtkWidget *widget, gpointer user_data)
{
	struct Produit *gp_produit_entre;
	const gchar *code_barres;
	code_barres = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_code_ean13")));
	
	if (bVerification_somme_de_controle(code_barres)== 1)
		{
		if ( mysql_verifier_existence_produit(code_barres) == 1)
			{
				gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "gp_existance_produit")), "Le produit exite dans le catalogue");
				gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_button_modif")));
				gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_button_sup")));
				
				gp_afficher_champs();
				
				gp_produit_entre =  mysql_recuperer_produit(code_barres);
							
				gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_marque")), gp_produit_entre->marque);
				gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_nom_produit")), gp_produit_entre->libelle);
				gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_type_marque")), gp_produit_entre->type_marque);
				
				char gp_code_tva[2];
				snprintf(gp_code_tva,2,"%d",gp_produit_entre->code_TVA);
				gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_tva")),gp_code_tva);

				char gp_prix[10];
				snprintf(gp_prix,10,"%.2f",gp_produit_entre->prix);
				gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_prix")),gp_prix);
				
				if (gp_produit_entre->conditionnement != NULL)
				{
					gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_info_cond")), gp_produit_entre->conditionnement);
				}
				
			}
			else
			{
				gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "gp_existance_produit")), "Le produit n'existe pas dans le catalogue");
				gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_button_ajout")));
				gp_afficher_champs();
			}
		}
		else
		{
			gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "gp_existance_produit")), "Le code-barres saisi n'est pas normalisé EAN13");	
		}
}

void gp_afficher_champs()
{
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_code_ean13")), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_button_valider")), FALSE);
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_marque")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_nom_produit")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_type_marque")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_tva")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_prix")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_info")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_marque")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_nom_produit")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_type_marque")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_prix")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_prix_devise")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_info_cond")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_button_annuler")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_tva")));
}

void gp_cacher_vider_champs()
{
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object (builder_cashut, "gp_existance_produit")),"");
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_code_ean13")),"");
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_marque")),"");
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_nom_produit")),"");
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_type_marque")),"");
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_prix")),"");
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_info_cond")),"");
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_cashut, "gp_entry_tva")),"");
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_marque")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_nom_produit")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_type_marque")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_tva")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_prix")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_prix_devise")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_label_info")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_marque")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_nom_produit")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_type_marque")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_prix")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_info_cond")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_button_modif")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_button_sup")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_button_ajout")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_button_annuler")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_entry_tva")));
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "gp_code_ean13")), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut,	"gp_button_valider")),TRUE);
}

void gp_button_annuler_click()
{
	gp_dialog_button_annuler();
}

void gp_dialog_button_annuler()
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new (GTK_WINDOW(gtk_builder_get_object (builder_cashut, "window_cashut")),
                                 1 << 1,
                                 GTK_MESSAGE_WARNING,
                                 GTK_BUTTONS_OK_CANCEL,
                                 "La confirmation de cette opération\nréinitialisera la formulaire.");
	gint result = gtk_dialog_run (GTK_DIALOG (dialog));
	
	switch (result)
  {
    case GTK_RESPONSE_OK:
       gp_cacher_vider_champs();
       break;
    default:
       break;
  }


	gtk_widget_destroy (dialog);
}

void gp_button_modif_click()
{
	if( 1 == gp_modifier_produit() ){
		gp_cacher_vider_champs();
	}
}

void gp_button_ajout_click()
{
	if( 1 == gp_ajouter_produit() ){
		gp_cacher_vider_champs();
	}
}

void gp_button_sup_click()
{
		GtkWidget *dialog;
	dialog = gtk_message_dialog_new (GTK_WINDOW(gtk_builder_get_object (builder_cashut, "window_cashut")),
                                 1 << 1,
                                 GTK_MESSAGE_WARNING,
                                 GTK_BUTTONS_OK_CANCEL,
                                 "La confirmation de cette opération supprimera\ndéfinitivement de la base de données les\ninformations concernant ce produit,\net réinitialisera le formulaire.");
	gint result = gtk_dialog_run (GTK_DIALOG (dialog));
	
	const gchar *code_barres;

	switch (result)
  {
    case GTK_RESPONSE_OK:
		code_barres = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_code_ean13")));	
		if( 0 == mysql_produit_supprimer(code_barres) )
		{
			gp_dialog_erreur_db();
		}
		gp_cacher_vider_champs();
       break;
    default:
       break;
  }

	gtk_widget_destroy (dialog);

}


int gp_ajouter_produit()
{

	const gchar *gp_marque;
	const gchar *gp_nom_produit;
	const gchar *gp_type_marque;
	const gchar *gp_prix;
	const gchar *gp_info_conditionnement;
	const gchar *gp_code_tva;
	const gchar *gp_code_barres;

	gp_marque = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_marque")));
	gp_nom_produit = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_nom_produit")));
	gp_type_marque = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_type_marque")));
	gp_prix = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_prix")));
	gp_info_conditionnement = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_info_cond")));
	gp_code_tva = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_tva")));
	gp_code_barres = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_code_ean13")));

	

	if( g_strcmp0(gp_marque, "") != 0 && g_strcmp0(gp_nom_produit, "") != 0 && g_strcmp0(gp_type_marque, "") != 0 && g_strcmp0(gp_prix, "") != 0 && g_strcmp0(gp_code_tva, "") != 0 ){
		if( 0 == mysql_produit_ajouter(gp_code_barres, gp_marque, gp_nom_produit, gp_type_marque, gp_info_conditionnement, gp_code_tva, gp_prix) ){
			gp_dialog_erreur_db();
			return 0;
		}
	} else {
		gp_dialog_erreur_champs();
		return 0;
	}
	
	return 1;
	
}

int gp_modifier_produit()
{

	const gchar *gp_marque;
	const gchar *gp_nom_produit;
	const gchar *gp_type_marque;
	const gchar *gp_prix;
	const gchar *gp_info_conditionnement;
	const gchar *gp_code_tva;
	const gchar *gp_code_barres;


	gp_marque = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_marque")));
	gp_nom_produit = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_nom_produit")));
	gp_type_marque = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_type_marque")));
	gp_prix = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_prix")));
	gp_info_conditionnement = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_info_cond")));
	gp_code_tva = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_entry_tva")));
	gp_code_barres = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "gp_code_ean13")));



	if( g_strcmp0(gp_marque, "") != 0 && g_strcmp0(gp_nom_produit, "") != 0 && g_strcmp0(gp_type_marque, "") != 0 && g_strcmp0(gp_prix, "") != 0 && g_strcmp0(gp_code_tva, "") != 0 ){
		if( 0 == mysql_produit_modifier(gp_code_barres, gp_marque, gp_nom_produit, gp_type_marque, gp_info_conditionnement, gp_code_tva, gp_prix) ){
			gp_dialog_erreur_db();
			return 0;
		}
	} else {
		gp_dialog_erreur_champs();
		return 0;
	}
	
	return 1;
}

void gp_verif_entiers(GtkWidget *widget, gpointer user_data)
{
	gchar *entry;
	entry = g_strconcat(gtk_entry_get_text (GTK_ENTRY (widget)), NULL);

	const gchar *nouvelle_chaine;
	nouvelle_chaine = g_strcanon(entry, (const gchar *)"0123456789", (gchar) NULL);

	gtk_entry_set_text(GTK_ENTRY(widget), nouvelle_chaine);
}

void gp_verif_floats(GtkWidget *widget, gpointer user_data)
{
	gchar *entry;
	entry = g_strconcat(gtk_entry_get_text (GTK_ENTRY (widget)), NULL);

	const gchar *nouvelle_chaine;
	nouvelle_chaine = g_strcanon(entry, (const gchar *)"0123456789.", (gchar) NULL);

	gtk_entry_set_text(GTK_ENTRY(widget), nouvelle_chaine);
}


void gp_dialog_erreur_db(){
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new (GTK_WINDOW(gtk_builder_get_object (builder_cashut, "window_cashut")),
			                         1 << 1,
			                         GTK_MESSAGE_WARNING,
			                         GTK_BUTTONS_OK,
			                         "Une erreur s'est produite lors\nde l'opération.");
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
}

void gp_dialog_erreur_champs(){
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new (GTK_WINDOW(gtk_builder_get_object (builder_cashut, "window_cashut")),
			                         1 << 1,
			                         GTK_MESSAGE_WARNING,
			                         GTK_BUTTONS_OK,
			                         "Merci de remplir tous les champs correctement.");
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
}

