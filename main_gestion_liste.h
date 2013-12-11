#ifndef MAIN_GESTION_LISTE_H
#define MAIN_GESTION_LISTE_H


int bVerification_somme_de_controle(const gchar *p_code_ean13);
short int initialisation_liste_chaine(void);
short int Ajouter_produit_liste_chaine(const gchar *code_barres);
short int Supprimer_produit_liste_chaine(short int place, short int nombres);
short int Supprimer_liste_chaine();
void afficherListe();


#endif /* MAIN_GESTION_LISTE_H */
