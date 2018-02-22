#ifndef _EXECUTION_H_
#define _EXECUTION_H_

#include "divers.h"
#include "parse.h"

void execution_ligne_cmd(parse_info *info);
t_bool execution_cmd(parse_info *info, int debut, int nb_arg);

#endif
