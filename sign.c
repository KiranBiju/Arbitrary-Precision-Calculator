#include "apc.h"

Status parse_signed_number(
    const char *input,
    const char **digits,
    int *sign)
{
    int i = 0;

    if (input == NULL || digits == NULL || sign == NULL)
        return FAILURE;

    if (input[0] == '\0')
        return FAILURE;

    *sign = 1;

    if (input[0] == '+')
    {
        *sign = 1;
        i++;
    }
    else if (input[0] == '-')
    {
        *sign = -1;
        i++;
    }

    if (input[i] == '\0')
        return FAILURE;

    *digits = &input[i];

    while (input[i] != '\0')
    {
        if (input[i] != '0')
            return SUCCESS;

        i++;
    }

    *sign = 1;

    return SUCCESS;
}

int add_sub_result_sign(
    char operator,
    int sign1,
    int sign2,
    int comparison,
    char *magnitude_operation)
{
    int effective_sign2;

    if (operator == '+')
    {
        effective_sign2 = sign2;
    }
    else if (operator == '-')
    {
        effective_sign2 = -sign2;
    }
    else
    {
        *magnitude_operation = '+';
        return 1;
    }

    if (sign1 == effective_sign2)
    {
        *magnitude_operation = '+';

        return sign1;
    }


    *magnitude_operation = '-';

    if (comparison == 0)
    {
        return 1;
    }

    if (comparison > 0)
    {
        return sign1;
    }
    else
    {
        return effective_sign2;
    }
}


int product_quotient_sign(int sign1, int sign2)
{
    if (sign1 == sign2)
        return 1;

    return -1;
}
