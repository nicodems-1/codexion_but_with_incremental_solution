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

int main(int ac, char **av)
{
    int i = 1;
    printf("Testing Programm functionnality\n");
    if (ac != 10)
    {
        printf("\nArgs count must be 9, current args count = %d\n\n", ac-1);
        exit(1);
    }
    while(i != 9)
    {
        if (valid_length(av[i]) == 1)
        {
            printf("Arg[%d]: \"%s\" length is too long", i, av[i]);
            exit(1);
        }
        if (ft_is_number(av[i]) == 1)
        {
            printf("Arg[%d]: \"%s\" is not a number", i, av[i]);
            exit(1);
        }
        i++;
    }
}