#include "divers.h"

void AfficheInvite() {

  char invite[CHAINE_MAX];
  char chaine[CHAINE_MAX];
  char var[CHAINE_MAX];
  size_t i;
  char * dwRet;

  if(lire_variable ("INV", invite, sizeof (invite))) {

    /* Format reconnu :
    * \u : remplace par l'utilisateur
    * \h : remplace par le nom de machine
    * \p : remplace par le chemin courant
    */

    strcpy(chaine, "");
    for(i=0; i<strlen(invite); i++) {
      if (invite[i]=='\\' && i<strlen(invite)-1) {
        i = i+1;

        switch(invite[i]) {
          case 'u' :
          lire_variable ("USERNAME", var, sizeof (var));
          break;

          case 'h' :
          lire_variable ("COMPUTERNAME", var, sizeof (var));
          break;

          case 's' :
          strcpy(var," ");
          break;

          case 'p' :
          dwRet = getcwd (var, sizeof (var));

          if (dwRet == NULL) {
            fprintf (stderr, "Echec lors de l'appel a getcwd !\n");
            fflush (stderr);
          }

          break;
          default :
          /* cas impossible a priori */
          break;
        }

      }

      else {
        sprintf(var, "%c", invite[i]);
      }

      strncat(chaine, var, CHAINE_MAX - strlen(chaine));
    }

  }

  else {
    strcpy(chaine, "$ ");
  }

  printf ("%s", chaine);
  fflush (stdout);
}

t_bool ecrire_variable (char* nomVar, char* valeur) { //setenv
  if(nomVar != NULL) {
    if(valeur != NULL) {
      setenv(nomVar,valeur,1);
    }
    else {
      setenv(nomVar,"",1);
    }
    return vrai;
  }

  return faux;
}

//Pour tester une variable du système : "echo %LANGUAGE"
t_bool lire_variable (char* nomVar, char* valeur, int taille) { //getenv
  strcpy(valeur, "");

  if(nomVar != NULL && taille > 0){
    char *variable = getenv(nomVar);
    
    if (variable != NULL) {
      strcpy(valeur, variable);
      return vrai;
    }
  }

  return faux;
}
