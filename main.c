
#include "apc.h"

int main(int argc, char *argv[])
{
    List operand1, operand2;
    List result, remainder;

    const char *num1;
    const char *num2;

    int sign1, sign2;
    int result_sign;

    int comparison;
    char magnitude_operation;

    char operator;

    /* Check command-line argument count */
    if (argc != 4)
    {
        printf("Usage: ./a.out <operand1> <operator> <operand2>\n");
        printf("Example: ./a.out 12345 + 6789\n");
        return FAILURE;
    }

    /* Check operator */
    if (strlen(argv[2]) != 1)
    {
        printf("Error: Invalid operator.\n");
        return FAILURE;
    }

    operator = argv[2][0];

    if (operator != '+' && operator != '-' &&
        operator != 'x' && operator != '/')
    {
        printf("Error: Unsupported operator.\n");
        return FAILURE;
    }

    create_list(&operand1);
    create_list(&operand2);
    create_list(&result);
    create_list(&remainder);

    if (parse_signed_number(argv[1], &num1, &sign1) == FAILURE)
    {
        printf("Error: Invalid first operand.\n");
        return FAILURE;
    }

    if (parse_signed_number(argv[3], &num2, &sign2) == FAILURE)
    {
        printf("Error: Invalid second operand.\n");
        return FAILURE;
    }

    if (validate_number(num1) == FAILURE || validate_number(num2) == FAILURE)
    {
        printf("Error: Operands must contain only digits.\n");
        return FAILURE;
    }

    if (string_to_list(num1, &operand1) == FAILURE || string_to_list(num2, &operand2) == FAILURE)
    {
        printf("Error: Memory allocation failed.\n");

        delete_list(&operand1);
        delete_list(&operand2);
        delete_list(&result);
        delete_list(&remainder);

        return FAILURE;
    }

    remove_leading_zeroes(&operand1);
    remove_leading_zeroes(&operand2);

    switch (operator)
    {
        case '+':

        case '-':
        {
            comparison = compare_lists(&operand1, &operand2);

            result_sign = add_sub_result_sign(
                operator,
                sign1,
                sign2,
                comparison,
                &magnitude_operation
            );

            if (magnitude_operation == '+')
            {
                if (addition(&operand1, &operand2, &result) == FAILURE)
                {
                    printf("Error during addition.\n");
                    goto clear;
                }
            }
            else
            {
                if (comparison >= 0)
                {
                    if (subtraction(&operand1, &operand2, &result) == FAILURE)
                    {
                        printf("Error during subtraction.\n");
                        goto clear;
                    }
                }
                else
                {
                    if (subtraction(&operand2, &operand1, &result) == FAILURE)
                    {
                        printf("Error during subtraction.\n");
                        goto clear;
                    }
                }
            }

            break;
        }

        case 'x':
        {
            result_sign = product_quotient_sign(sign1, sign2);

            if (multiplication(&operand1, &operand2, &result) == FAILURE)
            {
                printf("Error during multiplication.\n");
                goto clear;
            }

            break;
        }

        case '/':
        {
            if (operand2.count == 1 && operand2.head->data == 0)
            {
                printf("Error: Division by zero is not allowed.\n");
                goto clear;
            }

            result_sign = product_quotient_sign(sign1, sign2);

            if (division(&operand1, &operand2,
                         &result, &remainder) == FAILURE)
            {
                printf("Error during division.\n");
                goto clear;
            }

            break;
        }

        default:
            printf("Invalid operator.\n");
            goto clear;
    }

    printf("Result: ");

    if (!(result.count == 1 && result.head->data == 0))
    {
        if (result_sign < 0)
            printf("-");
    }

    print_list(&result);
    printf("\n");

    /* Print remainder for division */
    if (operator == '/')
    {
        printf("Remainder: ");

        if (!(remainder.count == 1 && remainder.head->data == 0))
        {
            if (sign1 < 0)
                printf("-");
        }

        print_list(&remainder);
        printf("\n");
    }

clear:

    delete_list(&operand1);
    delete_list(&operand2);
    delete_list(&result);
    delete_list(&remainder);

    return SUCCESS;
}
