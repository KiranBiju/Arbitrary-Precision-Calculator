#ifndef APC_H
#define APC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    FAILURE = 0,
    SUCCESS = 1
} Status;

typedef struct node
{
    int data;
    struct node *prev;
    struct node *next;
} Dlist;

/* List wrapper */
typedef struct
{
    Dlist *head;
    Dlist *tail;
    int count;
} List;

Status insert_at_first(List *list, int digit);
Status insert_at_last(List *list, int digit);
void create_list(List *list);
void delete_list(List *list);
void print_list(const List *list);

/* Number conversion and validation */
Status validate_number(const char *str);
Status string_to_list(const char *str, List *list);

int compare_lists(const List *list1, const List *list2);
void remove_leading_zeroes(List *list);

Status addition(const List *op1, const List *op2, List *result);

Status subtraction(
    const List *op1,
    const List *op2,
    List *result
);

Status multiplication(
    const List *op1,
    const List *op2,
    List *result
);

Status division(
    const List *dividend,
    const List *divisor,
    List *quotient,
    List *remainder
);

Status parse_signed_number(
    const char *input,
    const char **digits,
    int *sign
);

int add_sub_result_sign(
    char operator,
    int sign1,
    int sign2,
    int comparison,
    char *magnitude_operation
);


int product_quotient_sign(int sign1, int sign2);

#endif
