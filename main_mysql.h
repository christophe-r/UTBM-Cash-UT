#ifndef MYSQL_H
#define MYSQL_H

#include <my_global.h>
#include <mysql.h>
#include <glib.h>

#include "structures.h"


// Interface
void finish_with_error();
void connect_to_mysql();

// Identification
int mysql_verifier_identification(const gchar *utilisateur, const gchar *motdepasse);
const gchar *mysql_niveau_utilisateur(const gchar *utilisateur, const gchar *motdepasse);
const gchar *mysql_num_utilisateur(const gchar *utilisateur, const gchar *motdepasse);

// Produits
int mysql_verifier_existence_produit(const gchar *code_barres);
Produit *mysql_recuperer_produit(const gchar *code_barres);
int mysql_produit_ajouter(const gchar *code_barres, const gchar *marque, const gchar *libelle, const gchar *type_marque, const gchar *conditionnement, const gchar *code_tva, const gchar *prix);
int mysql_produit_modifier(const gchar *code_barres, const gchar *marque, const gchar *libelle, const gchar *type_marque, const gchar *conditionnement, const gchar *code_tva, const gchar *prix);
int mysql_produit_supprimer(const gchar *code_barres);
int mysql_nombre_rechercher_produits(const gchar *critere, const gchar *recherche);
Produit *mysql_rechercher_produits(const gchar *critere, const gchar *recherche);


// TVA
int mysql_nombre_taux_tva();
TauxTVA *mysql_recuperer_taux_tva();

// Gestion utilisateurs
int mysql_nombre_utilisateurs();
LISTEUTILISATEURS *mysql_recuperer_utilisateurs();
int mysql_utilisateur_ajouter(const gchar *utilisateur, const gchar *motdepasse, const gchar *niveau);
int mysql_utilisateur_supprimer(const gchar *id_utilisateur);
int mysql_utilisateur_modifier(const gchar *id_utilisateur, const gchar *nom_utilisateur, const gchar *mot_de_passe, const gchar *niveau);


// Encaissement
int mysql_encaissement_facturer(const gchar *date, const gchar *num_caisse, const gchar *num_utilisateur, const gchar *facture, float total_prix);

// Statistiques
int mysql_nombre_ventes_aujourdhui(const gchar *num_utilisateur);
float mysql_total_encaisse_aujourdhui(const gchar *num_utilisateur);
int mysql_nombre_produits();
const gchar *mysql_derniere_vente();


#endif /* MYSQL_H */
