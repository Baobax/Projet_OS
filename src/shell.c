#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "divers.h"
#include "execution.h"
#include "commandes_internes.h"
#include "commandes_externes.h"


int main (int argc, char *argv[]) {

  char ligne_cmd[CHAINE_MAX];
  char verOS[CHAINE_MAX];
  FILE* entree;
  int i;
  parse_info *info;
  t_bool mode_interactif = (argc==1);

  if (mode_interactif) {
    lire_variable ("OS", verOS, sizeof (verOS));
    printf ("imrShell - %s\n", (char *) verOS);
    printf ("Programmation Système Windows/Linux - IMR 1\n");
    fflush (stdout);
  }

  for(i=1; i<argc || mode_interactif; i++) {

    if (mode_interactif) {
      AfficheInvite ();
      entree = stdin;
    }
    else {
      /* Si le mode n'est pas interractif (il y a des arguments sur la ligne
       * de commande lors de l'appel au shell), alors il faut ouvrir les
       * fichiers en argument, un � un (argv[i]), puis interpreter leur contenu
       * dans la boucle suivante
       */
      (void) argv;
    }


    while ((fgets (ligne_cmd, sizeof (ligne_cmd), entree) != 0) && !COMMENCE_PAR(ligne_cmd, "exit")) {
      info = parse(ligne_cmd);
      execution_ligne_cmd(info);

      if (mode_interactif) {
       AfficheInvite ();
     }
   }

   mode_interactif = faux;
 }

 return EXIT_SUCCESS;
}
