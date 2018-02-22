#ifndef _PARSE_H_
#define _PARSE_H_

#include "divers.h"

typedef enum{AUTRE=0, /* le =0 est important, ainsi un appel a bzero rend toute la structure egale a AUTRE */
	REDIRECTION_ENTREE,
	REDIRECTION_SORTIE_AJOUT,
	REDIRECTION_SORTIE,
	EXECUTION,
	EXECUTION_SI,
	EXECUTION_SINON,
	TUBE,
	ARRIERE_PLAN,
	VARIABLE,
	COMMENTAIRE}
t_symbole;

typedef struct {
	int nb_arg;		/* nombre d'arguments sur la ligne (nom des commandes + leurs arguments) */
	char ligne_cmd[ARG_MAX][CHAINE_MAX];	/* la ligne de commande decoupee suivant les espaces */
	t_symbole modificateur[ARG_MAX];	/* la semantique associee a chaque token de ligne_cmd */
	char entree[MAX_PATH];	/* ces champs servent a passer une eventuelle redirection a la fonction */
	char sortie[MAX_PATH];	/* d'execution d'une commande */
} parse_info;

parse_info *parse(char *ligne_cmd);
void affiche_parse_info(parse_info *info);
void affiche_commande(parse_info *info, int debut);
int detection_fin_commande(parse_info *info, int debut);


#endif
