Pour lancer le programme :
	- Se placer à la racine du projet et lancer la commande "make" dans le terminal pour compiler
	- Se placer dans le dossier "bin" et exécuter le programme en écrivant "./shell"


-------------------------------------------------------------------------------------------------


Quelques commandes pour effectuer des tests :

- Pour les variables d'environnement :

set var = test
echo %var
echo %LANGUAGE


- Pour l'exécution d'une commande interne :

ls
ls /
ls -al


- Pour l'exécution d'une commande externe :

wc Makefile


- Pour l'exécution d'une commande externe au premier plan :

gedit
& gedit


- Pour la redirection en entrée et sortie de commandes internes :

echo test > resultat.txt


- Pour la redirection en entrée et sortie de commandes externes :

wc < Makefile
wc Makefile > resultat.txt


- Pour l'exécution de scripts :

imrshell script1.imr script2.imr


- Pour l'enchaînement de commandes :

cd repertoirebidon && echo ok || echo ko
cd / && echo ok || echo ko
echo ok && ls


- Pour les tubes :

ls | sort
ls -al | sort