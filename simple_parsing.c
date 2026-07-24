#include <stdio.h>
#include <stdlib.h>

typedef struct s_param
{
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char *scheduler;
}   t_param;

int valid_length(char *number)
{
    int i = 0;
    while (number[i])
        i++;
    if (i > 10)
        return (1);
    return(0);
}

int ft_is_number(char *number)
{
    int i = 0;
    while(number[i] != 0)
    {
        if ((number[i] > '9' || number[i] < '0'))
            return(1);
        i++;
    }
    return(0);
}

int update_struct(char **parsed, t_param *param)
{
    int i = 0;
    int converted[8];
    while(i < 9)
    {
        if ((converted[i-1] = atoi(parsed[i])) < 0)
            return(i);
        i++;
    }
    param->number_of_coders = converted[0];
    param->time_to_burnout = converted[1];
    param->time_to_compile = converted[2];
    param->time_to_debug = converted[3];
    param->time_to_refactor = converted[4];
    param->number_of_compiles_required = converted[5];
    param->dongle_cooldown = converted[6];
    param->dongle_cooldown = converted[7];
    return(0);
}

int basic_parsing(char **av)
{
    int i = 1;
    while(i != 9)
    {
        if (valid_length(av[i]) == 1)
        {
            printf("Arg[%d]: \"%s\" length is too long", i, av[i]);
            return(1);
        }
        if (ft_is_number(av[i]) == 1)
        {
            printf("Arg[%d]: \"%s\" is not a number", i, av[i]);
            return(1);
        }
        i++;
    }
    return(0);
}

int main(int ac, char **av)
{
    int index;
    t_param *param;

    param = malloc(sizeof(t_param));
    if (ac != 10)
    {
        printf("\nArgs count must be 9, current args count = %d\n\n", ac-1);
        exit(1);
    }
    if (basic_parsing(av) == 1)
        exit(1);
    if ((index = update_struct(av, param)) != 0)
        printf("Error, arg[%d]: \"%s\" bigger than int max", index, av[index]);
}