#include <my_global.h>
#include <mysql.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixdata.h>

#include "structures.h"

#include "main_callbacks.h"
#include "main_mysql.h"


#include "window_connexion.inc"
#include "resources/icon.xpm"
#include "resources/logo_220.xpm"


GtkBuilder 		*builder_connexion;
MYSQL 			*con;
const gchar 	*niveau_utilisateur;


/******** EXTERN CONFIG ********/
const gchar 	*database_host;
const gchar 	*database_db;
const gchar 	*database_user;
const gchar 	*database_pass;

const gchar 	*caisse_num;
/*******************************/



int
main (int argc, char *argv[])
{

	niveau_utilisateur = "1";
    GtkWidget  *window_connexion;

    gtk_init (&argc, &argv);
	builder_connexion = gtk_builder_new ();
    
	gtk_builder_add_from_string(builder_connexion, window_connexion_glade, -1, NULL);
    //gtk_builder_add_from_file (builder_connexion, "glade/window_connexion.glade", NULL);
    window_connexion = GTK_WIDGET (gtk_builder_get_object (builder_connexion, "window_connexion"));


	/**** ICON ****/
	GdkPixbuf *icon;
	icon = gdk_pixbuf_new_from_xpm_data((const char **)icon_png);
	gtk_window_set_icon(GTK_WINDOW(window_connexion), icon);
	g_object_unref (icon);
	/**************/

	GdkPixbuf *logo_small;
	logo_small = gdk_pixbuf_new_from_xpm_data((const char **)logo_220);
	
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_builder_get_object(builder_connexion, "image_logo_small")), logo_small);



	/************************* CONFIG FILE *************************/
	GKeyFile *key_file;
	key_file = g_key_file_new ();
	GKeyFileFlags flags;
	flags = G_KEY_FILE_NONE;

	if( g_key_file_load_from_file(key_file, "./Cash'UT/config.ini", flags, NULL) ){
		g_print("Config file loaded.\n");
	} else {
		g_print("Config file couldn't be loaded.\n");
		dialog_erreur_config_file();
	}

	database_host = g_key_file_get_string(key_file, "Database", "host", NULL);
	database_db   = g_key_file_get_string(key_file, "Database", "db", NULL);
	database_user = g_key_file_get_string(key_file, "Database", "user", NULL);
	database_pass = g_key_file_get_string(key_file, "Database", "pass", NULL);

	caisse_num    = g_key_file_get_string(key_file, "Caisse", "caisse_num", NULL);
	/**************************************************************/

	const gchar *caisse_message;
	caisse_message = g_strconcat("Caisse no: ", caisse_num, "\n", NULL);
	g_print(caisse_message);

    gtk_builder_connect_signals (builder_connexion, NULL);

 	g_signal_connect (
            gtk_builder_get_object (builder_connexion, "button_destroy"),
            "clicked", G_CALLBACK (on_window_destroy), NULL
	);

 	g_signal_connect (
            gtk_builder_get_object (builder_connexion, "window_connexion"),
            "delete-event", G_CALLBACK (on_window_destroy), NULL
	);


	g_signal_connect (
            gtk_builder_get_object (builder_connexion, "button_connect"),
            "clicked", G_CALLBACK (on_button_connect_clicked), NULL
	);

	g_signal_connect (
            gtk_builder_get_object (builder_connexion, "window_connexion"),
            "show", G_CALLBACK(connect_to_mysql), NULL
	);



	#ifdef AUTOCONNECT
		gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder_connexion, "username")), "christophe");
		gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder_connexion, "password")), "1234");

		connect_to_mysql();
		on_button_connect_clicked(GTK_WIDGET(gtk_builder_get_object(builder_connexion, "button_connect")), NULL);
	#endif



    gtk_widget_show (window_connexion);                
    gtk_main();
	g_object_unref (G_OBJECT (builder_connexion));
	g_object_unref (logo_small);

    return 0;
}


