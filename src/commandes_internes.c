#include "divers.h"
#include "commandes_internes.h"
#include "commandes_externes.h"

t_bool	ActionECHO (parse_info *info, int debut, int nbArg) {

  int i;
  FILE *sortie;
  /* si l'appel est correctement ecrit, on a :
   * arguments[0] == "echo"
   * arguments[1..nbArg-1] = "..."
   *
   * Pas de lecture, mais une ecriture redirigee possible
   */

  sortie=stdout;

  i = 1;
  while(i<nbArg)	{
    fprintf(sortie, "%s ", info->ligne_cmd[debut+i]);
    i++;
  }
  printf("\n");

  return vrai;
}

t_bool	ActionSET (parse_info *info, int debut, int nbArg) {

  /* si l'appel est correctement ecrit, on a :
   * arguments[0] == "set"
   * arguments[1] == "nom_variable"
   * arguments[2] == "="
   * arguments[3] == "valeur"
   * nbArg == 4
   *
   * ou
   * arguments[0] == "set"
   * arguments[1] == "nom_variable"
   * arguments[2] == "="
   * nbArg == 3
   *
   */

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
  } else {

    strcpy(dirName,"");
    for(i=1; i<nbArg; i++) {
      if(strlen(dirName)!=0) strcat(dirName," ");
      strcat(dirName, info->ligne_cmd[debut+i]);
    }

    if (chdir (dirName) == -1) {
      printf ("Impossible de changer vers le repertoire '%s'\n", dirName);
      return faux;
    }
  }
  return vrai;

}


t_bool	ActionLS (parse_info *info, int debut, int nbArg) {

  /* Utilisation des parametres */
  (void) info;
  (void) debut;
  (void) nbArg;

  if (nbArg == 2 ){

    struct dirent *lecture;
    DIR *repertoire;
    repertoire = opendir(info->ligne_cmd[1]);
    while ((lecture = readdir(repertoire))) {
        printf("%s\n", lecture->d_name);
    }
    closedir(repertoire);
  }

  else{
    ActionEXEC(info, debut, nbArg);
  }

  // printf("Appel a actionLS (%s %d) a ecrire.\n",
	//  _FILE_,
	//  _LINE_);

  return faux;
}
