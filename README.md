Cash'UT
=======

Logiciel de caisse enregistreuse en C / GTK+3.0

Adresse du Wiki : http://if20.christophe-ribot.fr/

Ce logiciel a été crée à l'UTBM dans le cadre de l'unité de valeur IF20 enseignée par Eric Bachard.

Les membres du groupe sont :
 - Anissa Bessahraoui
 - Maxence Chaudey
 - Mathieu Lacroix
 - Romain Lambert
 - Vincent Marseguerra
 - Christophe Ribot
 - Niels Suzanne 


Ce logiciel est placé sous la licence GPLv3 : http://www.gnu.org/licenses/gpl.html


=======
Instructions d'installation :

* Faire un "git clone" ou bien télécharger l'archive ZIP du Git.
* Installer les paquets suivants : gtk+-3.0 mysql-server libmysqlclient18 libmysqlclient-dev
* Installer PhpMyAdmin ou MysqlWorkbench par exemple (ou directement en console via la commande "mysql") afin de créer un utilisateur et une base de données associée. (L'utilisateur créé doit avoir les pleins droits sur sa base de données)
* Importer le fichier ./DATABASE-DUMP.sql (présent sur ce Git) dans la base de données créée.
* Configurer les accès à la base de données dans le fichier de configurations qui se trouve à cet endroit : ./Cash'UT/config.ini (ce fichier de configurations est relu à chaque lancement du logiciel, cette étape peut donc éventuellement se faire après la compilation)
* Compiler le logiciel en faisant "make"
* Le lancer en faisant ./cashut ou bien en double-cliquant sur le binaire créé.
* Se connecter à l'aide d'un des identifiants suivants : christophe, vincent, niels, romain, maxence, mathieu, anissa. Et le mot de passe : 1234

