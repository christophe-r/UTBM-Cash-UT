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
extern paiement paiement_encour;

extern TauxTVA *tb_taux_tva;
extern int nombre_taux_tva;

extern GKeyFile *key_file;
/******************************/
/* Partie encaisment */
/******************************/

/******************/
/* Initialisation */
/******************/

void init_treeview_lists_chaine() /* fonction d'initialisation de la treeview*/
{

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeSelection *selection;
  GtkListStore *store;

  /* ajout des colonnes */
  // colonnes des quantitées
  renderer = gtk_cell_renderer_text_new(); // type de donnée
  column = gtk_tree_view_column_new_with_attributes("Qté",renderer, "text", QUANTITES, NULL); // definie la donnée
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);
  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (column),GTK_TREE_VIEW_COLUMN_FIXED); // fixe la taille de la largeur de la colonne
  gtk_tree_view_column_set_fixed_width(GTK_TREE_VIEW_COLUMN(column),35);

  // colonnes des marque
  renderer = gtk_cell_renderer_text_new(); 
  column = gtk_tree_view_column_new_with_attributes("Marque",renderer, "text", MARQUE, NULL); 
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);
  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (column),GTK_TREE_VIEW_COLUMN_FIXED); // fixe la taille de la largeur de la colonne
  gtk_tree_view_column_set_fixed_width(GTK_TREE_VIEW_COLUMN(column),130);

  // colonnes des libellés
  renderer = gtk_cell_renderer_text_new(); 
  column = gtk_tree_view_column_new_with_attributes("Libellé",renderer, "text", LIBELLE, NULL); 
  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (column),GTK_TREE_VIEW_COLUMN_FIXED); // fixe la taille de la largeur de la colonne
  gtk_tree_view_column_set_fixed_width(GTK_TREE_VIEW_COLUMN(column),250); 
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);

  // colonnes des prix
  renderer = gtk_cell_renderer_text_new(); 
  column = gtk_tree_view_column_new_with_attributes("Prix",renderer, "text", PRIX, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_liste_chaine), column);

  // Crée le modéle de donnée
  store = gtk_list_store_new(N_COLUMNS,G_TYPE_INT,G_TYPE_STRING , G_TYPE_STRING, G_TYPE_STRING,G_TYPE_INT);

  // attache le modèle de donnée a notre treeview
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_liste_chaine), GTK_TREE_MODEL(store));

  // permet de pouvoir selectionner plusieur item dans le treeview (fonctionnaliter à venir)
  selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_liste_chaine));
  gtk_tree_selection_set_mode(selection,GTK_SELECTION_MULTIPLE);

  g_object_unref(store);
}
/***********************************************/
/* Maj des données, des labels et des bouttons */
/***********************************************/

void maj_treeview_liste_chaine() /* fonction qui met à jour les lignes de la treeview */
{
  GtkTreeIter iter;
  GtkListStore *store;
  char prix[10];

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_liste_chaine)));
  gtk_list_store_clear(store); /* supprésion des ligne du treeview */ 
  Element *actuel = liste_course->debut;
  while (actuel != NULL ) /* parcour de la liste chainnée en ajoutant les lignes dans le treeview */
  {
	snprintf(prix, 10, "%.2f", actuel->p_produit->prix); /*applique le format pour le prix */
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
						QUANTITES,actuel->quantitee,
						MARQUE,actuel->p_produit->marque,
						LIBELLE,actuel->p_produit->libelle,
						PRIX,g_strconcat(prix, " €", NULL),
						CACHE,actuel->p_produit->produitid,
						-1);
	  actuel = actuel->suivant;

  }	
}

