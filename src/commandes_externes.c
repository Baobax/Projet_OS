#include "divers.h"
#include "commandes_externes.h"

t_bool	ActionEXEC (parse_info *info, int debut, int nbArg) {
	char* ligne[CHAINE_MAX];
	t_bool premierPlan;
	int i;
	int sortie, entree;
	//Ces deux variables pour pouvoir remettre la bonne entrée ou sortie après avoir exécuté le code
	int sortieStandard = dup(1);
	int entreeStandard = dup(0);


	for(i=0; i < nbArg; i++) {
		ligne[i] = (char*) malloc(CHAINE_MAX);
		strcpy(ligne[i], info->ligne_cmd[debut+i]);
	}
	//Pour marquer la fin des arguments
	ligne[i] = NULL;


	//On regarde s'il est demandé de faire une redirection d'entrée ou de sortie
	if (! EST_EGAL(info->sortie, "")) {
		sortie = open(info->sortie, O_WRONLY | O_CREAT, 0666);

		if (sortie==-1) {
  			/* Traitement du cas où le fichier n’est pas accessible en écriture */
			printf("Impossible d'accéder en écriture\n");
			return faux;
		}
		// Remplace la sortie standard dont le numero de descripteur est 1
		dup2(sortie, 1);
	}
	else if (! EST_EGAL(info->entree, "")) {
		entree = open(info->entree, O_RDONLY);

		if (entree==-1) {
  			/* Traitement du cas où le fichier n’est pas accessible en lecture */
			printf("Impossible d'accéder en lecture\n");
			return faux;
		}
		//Remplace l'entrée standard dont le numero de descripteur est 0
		dup2(entree, 0);
	}

	premierPlan = (info->modificateur[debut]!=ARRIERE_PLAN);

	pid_t pid_fils = fork();

	if (pid_fils == -1) {
		printf("erreur dans le fork %d\n", errno);
	}
	else if (pid_fils != 0) {
		if(premierPlan) {
			wait(NULL);
		}
	}
	else {
		execvp(info->ligne_cmd[debut], ligne);
	}



	if (! EST_EGAL(info->sortie, "")) {
		//On remet la sortie standard
		dup2(sortieStandard, 1);
		close(sortie);
	}
	else if (! EST_EGAL(info->entree, "")) {
		//On remet l'entrée standard
		dup2(entreeStandard, 0);
		close(entree);
	}

	return faux;
}
