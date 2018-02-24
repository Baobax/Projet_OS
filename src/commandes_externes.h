#ifndef _COMMANDES_EXTERNES_H_
#define _COMMANDES_EXTERNES_H_
#include "divers.h"
#include "parse.h"
#include <errno.h>
#include <sys/wait.h>

//Pour pouvoir utiliser la fonction open
//Il est requis d'utiliser la fonction open pour utiliser dup2
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


t_bool ActionEXEC (parse_info *info, int debut, int nbArg);

#endif