void maj_tb_taux_tva()
{
  int i;
  for (int i = 0; i < nombre_taux_tva; i++) // On remet a zéro le tb des taux de TVA
  {
	tb_taux_tva[i].ht = 0;
	tb_taux_tva[i].tva = 0;
	tb_taux_tva[i].ttc =0;
  }
  Element *actuel = liste_course->debut;
  while (actuel != NULL ) /* parcour de la liste chainnée pour re-remplir les colonnes ttc, ht, tva du tableau de taux tva*/
  {
	for( i=0 ; i<nombre_taux_tva ; i++ ) // pour chaque produits ont se positinne sur la ligne du tb_taux tva avec le code qui correspondant
	{
		if (actuel->p_produit->code_TVA == tb_taux_tva[i].code ) // si le code correspond au code produit on l'ajoute au tb taux tva   
		{
			tb_taux_tva[i].ttc += actuel->p_produit->prix *actuel->quantitee;

			tb_taux_tva[i].ht += ( ( 100 / ( 100 + tb_taux_tva[i].taux ) )*actuel->p_produit->prix *actuel->quantitee);
			tb_taux_tva[i].tva += tb_taux_tva[i].ttc - tb_taux_tva[i].ht;
		}
	}
  actuel = actuel->suivant;
  }

}

void maj_footer_total()
{
	float total_tva=0.0; /* Déclaration  des variables*/
	char char_total_tva[10];
	float total_prix=0.0;
	char char_total_prix[10];
	char char_total_prix_sans_euro[10];
	int i;

	for( i=0 ; i<nombre_taux_tva ; i++ ) // On cherche à avoir le total tva et le total ttc
	{
		   total_tva += tb_taux_tva[i].tva;
		   total_prix += tb_taux_tva[i].ttc;
	}
	snprintf(char_total_prix_sans_euro, 10, "%.2f", total_prix);// transforme les chars en floats et applique les bons formats
	snprintf(char_total_tva, 10, "%.2f", total_tva);
	g_strconcat(char_total_tva, " €", NULL);
	snprintf(char_total_prix, 10, "%.2f", total_prix);
	g_strconcat(char_total_prix, " €", NULL);
	gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_total_ttc")),char_total_prix);// met à jour les labels
	gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_total_tva")),char_total_tva);
}

void maj_modif_liste_produit(short int only_maj_treeview){ // fonctionqui met a jour les différente partie de l'UI quand il y a eu une modification dans la liste de produit

	if (only_maj_treeview == 1 || only_maj_treeview == 2 ) // met à jour le tree view ou pas
	{
		maj_treeview_liste_chaine(); /* Met à jour la treeview si le produit est valable */

	}
	if (only_maj_treeview == 0 || only_maj_treeview == 2 ){
	maj_tb_taux_tva(); /* Met à jour le tb de taux de tva*/
	maj_footer_total(); /* Met à jour les totals */
	maj_facture(); /*met à jour la partie paiement */
	char montantreste[10]="";
	snprintf(montantreste, 10, "%.2f", paiement_encour.reste); // mise à jour de la text entry montant 
	g_strconcat(montantreste, " €", NULL);
	gtk_entry_set_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_montant")),montantreste);
	}
	
	
}

void maj_facture() // fonction qui met jour les bouttons et les  textentry en fonction du reste à payer
{
	char montantreste[10]="";
	int i;
	float total_prix=0.0;

	for( i=0 ; i<nombre_taux_tva ; i++ ) // On calcul de taux de prix total
	{
		   total_prix += tb_taux_tva[i].ttc;
	}

	paiement_encour.reste = total_prix -(paiement_encour.carte + paiement_encour.espece + paiement_encour.cheque);// calcul du reste
	snprintf(montantreste, 10, "%.2f", paiement_encour.reste);// transforme le reste (float) en char
	g_strconcat(montantreste, " €", NULL);
	gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_reste")),montantreste);// met a jour le label

	if (liste_course->debut != NULL && paiement_encour.reste == 0.0) // active ou désactive les bouttons du paiement
	{
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_btn_terminer_nofac")), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_btn_terminer_fac")), TRUE); 
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_btn_espece")), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_btn_cheque")), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_btn_carte")), FALSE);      
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_entry_montant")), FALSE);      
	}else  
	{
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_btn_terminer_nofac")), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_btn_terminer_fac")), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_btn_espece")), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_btn_cheque")), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_btn_carte")), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_entry_montant")), TRUE);
	}
}

