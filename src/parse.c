#include <stdio.h>
#include <stdlib.h>
#include "divers.h"
#include "parse.h"

#define SEPARATEUR " \n"

parse_info *parse(char *ligne_cmd)
{
	parse_info *info;
	char *tok;
	int i;

	/* L'appel calloc initialise à zéro la structure de données,
	 * ainsi on est certain d'avoir des 'AUTRE' dans tout le tableau
	 * modificateur.
	 * La même remarque est valable pour les chemin entree et sortie. 
	 */
	info = calloc(1, sizeof(parse_info));

	tok = strtok(ligne_cmd, SEPARATEUR);
	i = 0;
	while (tok)
	{
		if (EST_EGAL(tok, "<"))
		{
			info->modificateur[i] = REDIRECTION_ENTREE;
		}
		else if (EST_EGAL(tok, ">>"))
		{
			info->modificateur[i] = REDIRECTION_SORTIE_AJOUT;
		}
		else if (EST_EGAL(tok, ">"))
		{
			info->modificateur[i] = REDIRECTION_SORTIE;
		}
		else if (EST_EGAL(tok, ";"))
		{
			info->modificateur[i] = EXECUTION;
		}
		else if (EST_EGAL(tok, "&&"))
		{
			info->modificateur[i] = EXECUTION_SI;
		}
		else if (EST_EGAL(tok, "||"))
		{
			info->modificateur[i] = EXECUTION_SINON;
		}
		else if (EST_EGAL(tok, "|"))
		{
			info->modificateur[i] = TUBE;
		}
		else if (EST_EGAL(tok, "&"))
		{
			info->modificateur[i] = ARRIERE_PLAN;
		}
		else if (EST_EGAL(tok, "#"))
		{
			/* Attention pour la prise en compte du commentaire : 
			 * ... # commande en arriere plan
			 * le commentaire doit être attaché à la suite
			 */
			i++;
			info->modificateur[i] = COMMENTAIRE;
			strcpy(info->ligne_cmd[i], ""); /* on copie par défaut une chaine vide qui sera remplacée dans les itérations suivantes */
		}
		else if (COMMENCE_PAR(tok, "#"))
		{
			/* Attention pour la prise en compte du commentaire : 
			 * ... #commande en arriere plan
			 * le commentaire doit être attaché au token courant
			 */
			i++;
			info->modificateur[i] = COMMENTAIRE;
			strcpy(info->ligne_cmd[i], &(tok[1]));
		}
		else if (COMMENCE_PAR(tok, "%"))
		{
			/* Comme la substitution est faite directement, il est en réalité inutile
			 * de marquer la chaîne comme une variable.
			 * Ce ne serait pas le cas si on choisissait de faire la substitution de
			 * la variable par sa valeur lors de l'interprétation de la commande
			 */

			//info->modificateur[i] = VARIABLE;
			lire_variable(&(tok[1]), info->ligne_cmd[i], sizeof(info->ligne_cmd[i]));
			i++;
		}
		else
		{
			strcpy(info->ligne_cmd[i], tok);
			if (info->modificateur[i] != COMMENTAIRE) /* pour le commentaire l'incrément de i est déjà réalisé */
				i++;
		}

		tok = strtok(NULL, SEPARATEUR);
	}           

	info->nb_arg = i;

	return info;
}

int detection_fin_commande(parse_info *info, int debut)
{
	int i;

	i = debut+1;
	while (i<info->nb_arg && \
			(info->modificateur[i]!=EXECUTION && \
					info->modificateur[i]!=EXECUTION_SI && \
					info->modificateur[i]!=EXECUTION_SINON && \
					info->modificateur[i]!=TUBE && \
					info->modificateur[i]!=COMMENTAIRE))
	{
		/* Les seuls éléments pouvant apparaître entre deux commandes
		 * sont ceux d'exécution conditionnelle, de tube ou de
		 * commentaire (ce qui vient après n'étant plus une commande)
		 */
		i++;
	}
	return i;
}

void affiche_parse_info(parse_info *info)
{
	int i;
	i = 0;

	while (i<info->nb_arg)
	{
		affiche_commande(info, i);
		i = detection_fin_commande(info, i);

		/* Les seuls éléments pouvant apparaître entre deux commandes
		 * sont ceux d'exécution conditionnelle, de tube ou de
		 * commentaire (ce qui vient après n'étant plus une commande)
		 */
		if (i<info->nb_arg && info->modificateur[i]==EXECUTION)
		{
			printf(";");
		} else if (i<info->nb_arg && info->modificateur[i]==EXECUTION_SI)
		{
			printf("&&");
		} else if (i<info->nb_arg && info->modificateur[i]==EXECUTION_SINON)
		{
			printf("||");
		} else if (i<info->nb_arg && info->modificateur[i]==TUBE)
		{
			printf("|");
		} else if (i<info->nb_arg && info->modificateur[i]==COMMENTAIRE)
		{
			printf("#");
		}
	}
	printf("\n");
}

void affiche_commande(parse_info *info, int debut)
{
	int i, n;

	n = detection_fin_commande(info, debut);

	i = debut;
	while (i<n)
	{
		/* Les seuls éléments pouvant apparaître dans une commande
		 * sont ceux de redirection ou d'exécution en arrière-plan
		 */
		if (info->modificateur[i]==REDIRECTION_ENTREE)
		{
			printf("<");
		} else if (info->modificateur[i]==REDIRECTION_SORTIE)
		{
			printf(">");
		} else if (info->modificateur[i]==REDIRECTION_SORTIE_AJOUT)
		{
			printf(">>");
		} else if (info->modificateur[i]==ARRIERE_PLAN)
		{
			printf("&");
		}
		printf(" '%s' ", info->ligne_cmd[i]);

		i++;
	}
}
