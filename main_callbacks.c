#include <my_global.h>
#include <mysql.h>
#include <glib.h>

#include "main_callbacks.h"
#include "main_mysql.h"
#include "cashut_main.h"


extern GtkBuilder 	*builder_connexion;
extern MYSQL 		*con;
extern const gchar 	*niveau_utilisateur;
extern const gchar 	*nom_utilisateur;
extern const gchar 	*num_utilisateur;

void 
on_window_destroy (GtkWidget *widget, gpointer user_data)
{
	finish_with_error();
	g_print ("Cash'UT quitted with success\n");
    gtk_main_quit();
	exit(1);
}

void 
on_button_connect_clicked (GtkWidget *widget, gpointer user_data)
{
	const gchar *username;
	const gchar *password;

	username = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_connexion, "username")));
	password = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_connexion, "password")));


	if( mysql_verifier_identification(username, password) == 1 ){
		g_print("Authentification success\n");
		
		nom_utilisateur = username;
		niveau_utilisateur = mysql_niveau_utilisateur(username, password);
		num_utilisateur = mysql_num_utilisateur(username, password);

		if( g_strcmp0(niveau_utilisateur, "2") == 0 ){ // Si le niveau == 2
			g_print("Level 2\n");
		} else {
			g_print("Level 1\n");
		}

		gtk_widget_hide(GTK_WIDGET (gtk_builder_get_object (builder_connexion, "window_connexion")));
		cashut_main_window(); // On lance la fenêtre principale
		

	} else {
		dialog_mauvais_identifiants();
	}

}

void dialog_mysql_error(){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new (GTK_WINDOW(gtk_builder_get_object (builder_connexion, "window_connexion")),
                                 1 << 1,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_CLOSE,
                                 "Erreur: Cash'UT ne peut pas se connecter au serveur MySQL.\nVeuillez vérifier la configuration dans ./Cash'UT/config.ini");
	gtk_dialog_run (GTK_DIALOG (dialog));
	exit(1);

}

void dialog_mauvais_identifiants(){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new (GTK_WINDOW(gtk_builder_get_object (builder_connexion, "window_connexion")),
                                 1 << 1,
                                 GTK_MESSAGE_INFO,
                                 GTK_BUTTONS_OK,
                                 "Les identifiants sont incorrects.\nVeuillez réessayer...");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);

	const gchar *empty;
	empty = "";

	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object (builder_connexion, "password")), empty);
	gtk_widget_grab_focus(GTK_WIDGET(gtk_builder_get_object (builder_connexion, "username")));

}

void dialog_erreur_config_file(){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new (GTK_WINDOW(gtk_builder_get_object (builder_connexion, "window_connexion")),
                                 1 << 1,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_CLOSE,
                                 "Erreur: Le fichier de configuration ne peut pas être chargé...\nVeuillez vérifier la présence du fichier ./Cash'UT/config.ini");
	gtk_dialog_run (GTK_DIALOG (dialog));
	exit(1);

}