void gen_facture(int with_fact)
{
	g_print("debut de la génération de la facture\n");

	FILE* fichier_fac = NULL; // création des variables nécéssaire
	char nom_fichier_fac[100];
	int nb_fac= 0 ;
	char charnb[10];
	float total_prix=0.0;
	int total_article=0;
	int i ;
	char tampon[30]; // chaine tampon utilisé dans les snprinf
	const gchar *chem_fac;
    Element *actuel = liste_course->debut;

    tampon[0]='\n';
	chem_fac=g_key_file_get_string(key_file, "Facture", "chemin", NULL);

    do // boucle qui vérrifie que le fichier de la nouvelle facture ne sera pas écrit dans un fichier éxistant
    {	
    	nom_fichier_fac[0] = '\0'; // vide la chaine de caractères
		snprintf(charnb, 10, "%d", nb_fac);				// Créé le chaine qui contient le chemin du fichier de la facture 
    	strcat(nom_fichier_fac,chem_fac);				//				|
    	strcat(nom_fichier_fac,"facture");				//				|
    	strcat(nom_fichier_fac,charnb);					//				|
    	strcat(nom_fichier_fac,".txt");					//				|
    	fichier_fac = fopen(nom_fichier_fac, "r+"); // essaye d'ouvrir le fichier en lecture/écriture -> renvoie NULL si il y à échec de l'ouverture.
    	nb_fac ++;

    }while(fichier_fac != NULL);

    fichier_fac = fopen(nom_fichier_fac, "w"); // crée le fichier de la facture

    fputs( "******************************************\n" , fichier_fac);
    fprintf( fichier_fac,"%s\n",  align(g_key_file_get_string(key_file, "Facture", "nom", NULL),42));
    fputs( "******************************************\n" , fichier_fac);
    fprintf( fichier_fac,"%s\n",  align(g_key_file_get_string(key_file, "Facture", "ligned1", NULL),42));
    fprintf( fichier_fac,"%s\n",  align(g_key_file_get_string(key_file, "Facture", "ligned2", NULL),42));
    fprintf( fichier_fac,"%s\n",  align(g_key_file_get_string(key_file, "Facture", "ligned3", NULL),42));
    fprintf( fichier_fac,"%s\n",  align(g_key_file_get_string(key_file, "Facture", "ligned4", NULL),42));
    fprintf( fichier_fac,"%s\n",  align(g_key_file_get_string(key_file, "Facture", "ligned5", NULL),42));
    fprintf( fichier_fac,"%s\n",  align(g_key_file_get_string(key_file, "Facture", "ligned6", NULL),42));
    fputs( "------------------------------------------\n" , fichier_fac);
    fputs( "Qte  Marque     Libellé          Prix    \n" , fichier_fac);
    fputs( "------------------------------------------\n" , fichier_fac);
      
  	while (actuel != NULL ) 
  	{	
  		snprintf(tampon, 4, "%d", actuel->quantitee);
  		fprintf( fichier_fac,"%s ",padding_char(tampon,4,0));

  		snprintf(tampon, 11, "%s", actuel->p_produit->marque);
  		fprintf( fichier_fac,"%s ", padding_char(tampon,10,0));

  		snprintf(tampon, 16, "%s", actuel->p_produit->libelle);
  		fprintf(stdout, "%d\n", strlen(tampon));	
  		fprintf( fichier_fac,"%s ", padding_char(tampon,15,0));

  		snprintf(tampon, 8, "%.2f", actuel->p_produit->prix);
  		fprintf( fichier_fac,"%s\n",padding_char(tampon,7,0));

  		actuel = actuel->suivant;
  	}
    fputs( "------------------------------------------\n" , fichier_fac);
    fputs( "Total :                            " , fichier_fac);
	for( i=0 ; i<nombre_taux_tva ; i++ ) // calcul du prix total
	{
		   total_prix += tb_taux_tva[i].ttc;
	}
  	fprintf(fichier_fac, "%.2f€\n",total_prix );//affichage du prix total
    fputs( "------------------------------------------\n" , fichier_fac);

    fputs( "TVA :\n" , fichier_fac);
    for( i=0 ; i<nombre_taux_tva ; i++ ) 
	{	
		if (tb_taux_tva[i].tva != 0.0)
		{
  			snprintf(tampon, 15, "%.2f%%", tb_taux_tva[i].taux);
			fprintf(fichier_fac, "%s ",padding_char(tampon,8,1) ); 
			fprintf(fichier_fac, "%.2f€\n",tb_taux_tva[i].tva ); 
		}
		  
	}
    fputs( "\n" , fichier_fac);

    fputs( "Paimement :\n" , fichier_fac);
    if (paiement_encour.cheque != 0.0)
    {
    	fprintf(fichier_fac, "Chéque                            %.2f€\n",paiement_encour.cheque ); 	
    }
    if (paiement_encour.espece != 0.0)
    {
    	fprintf(fichier_fac, "Espèce                            %.2f€\n",paiement_encour.espece ); 	
    }
    if (paiement_encour.carte != 0.0)
    {
    	fprintf(fichier_fac, "Carte                             %.2f€\n",paiement_encour.carte ); 	
    }
    fputs( "\n" , fichier_fac);

    actuel = liste_course->debut;
  	while (actuel != NULL ) /* parcour de la liste chainnée en ajoutant les lignes dans le treeview */
  	{
  		total_article += actuel->quantitee;
	  	actuel = actuel->suivant;
  	}	
  	if (total_article > 1)
  	{
  		fprintf(fichier_fac, "%d articles\n",total_article );

  	}else{
  		fprintf(fichier_fac, "%d article\n", total_article );
  	}
  	
    fprintf( fichier_fac,"%s\n",  align(g_key_file_get_string(key_file, "Facture", "messagefin", NULL),42));

    fclose(fichier_fac);
	fprintf(stdout, "%s\n", "fin gen facture");
}

