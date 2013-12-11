#include <my_global.h>
#include <mysql.h>

#include <string.h>
#include <glib.h>

#include "structures.h"

#include "main_mysql.h"

#include "main_gestion_liste.h"


extern Liste *liste_course;

int bVerification_somme_de_controle(const gchar *p_code_ean13){

	if( strlen(p_code_ean13) != 13 ){ // S'il n'y a pas 13 caractères
		return 0;
	}

	int i = 0;
	int somme_de_controle = 0;
	int somme = 0;
	double modulo = 0;

	for( i=0; i<12; i +=2 ){

		char element[2];
		memcpy(element, &p_code_ean13[i], 1 );
		element[1] = '\0';

		somme = somme + atoi(element);
	}
 
	for( i=1; i<12; i += 2 ){

		char element[2];
		memcpy(element, &p_code_ean13[i], 1 );
		element[1] = '\0';

		somme = somme + (atoi(element) * 3);
 
	}
 
	modulo = somme % 10;
 
	if (modulo != 0){
		somme_de_controle = 10 - modulo;
	}

	char checksum[2];
	memcpy(checksum, &p_code_ean13[12], 1 );
	checksum[1] = '\0';

	if (somme_de_controle == atoi(checksum) ){
		return 1;
	} else {
		return 0;
	}
}



short int initialisation_liste_chaine(void){
	Liste *liste = malloc(sizeof(*liste));

    if (liste == NULL){
		return 0;
    }

    liste->debut = NULL;
    liste_course = liste;
    return 1;
}

short int Ajouter_produit_liste_chaine(const gchar *code_barres){
	/* Ajout d'un produit à la liste */
	 
	if( bVerification_somme_de_controle(code_barres) == 0 ) // verification de la validitée du code barres
	{
		return 0;
	}

	Produit *newproduit = mysql_recuperer_produit(code_barres); /* importation depuis la base de données */

	if( newproduit->produitid == 0 ) /* verifier que le produit existe dans la base de donnée */ 
	{
		return 0;
	}

	Element *actuel = liste_course->debut;
	while (actuel != NULL){
		if (actuel->p_produit->produitid == newproduit->produitid){
			actuel->quantitee++;
			return 1;
		} 
        actuel = actuel->suivant;
    }


	/* Création du nouvel élément */
    Element *nouveau = malloc(sizeof(*nouveau));
    if (nouveau == NULL){
        return 0;
    }

    nouveau->p_produit = newproduit;
   	nouveau->quantitee = 1;
    nouveau->suivant = liste_course->debut; /* Insertion de l'élément au début de la liste */
    liste_course->debut = nouveau;

    return 1;
}

short int Supprimer_produit_liste_chaine(short int place, short int nombres){ /* Suppression de produits à la liste */

    if( liste_course->debut != NULL ){

		Element *aSupprimer = liste_course->debut;
		Element *lienasupprimer;
		int i;
		int placemoins = place - 1; /* le premier element de la liste et "1" */

    	for( i = 0; i < placemoins; i++ ) /* recherche du pointeur en fonction de la place */
    	{	
    		lienasupprimer= aSupprimer;
    		aSupprimer = aSupprimer->suivant;
    	}

    	for (i = 0; i < nombres; i++) /* suppression des elements en fonction du nombre voulu */
    	{
    		free(aSupprimer);
    		if( aSupprimer->suivant!=NULL ) // résout les bugs de suppression en fin de chaine 
    		{
    			aSupprimer = aSupprimer->suivant;
    		} else {
    			aSupprimer = NULL;
    		}
    		 
    	}

    	if (place == 1) /* relinkage de la liste chainée */  
    	{
    		liste_course->debut= aSupprimer;
    	} else {
    		lienasupprimer->suivant= aSupprimer;
    	}
           
    }
return 1;
}


short int Supprimer_liste_chaine(){
	/* suppression de la liste */
	Element *actuel = liste_course->debut;
	while (actuel != NULL) /* On parcours la liste et on supprime chaque terme */ 
    {
        free(actuel);
        actuel = actuel->suivant;
    }

    liste_course->debut = NULL; /* relinkage de la liste chainée */
	return 1;
}

void afficherListe() { /* affiche la liste */
    Element *actuel = liste_course->debut;

    while (actuel != NULL )
    {
   printf("%d",actuel->p_produit->produitid);
        actuel = actuel->suivant;
        printf(" -> "  );
    }

    printf("NULL\n");
}


/*

int main(int argc, char const *argv[]){

	initialisation_liste();
	Ajouter_produit("3264420101830");
	Ajouter_produit("5410076769421");
	Ajouter_produit("4015400535737");
	Supprimer_produit(2,1);
	Supprimer_liste();
	afficherListe();
	return 0;
}*/
