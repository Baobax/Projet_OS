#include "divers.h"
#include "commandes_internes.h"
#include "commandes_externes.h"

t_bool	ActionECHO (parse_info *info, int debut, int nbArg) {
  int i;
  FILE *sortie;

  if (!EST_EGAL(info->sortie, "")) {
    sortie=fopen(info->sortie,"w");

    if (sortie==NULL) {
      /* Traitement du cas où le fichier n’est pas accessible en écriture */
      printf("Impossible d'accéder en écriture\n");
      return faux;
    }
  }
  else {
    sortie=stdout;
  }


  i = 1;
  while(i<nbArg)	{
    fprintf(sortie, "%s ", info->ligne_cmd[debut+i]);
    i++;
  }

  if (! EST_EGAL(info->sortie, "")) {
    fclose(sortie);
  }
  else {
    printf("\n");
  }

  return vrai;
}

t_bool	ActionSET (parse_info *info, int debut, int nbArg) {
  if (!EST_EGAL(info->ligne_cmd[debut], "set") && !EST_EGAL(info->ligne_cmd[debut+2], "=")) {
    printf("Impossible de mettre a jour la variable, appel incorrect\n");
    return faux;
  }

  if (nbArg == 4)	{
    return ecrire_variable (info->ligne_cmd[debut+1], info->ligne_cmd[debut+3]);
  }
  else if (nbArg == 3) {
    return ecrire_variable (info->ligne_cmd[debut+1], NULL);
  }
  else {
    return faux;
  }
}

t_bool ActionCD (parse_info *info, int debut, int nbArg) {
  char def[MAX_PATH];
  char dirName[MAX_PATH];
  int i;

  /* le cas nbArg==1 a un comportement par defaut qui mene au
   * repertoire HOME utilisateur
   *
   * Si nbArg>=1 il faut aggreger tous les arguments en un seul chemin
   * (le repertoire de destination contient des espaces)
   */
  if (nbArg == 1) {
    lire_variable ("USERPROFILE", def, sizeof (def));

    if (chdir (def) == -1) {
      printf ("Impossible de changer vers le repertoire '%s' \n", def );
      return faux;
    }
  }
  else {
    strcpy(dirName,"");

    for(i=1; i<nbArg; i++) {
      if(strlen(dirName)!=0) strcat(dirName," ");
      strcat(dirName, info->ligne_cmd[debut+i]);
    }

    if (chdir(dirName) == -1) {
      printf("Impossible de changer vers le repertoire '%s'\n", dirName);
      return faux;
    }
  }

  return vrai;
}


t_bool	ActionLS (parse_info *info, int debut, int nbArg) {
  if(nbArg == 1) {
    struct dirent *lecture;
    DIR *repertoire;
    repertoire = opendir(".");

    if(repertoire != NULL) {
      while ((lecture = readdir(repertoire))) {
        printf("%s\n", lecture->d_name);
      }
      closedir(repertoire);
    }
  }
  else if (nbArg == 2){
    struct dirent *lecture;
    DIR *repertoire;
    repertoire = opendir(info->ligne_cmd[debut + 1]);

    if(repertoire != NULL) {
      while ((lecture = readdir(repertoire))) {
        printf("%s\n", lecture->d_name);
      }
      closedir(repertoire);
    }
    //Si on n'a pas pu ouvrir le répertoire, alors le deuxième argument est peut être autre chose qu'un chemin
    else {
      ActionEXEC(info, debut, nbArg);
    }
  }
  else {
    ActionEXEC(info, debut, nbArg);
  }

  return faux;
}