char *align(char *text, int taille)
{	
	char ligne[50];
	int i;
	char debut[30];
	char fin[30];
	int taillediv2 = taille / 2 ;

	ligne[0]='\0';
	fin[0]='\0';
	debut[0]='\0';
	int placedeb = taillediv2 -strlen(text)/2;
	int placefin = taille -(placedeb + strlen(text));

	for ( i = 0; i <  placedeb; i++)
	{
		strcat(debut," ");
	}

	for ( i = 0; i <  placefin; i++)
	{
		strcat(fin," ");
	}

	strcat(ligne,debut);
	strcat(ligne,text);
	strcat(ligne,fin);

	return ligne;
}

char *padding_char(char *text, int taille, int type)
{
	char ligne[50];
	int fin_chainne;
	int i;
	int taille_padding;

	ligne[0]='\0';
	strcat(ligne,text);

	if (type == 0 ) // Padding apres la chaine (text) -> text = chaine avant le padding ; taille = taille du padding sur la droite
	{
		fin_chainne = strlen(text);
		taille_padding =  taille - fin_chainne ;
		for ( i = 0; i < taille_padding; i++)
		{
			strcat(ligne," ");
		}

	}
	else{ // padding pour alligné la chaine à droite (text) -> text = Chainne avant le padding ; taille = taille de la chaine aprés le padding (avant la fin de la ligne)

		taille_padding = 43 - (taille + strlen(text));
		for ( i = 0; i < taille_padding; i++)
		{
			strcat(ligne," ");
		}
	}

	return ligne;
}


/********************************************/
/* Fonction d'action sur les bouttons de gtk*/
/********************************************/

void pe_verif_caractere(GtkWidget *widget, gpointer   data) //fonction pour eviter que l'utilisateur ne rajoute d'autre caractére que des nombres pour le code barre
{
	gchar *entry;
	entry = g_strconcat(gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_codebarres"))), NULL);
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder_cashut, "pe_entry_codebarres")), g_strcanon(entry, "0123456789", "")); // autotrise uniqueùent les caratères de 0 à 9;
}

void pe_ajouter_produit (GtkWidget *widget, gpointer   data) /* fonction pour ajouté un produit*/ 
{   
	if (Ajouter_produit_liste_chaine(gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_codebarres"))))) /* On essaye d'ajouté le produit à la liste chainnée */
	 {
		maj_modif_liste_produit(2); // Met à jour les différente partie de l'UI (TVA , Paiement, ...)
		gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder_cashut, "pe_entry_codebarres")),"");
		gtk_widget_grab_focus(GTK_WIDGET(gtk_builder_get_object (builder_cashut, "pe_entry_codebarres")));
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
  gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_total_ttc")),"0 €");// met à jour les labels
  gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_total_tva")),"0 €");
  int i;
  for( i=0 ; i<nombre_taux_tva ; i++ ) // pour chaque ligne de tb_taux_tva on remet à les taux
  {
	tb_taux_tva[i].tva=0;
	tb_taux_tva[i].ht=0;
	tb_taux_tva[i].ttc=0;

  }
   pe_annuler_paiement(GTK_WIDGET(gtk_builder_get_object(builder_cashut, "btn_paiement_annuler"))  , NULL); // annulation des paiements
}

