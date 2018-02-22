#include "divers.h"
#include "commandes_externes.h"

t_bool	ActionEXEC (parse_info *info, int debut, int nbArg) {
  char ligne[CHAINE_MAX];
  t_bool premierPlan;
  int i;

  // pid_t pid_fils = fork();
  //
  // if (pid_fils == -1) {
  //
  //     printf("erreur dans le fork %d\n", errno);
  //
  // }
  //
  // else if (pid_fils != 0) {
  //
  //     execvp(nbArg[1], nbArg[1]);
  //
  // }
  //
  // else {
  //
  //   execvp(nbArg[1], nbArg[1]);
  //   wait(NULL);
  // }

  strcpy(ligne, "");
  for(i=0; i<nbArg; i++) {

    if(strlen(ligne)!=0) {
      strcat(ligne," ");
    }
    strcat(ligne, info->ligne_cmd[debut+i]);
  }

  premierPlan = (info->modificateur[debut]!=ARRIERE_PLAN);

  printf("execution d'une commande externe  (%s %d) a ecrire :\n%s\n", __FILE__, __LINE__, ligne);
  (void) premierPlan;

  return faux;
}
