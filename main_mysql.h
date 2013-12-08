#ifndef MYSQL_H
#define MYSQL_H

#include <my_global.h>
#include <mysql.h>
#include <glib.h>

#include "structures.h"


void finish_with_error();
void connect_to_mysql();
int mysql_verifier_identification(const gchar *utilisateur, const gchar *motdepasse);
const gchar *mysql_niveau_utilisateur(const gchar *utilisateur, const gchar *motdepasse);
int mysql_verifier_existence_produit(const gchar *code_barres);

struct Produit *mysql_recuperer_produit(const gchar *code_barres);

int mysql_nombre_taux_tva();
struct TauxTVA *mysql_recuperer_taux_tva();

// Gestion utilisateurs
int mysql_nombre_utilisateurs();
LISTEUTILISATEURS *mysql_recuperer_utilisateurs();
int mysql_utilisateur_ajouter(const gchar *utilisateur, const gchar *motdepasse, const gchar *niveau);
int mysql_utilisateur_supprimer(const gchar *id_utilisateur);
int mysql_utilisateur_modifier(const gchar *id_utilisateur, const gchar *nom_utilisateur, const gchar *mot_de_passe, const gchar *niveau);

#endif /* MYSQL_H */