void pe_supprimer_produit(GtkWidget *widget, gpointer   data)
{	
  	GtkTreeIter iter; // Selecteur GTK des lignes
  	GtkTreePath *path = gtk_tree_path_new_first(); // chemim d'un element dans le liststore
 	Element *actuel = liste_course->debut; // première élément de la liste chainé
  	GtkTreeSelection *selection; // selection dans les listestore
  	short int place = 1;

   	selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_liste_chaine)); // prend la selection de notre listestore
	while (actuel != NULL ) /* parcours de la liste chainnée */ 
	{	
		if (gtk_tree_selection_path_is_selected (selection,path) == TRUE) // verifie si le chemin de l'élément sélectionné et sélectionné
	 	{
	 		Supprimer_produit_liste_chaine( place, 1); // supprime le produit de la place en cour 
	 	}
	 	gtk_tree_path_next (path); // selectionne le chemin suivant dans le liststore
	  	actuel = actuel->suivant; // selectionne l'element suivant dans la liste chainé
	  	place ++; // icrémente la place
  	}	
	maj_modif_liste_produit(2); // Met à jour les différente partie de l'UI (TVA , Paiement, ...)
}

void pe_qte_produit_ajout(GtkWidget *widget, gpointer   data){

  	GtkTreePath *path = gtk_tree_path_new_first(); // chemim d'un element dans le liststore
 	Element *actuel = liste_course->debut; // première élément de la liste chainé
  	GtkTreeSelection *selection; // selection dans les listestore
  	short int place = 1;

   	selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_liste_chaine)); // prend la selection de notre listestore
	while (actuel != NULL ) /* parcours de la liste chainnée */ 
	{	
		if (gtk_tree_selection_path_is_selected (selection,path) == TRUE) // verifie si le chemin de l'élément sélectionné et sélectionné
	 	{
	 		actuel->quantitee++; // Ajoute une quantitée au produit en cour
	 		maj_modif_liste_produit(1); // Met à jour le tree view
	 		gtk_tree_selection_select_path(selection,path); // re-sélectionne le champ précedent
	 	}
	 	gtk_tree_path_next (path); // selectionne le chemin suivant dans le liststore
	  	actuel = actuel->suivant; // selectionne l'element suivant dans la liste chainé
	  	place ++; // icrémente la place
  	}	
	maj_modif_liste_produit(0); // Met à jour tout sauf le tree view
}

void pe_qte_produit_retire(GtkWidget *widget, gpointer   data){

  	GtkTreePath *path = gtk_tree_path_new_first(); // chemim d'un element dans le liststore
 	Element *actuel = liste_course->debut; // première élément de la liste chainé
  	GtkTreeSelection *selection; // selection dans les listestore
  	short int place = 1;

   	selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_liste_chaine)); // prend la selection de notre listestore
	while (actuel != NULL ) /* parcours de la liste chainnée */ 
	{	
		if (gtk_tree_selection_path_is_selected (selection,path) == TRUE) // verifie si le chemin de l'élément sélectionné et sélectionné
	 	{
	 		actuel->quantitee--; // Retire une quantitée au produit en cour
	 		if (actuel->quantitee == 0) // si la quantiter vaux 0 
	 		{
	 			Supprimer_produit_liste_chaine( place, 1); // supprime le produit de la place en cour
	 		}else{
	 			gtk_tree_selection_select_path(selection,path); // on reselectionne le champ que si le produitn'a pas été supprimer
	 		}
	 		maj_modif_liste_produit(1); // Met à jour tout sauf le tree view
	 		
	 	}
	 	gtk_tree_path_next (path); // selectionne le chemin suivant dans le liststore
	  	actuel = actuel->suivant; // selectionne l'element suivant dans la liste chainé
	  	place ++; // icrémente la place
  	}
	maj_modif_liste_produit(0); //Met à jour tout sauf le tree view
}

