#ifndef MAIN_GESTION_LISTE_H
#define MAIN_GESTION_LISTE_H


int bVerification_somme_de_controle(const gchar *p_code_ean13);
short int initialisation_liste(void);
short int Ajouter_produit(const gchar *code_barres);
short int Supprimer_produit(short int place, short int nombres);
short int Supprimer_liste();
void afficherListe();


#endif /* MAIN_GESTION_LISTE_H */
