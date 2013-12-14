#ifndef STRUCTURES_H
#define STRUCTURES_H


typedef struct Produit /* structure du produit */
{
	int produitid;
	char *code_barres;
	char *marque;
	char *libelle;
	char *type_marque;
	char *conditionnement;
	int code_TVA;
	float prix;
}Produit;

typedef struct Element Element; /* structure de base le liste chainée */
struct Element
{
    Produit	*p_produit; /* pointeur vers la structure du produits */
    int		quantitee;
    Element	*suivant;
};
typedef struct Liste Liste; /* Structure de contrôle de la liste chainée*/
struct Liste
{
    Element *debut;
};


typedef struct TypeTauxTVA /* structure de la TVA */
{
	int		code;
	float	taux;
  float ht;
  float tva;
  float ttc;
}TauxTVA;


typedef struct ListeUtilisateurs
{
  const gchar	*id;
  const gchar	*utilisateur;
  const gchar	*motdepasse;
  const gchar 	*niveau;
}LISTEUTILISATEURS;

enum /*tree view partie encaissement */ 
{
  QUANTITES,
  MARQUE,
  LIBELLE,
  PRIX,
  N_COLUMNS
};


#endif /* STRUCTURES_H */
