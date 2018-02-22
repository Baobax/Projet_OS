#ifndef _COMMANDES_EXTERNES_H_
#define _COMMANDES_EXTERNES_H_
#include "divers.h"
#include "parse.h"
#include <errno.h>
#include <sys/wait.h>

t_bool ActionEXEC (parse_info *info, int debut, int nbArg);

#endif
