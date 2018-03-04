#include "execution.h"
#include "commandes_internes.h"
#include "commandes_externes.h"

void execution_ligne_cmd(parse_info *info) {

  int i, j, n, nb_arg, nb_args_total;
  int resultat;

  i = 0;
  while (i<info->nb_arg) {

    strcpy(info->entree, "");
    strcpy(info->sortie, "");

    /* n aura la valeur de la fin de ligne de commande, ou du debut
     * de la prochaine commande a executer
     */
    n = detection_fin_commande(info, i);

    j = i;
    nb_arg = 0;
    while (j<n) {
      /* Attention cas non detecte arguments apr�s une redirection :
       * commande arg1 < redirection arg2
       * il s'agit a priori d'une erreur de syntaxe du shell
       */

      switch(info->modificateur[j]) {
        case REDIRECTION_ENTREE:
        strcpy(info->entree, info->ligne_cmd[j]);
        break;
        case REDIRECTION_SORTIE:
        strcpy(info->sortie, info->ligne_cmd[j]);
        break;
        case ARRIERE_PLAN:
        info->modificateur[i]=ARRIERE_PLAN;
        break;
        default:
        nb_arg++;
      }

      j++;
    }


    if(info->modificateur[j]==TUBE) {
      /* il faut traiter (par simplification uniquement pour deux commandes)
       * le cas de la communication via un tube
       */
      nb_args_total = detection_fin_commande(info, nb_arg);
      execution_cmd_tube(info, i, nb_arg, nb_args_total);
      return;
    }
    else {
      resultat = execution_cmd(info, i, nb_arg);
    }

    if(info->modificateur[j]==COMMENTAIRE) {
      /* il suffit de sortir de la fonction, on est arrive a un commentaire
       * le reste de la ligne de commande n'est pas a traiter
       */
      return;
    }

    /* a ce stade, on a le resultat de la commande, et on sait s'il faut traiter la commande
     * suivante en sequence ou avec une condition
     */

    /* traitement post-execution de la partie concernant inf->modificateur[j] != RIEN */
    switch(info->modificateur[j]) {
      case EXECUTION:
      break;
      case EXECUTION_SI:
      /* la/les commande suivante va �tre directement traitee ici :
       * i.e. ne sera pas executee
       */
        if(!resultat) {/* si on doit passer a la commande suivante... */
      while (j<info->nb_arg && \
        (info->modificateur[j]!=EXECUTION && \
          info->modificateur[j]!=EXECUTION_SINON)) {
        j++;
    }
  }
  break;
  case EXECUTION_SINON:
      /* la/les commande suivante va �tre directement traitee ici :
       * i.e. ne sera pas executee
       */
      if (resultat) { /* si on doit passer a la commande suivante... */
        j++;//S'il y a resultat, alors il ne faut pas exécuter le SINON, on passe à la commande suivante
      }
      break;
      default :
      /* cas impossible a priori */
      break;
    }

    info->modificateur[j] = AUTRE; /* il suffit d'executer la commande suivante en sequence */
    i = j;
  }
}

t_bool execution_cmd(parse_info *info, int debut, int nb_arg)
{
  if (EST_EGAL(info->ligne_cmd[debut], "echo")) {
    return ActionECHO(info, debut, nb_arg);
  }

  else if (EST_EGAL(info->ligne_cmd[debut], "cd")) {
    return ActionCD(info, debut, nb_arg);
  }

  else if (EST_EGAL(info->ligne_cmd[debut], "ls")) {
    return ActionLS(info, debut, nb_arg);
  }

  else if (EST_EGAL(info->ligne_cmd[debut], "set")) {
    return ActionSET(info, debut, nb_arg);
  }

  else {
    return ActionEXEC(info, debut, nb_arg);
  }
}

//Commande utile pour tester les tubes : "ls | sort"
t_bool execution_cmd_tube(parse_info *info, int debut, int nb_args_cmd_1, int nb_args_total) {
  pid_t pid_fils;
  int tuyaux[2];
  pipe(tuyaux);
  int sortieStandard = dup(1);
  int entreeStandard = dup(0);

  pid_fils = fork();

  if(pid_fils == -1) {
    printf("erreur dans le fork %d\n", errno);
  }
  else if(pid_fils != 0) {
    close(tuyaux[1]);
    //Redirection de l'entrée standard dans le tuyau pour
    //que la commande ait le résultat de l'autre commande en entrée
    dup2(tuyaux[0], 0);

    execution_cmd(info, nb_args_cmd_1, nb_args_total - nb_args_cmd_1);
    waitpid(pid_fils, NULL, 0);
  }
  else {
    close(tuyaux[0]);
    //Redirection de la sortie standard dans le tuyau
    //pour envoyer le résultat de l'exécution à l'autre processus
    dup2(tuyaux[1], 1);
    execution_cmd(info, debut, nb_args_cmd_1);
    exit(1);
  }


  dup2(sortieStandard, 1);
  dup2(entreeStandard, 0);

  return faux;
}