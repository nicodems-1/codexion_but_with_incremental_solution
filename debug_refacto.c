#include "include.h"


int ft_uslepp()
{

    return(0);
}
int debug(t_coder *coder)
{
    usleep(coder->param->time_to_debug);
    print_logs("is debugging", coder);
    return(0);
}
int refactor(t_coder *coder)
{
    usleep(coder->param->time_to_refactor);
    print_logs("is refactoring", coder);
    return(0);
}