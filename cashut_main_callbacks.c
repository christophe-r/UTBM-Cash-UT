#include <my_global.h>
#include <mysql.h>

#include <glib.h>

#include "cashut_main_callbacks.h"
#include "main_mysql.h"

#include "cashut_gu_callbacks.h"
#include "cashut_pe_callbacks.h"
#include "cashut_ca_callbacks.h"

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


void cashut_key_event(GtkWidget *widget, GdkEventKey *event){

	// Gestion de la calculatrice au clavier
	if( gtk_notebook_get_current_page(GTK_NOTEBOOK(gtk_builder_get_object(builder_cashut, "notebook"))) == 2 ){

		GtkLabel *ca_label_screen;
		ca_label_screen = GTK_LABEL(gtk_builder_get_object(builder_cashut, "ca_screen"));

		const gchar *keyvalue;
		keyvalue = gdk_keyval_name(event->keyval);

		if( g_strcmp0(keyvalue, "Return") == 0 || g_strcmp0(keyvalue, "KP_Enter") == 0 || g_strcmp0(keyvalue, "equal") == 0 ){ 
			ca_equals(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_equals")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_Decimal") == 0 || g_strcmp0(keyvalue, "period") == 0 || g_strcmp0(keyvalue, "comma") == 0 ){
			ca_append_decimal(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_decimal")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "BackSpace") == 0 ){
			ca_clear(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_clear")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_0") == 0 || g_strcmp0(keyvalue, "0") == 0  ){ 
			ca_append_0(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_0")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_1") == 0 || g_strcmp0(keyvalue, "1") == 0  ){ 
			ca_append_1(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_1")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_2") == 0 || g_strcmp0(keyvalue, "2") == 0  ){ 
			ca_append_2(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_2")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_3") == 0 || g_strcmp0(keyvalue, "3") == 0  ){ 
			ca_append_3(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_3")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_4") == 0 || g_strcmp0(keyvalue, "4") == 0  ){ 
			ca_append_4(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_4")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_5") == 0 || g_strcmp0(keyvalue, "5") == 0  ){ 
			ca_append_5(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_5")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_6") == 0 || g_strcmp0(keyvalue, "6") == 0  ){ 
			ca_append_6(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_6")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_7") == 0 || g_strcmp0(keyvalue, "7") == 0  ){ 
			ca_append_7(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_7")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_8") == 0 || g_strcmp0(keyvalue, "8") == 0  ){ 
			ca_append_8(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_8")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_9") == 0 || g_strcmp0(keyvalue, "9") == 0  ){ 
			ca_append_9(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_9")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_Add") == 0 || g_strcmp0(keyvalue, "plus") == 0 ){ 
			ca_add(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_plus")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_Subtract") == 0 || g_strcmp0(keyvalue, "minus") == 0 ){ 
			ca_subtract(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_minus")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_Multiply") == 0 || g_strcmp0(keyvalue, "asterisk") == 0 ){
			ca_multiply(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_asterisk")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "KP_Divide") == 0 || g_strcmp0(keyvalue, "slash") == 0 ){
			ca_divide(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_slash")), ca_label_screen);
		} else if( g_strcmp0(keyvalue, "percent") == 0 ){
			ca_percent(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "ca_percent")), ca_label_screen);
		}
	}

		//g_print("%s %d\n", gdk_keyval_name (event->keyval), (int)event->keyval);
}



