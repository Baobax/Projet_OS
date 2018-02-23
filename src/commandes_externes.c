#include "divers.h"
#include "commandes_externes.h"

t_bool	ActionEXEC (parse_info *info, int debut, int nbArg) {
	char* ligne[CHAINE_MAX];
	t_bool premierPlan;
	int i;


	for(i=0; i<nbArg; i++) {
		strcpy(ligne[i], info->ligne_cmd[debut+i]);
	}
	ligne[i] = NULL;

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
		execvp(info->ligne_cmd[0], ligne);
	}

	return faux;
}
