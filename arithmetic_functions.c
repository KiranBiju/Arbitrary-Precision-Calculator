#include "apc.h"

void create_list(List *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

Status insert_at_first(List *list, int digit)
{
    Dlist *new = malloc(sizeof(Dlist));

    if (new == NULL)
        return FAILURE;

    new->data = digit;
    new->prev = NULL;
    new->next = list->head;

    if (list->head == NULL)
    {
        list->head = new;
        list->tail = new;
    }
    else
    {
        list->head->prev = new;
        list->head = new;
    }

    list->count++;

    return SUCCESS;
}

Status insert_at_last(List *list, int digit)
{
    Dlist *new = malloc(sizeof(Dlist));

    if (new == NULL)
        return FAILURE;

    new->data = digit;
    new->next = NULL;
    new->prev = list->tail;

    if (list->tail == NULL)
    {
        list->head = new;
        list->tail = new;
    }
    else
    {
        list->tail->next = new;
        list->tail = new;
    }

    list->count++;

    return SUCCESS;
}

void delete_list(List *list)
{
    Dlist *temp = list->head;

    while (temp != NULL)
    {
        Dlist *next = temp->next;

        free(temp);

        temp = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

void print_list(const List *list)
{
    Dlist *temp = list->head;

    if (temp == NULL)
    {
        printf("0");
        return;
    }

    while (temp != NULL)
    {
        printf("%d", temp->data);
        temp = temp->next;
    }
}

//VALIDATION AND CONVERSION

Status validate_number(const char *str)
{
    int i = 0;

    if (str == NULL || str[0] == '\0')
        return FAILURE;

    while (str[i] != '\0')
    {
        if (str[i] < '0' || str[i] > '9')
            return FAILURE;

        i++;
    }

    return SUCCESS;
}

Status string_to_list(const char *str, List *list)
{
    int i = 0;

    while (str[i] != '\0')
    {
        int digit = str[i] - '0';

        if (insert_at_last(list, digit) == FAILURE)
        {
            delete_list(list);
            return FAILURE;
        }

        i++;
    }

    return SUCCESS;
}

int compare_lists(const List *list1, const List *list2)
{
    Dlist *temp1;
    Dlist *temp2;

    if (list1->count > list2->count)
        return 1;

    if (list1->count < list2->count)
        return -1;

    temp1 = list1->head;
    temp2 = list2->head;

    while (temp1 != NULL && temp2 != NULL)
    {
        if (temp1->data > temp2->data)
            return 1;

        if (temp1->data < temp2->data)
            return -1;

        temp1 = temp1->next;
        temp2 = temp2->next;
    }

    return 0;
}

void remove_leading_zeroes(List *list)
{
    while (list->head != NULL &&
           list->head->data == 0 &&
           list->head != list->tail)
    {
        Dlist *temp = list->head;

        list->head = list->head->next;
        list->head->prev = NULL;

        free(temp);

        list->count--;
    }

    if (list->head == NULL)
        insert_at_last(list, 0);
}

//ADDITION

Status addition(const List *op1, const List *op2, List *result)
{
    Dlist *ptr1 = op1->tail;
    Dlist *ptr2 = op2->tail;

    int carry = 0;

    while (ptr1 != NULL || ptr2 != NULL || carry != 0)
    {
        int digit1 = 0;
        int digit2 = 0;
        int sum;
        int digit;

        if (ptr1 != NULL)
        {
            digit1 = ptr1->data;
            ptr1 = ptr1->prev;
        }

        if (ptr2 != NULL)
        {
            digit2 = ptr2->data;
            ptr2 = ptr2->prev;
        }

        sum = digit1 + digit2 + carry;

        digit = sum % 10;
        carry = sum / 10;

        if (insert_at_first(result, digit) == FAILURE)
        {
            delete_list(result);
            return FAILURE;
        }
    }

    remove_leading_zeroes(result);

    return SUCCESS;
}

//SUBTRACTION
Status subtraction(const List *op1, const List *op2, List *result)
{
    Dlist *ptr1 = op1->tail;
    Dlist *ptr2 = op2->tail;

    int borrow = 0;

    while (ptr1 != NULL)
    {
        int digit1 = ptr1->data - borrow;
        int digit2 = 0;
        int difference;

        if (ptr2 != NULL)
        {
            digit2 = ptr2->data;
            ptr2 = ptr2->prev;
        }

        if (digit1 < digit2)
        {
            digit1 += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        difference = digit1 - digit2;

        if (insert_at_first(result, difference) == FAILURE)
        {
            delete_list(result);
            return FAILURE;
        }

        ptr1 = ptr1->prev;
    }

    remove_leading_zeroes(result);

    return SUCCESS;
}

//MULTIPLICATION
Status multiplication(const List *op1, const List *op2, List *result)
{
    Dlist *ptr2;

    List partial;
    List sum;

    int shift = 0;

    create_list(&partial);
    create_list(&sum);

    if (insert_at_last(result, 0) == FAILURE)
        return FAILURE;

    ptr2 = op2->tail;

    while (ptr2 != NULL)
    {
        int multiplier = ptr2->data;
        int carry = 0;

        Dlist *ptr1 = op1->tail;

        create_list(&partial);
        create_list(&sum);

        /* Multiply op1 by one digit */
        while (ptr1 != NULL || carry != 0)
        {
            int digit1 = 0;
            int product;

            if (ptr1 != NULL)
            {
                digit1 = ptr1->data;
                ptr1 = ptr1->prev;
            }

            product = (digit1 * multiplier) + carry;

            if (insert_at_first(&partial, product % 10) == FAILURE)
            {
                delete_list(&partial);
                delete_list(&sum);
                return FAILURE;
            }

            carry = product / 10;
        }

        remove_leading_zeroes(&partial);

        for (int i = 0; i < shift; i++)
        {
            if (insert_at_last(&partial, 0) == FAILURE)
            {
                delete_list(&partial);
                delete_list(&sum);
                return FAILURE;
            }
        }

        if (addition(result, &partial, &sum) == FAILURE)
        {
            delete_list(&partial);
            delete_list(&sum);
            return FAILURE;
        }

        delete_list(result);
        *result = sum;

        create_list(&sum);

        delete_list(&partial);

        shift++;
        ptr2 = ptr2->prev;
    }

    remove_leading_zeroes(result);

    delete_list(&partial);
    delete_list(&sum);

    return SUCCESS;
}

//DIVISION
Status division(
    const List *dividend,
    const List *divisor,
    List *quotient,
    List *remainder)
{
    Dlist *ptr = dividend->head;

    List temp;

    int quotient_digit;

    create_list(&temp);

    if (divisor->count == 1 && divisor->head->data == 0)
        return FAILURE;

    while (ptr != NULL)
    {
        if (insert_at_last(remainder, ptr->data) == FAILURE)
        {
            delete_list(&temp);
            return FAILURE;
        }

        remove_leading_zeroes(remainder);

        quotient_digit = 0;

        while (compare_lists(remainder, divisor) >= 0)
        {
            create_list(&temp);

            if (subtraction(remainder, divisor, &temp) == FAILURE)
            {
                delete_list(&temp);
                return FAILURE;
            }

            delete_list(remainder);

            *remainder = temp;

            create_list(&temp);

            quotient_digit++;
        }

        if (insert_at_last(quotient, quotient_digit) == FAILURE)
        {
            delete_list(&temp);
            return FAILURE;
        }

        ptr = ptr->next;
    }

    remove_leading_zeroes(quotient);
    remove_leading_zeroes(remainder);

    delete_list(&temp);

    return SUCCESS;
}