void pe_annuler_paiement(GtkWidget *widget, gpointer   data) // fonction pour annuler le paiement
{
 
	paiement_encour.carte = 0.0; // remet à zéro la structure des paiements
	paiement_encour.espece = 0.0;
	paiement_encour.cheque = 0.0; 
	gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_deja_payer_cheque")),"0 €"); // met a jour les labels des paiement : carte, espece, cheque
	gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_deja_payer_carte")),"0 €");
	gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_deja_payer_espece")),"0 €");
	maj_facture(); // met à jour la partie paiement

}

void pe_btn_cheque(GtkWidget *widget, gpointer   data)// fonction si l'ont encaisse par chéque
{
	const gchar *gmontant; // créé les variables néssaire
	char chmontant[10];
	float montant;
	gmontant = gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_montant"))); // récupere le montant à paiement
	//g_strcanon(gmontant, "0123456789.", ",");
	montant =atof(gmontant); // On le transforme en float
	if (montant > paiement_encour.reste) // test si le montant n'est pas superieure au monant restant à payer
	{
		montant = paiement_encour.reste;
		snprintf(chmontant, 10, "%.2f", paiement_encour.reste);
		gtk_entry_set_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_montant")),chmontant); // on met aussi le a jour le text entry 
	}
	paiement_encour.cheque += montant; // ajout le montant à la structure
	snprintf(chmontant, 10, "%.2f", paiement_encour.cheque);
	// chmontant = g_strconcat(chmontant, " €", NULL);
	gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_deja_payer_cheque")),chmontant); // affiche le montant
	maj_facture(); // met à jour la facture
}

void pe_btn_espece(GtkWidget *widget, gpointer   data)// fonction si l'ont encaisse par espéce
{
	const gchar *gmontant;
	char chmontant[10];
	float montant;
	gmontant = gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_montant")));
	//g_strcanon(gmontant, "0123456789.", ",");
	montant =atof(gmontant);
	if (montant > paiement_encour.reste)
	{
		montant = paiement_encour.reste;
		snprintf(chmontant, 10, "%.2f", paiement_encour.reste);
		gtk_entry_set_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_montant")),chmontant);
	}
	paiement_encour.espece +=montant;
	snprintf(chmontant, 10, "%.2f", paiement_encour.espece);
	// espece = g_strconcat(chmontant, " €", NULL);
	gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_deja_payer_espece")),chmontant);
	maj_facture();
}

void pe_btn_carte(GtkWidget *widget, gpointer   data)// fonction si l'ont encaisse par carte
{
	const gchar *gmontant;
	char chmontant[10];
	float montant;
	gmontant = gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_montant")));
	//g_strcanon(gmontant, "0123456789.", ",");
	montant =atof(gmontant);
	if (montant > paiement_encour.reste)
	{
		montant = paiement_encour.reste;
		snprintf(chmontant, 10, "%.2f", paiement_encour.reste);
		gtk_entry_set_text (GTK_ENTRY (gtk_builder_get_object (builder_cashut, "pe_entry_montant")),chmontant);
	}
	paiement_encour.carte +=montant;
	snprintf(chmontant, 10, "%.2f", paiement_encour.carte);
	// chmontant = g_strconcat(chmontant, " €", NULL);
	gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder_cashut, "pe_lbl_deja_payer_carte")),chmontant);
	maj_facture();
}

void pe_facture_no_fact(GtkWidget *widget, gpointer   data)// fonction si l'ont encaisse par carte
{
   gen_facture(0);
}

void pe_facture_fact(GtkWidget *widget, gpointer   data)// fonction si l'ont encaisse par carte
{
   gen_facture(1);
}


void testfonction (GtkWidget *widget, gpointer   data)
{   
/* Fonction de test*/
	fprintf(stdout, "tb taux tva :\n");

for (int i = 0; i < nombre_taux_tva; i++)
{
	fprintf(stdout, "code : %d   ",tb_taux_tva[i].code );
	fprintf(stdout, "taux : %f   ",tb_taux_tva[i].taux);
	fprintf(stdout, "ht : %f   ",tb_taux_tva[i].ht );
	fprintf(stdout, "ttc : %f   ",tb_taux_tva[i].ttc );
	fprintf(stdout, "tva : %f   \n",tb_taux_tva[i].tva );
}
	fprintf(stdout, "\n");
}
