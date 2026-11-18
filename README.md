
# Arbitrary Precision Calculator (APC)

## 1. Project Overview

The **Arbitrary Precision Calculator (APC)** is a command-line calculator developed in C that performs arithmetic operations on integers of arbitrary length.

Unlike normal C arithmetic operations, which are restricted by the storage capacity of built-in data types such as `int`, `long`, and `long long`, this calculator can handle very large integers.

The project uses a **Doubly Linked List (DLL)** to store individual digits of numbers dynamically.

Each node stores one decimal digit, allowing the program to perform arithmetic operations without storing the complete number in a native integer variable.

## 2. Features

The calculator supports the following operations:

- Addition (`+`)
- Subtraction (`-`)
- Multiplication (`x`)
- Integer Division (`/`)
- Positive and negative numbers
- Numbers with or without an explicit positive sign
- Large integers beyond the range of built-in C data types
- Quotient and remainder calculation
- Invalid input detection
- Division-by-zero handling

## 3. Technologies Used

- **Programming Language:** C
- **Data Structure:** Doubly Linked List
- **Compiler:** GCC
- **Build Tool:** GNU Make
- **Development Environment:** Linux / WSL Ubuntu

## 4. Project Structure

```text
APC/
│
├── apc.h
├── main.c
├── arithmetic_functions.c
├── sign.c
├── Makefile
└── README.md
```

### File Responsibilities

| File | Responsibility |
|---|---|
| `apc.h` | Contains header files, structures, macros, status definitions and function declarations. |
| `main.c` | Handles command-line arguments, input validation, operation selection, function calls and output. |
| `arithmetic_functions.c` | Implements DLL operations and arithmetic algorithms. |
| `sign.c` | Handles operand signs and determines the sign of arithmetic results. |
| `Makefile` | Automates compilation and executable generation. |
| `README.md` | Documents the project. |

## 5. Data Structure Used

### Doubly Linked List

A Doubly Linked List is used to store the digits of each operand and the arithmetic result.

Each node contains:

- `data`: Stores one decimal digit.
- `prev`: Pointer to the previous node.
- `next`: Pointer to the next node.

The number is stored in normal left-to-right order.

For example, the number `12345` is represented as:

```text
HEAD                                  TAIL
  |                                     |
  v                                     v
NULL <- [1] <-> [2] <-> [3] <-> [4] <-> [5] -> NULL
```

- Head stores the most significant digit (MSD).
- Tail stores the least significant digit (LSD).

Arithmetic operations generally begin from the tail and move backwards using the `prev` pointer.

### List Structure

A wrapper structure maintains the linked list:

```c
typedef struct
{
    Dlist *head;
    Dlist *tail;
    int count;
} List;
```

This makes it easier to manage multiple lists independently.

The project uses separate lists for:

- First operand
- Second operand
- Result
- Remainder during division
- Temporary intermediate calculations

### Why Doubly Linked List?

A DLL provides:

1. Dynamic memory allocation without a predefined maximum number of digits.
2. Forward traversal from MSD to LSD.
3. Backward traversal from LSD to MSD.
4. Efficient insertion at both ends.
5. Convenient implementation of arithmetic operations involving carry and borrow.

A normal array could also store large numbers, but a fixed-size array would impose a maximum capacity. Dynamic arrays would require resizing and potentially shifting elements.

A DLL was selected because it provides convenient insertion and traversal from both ends, which are useful for this project.

## 6. Arithmetic Algorithms

### 6.1 Addition

Addition follows the conventional decimal addition algorithm.

Steps:

1. Start from the tail of both operand lists.
2. Extract one digit from each operand.
3. Add the digits along with the carry.
4. Calculate the result digit using `sum % 10`.
5. Calculate the next carry using `sum / 10`.
6. Insert the result digit at the beginning of the result list.
7. Continue until both operands and the carry are exhausted.

Example:

```text
      12345
    +  6789
    -------
      19134
```

### 6.2 Subtraction

Subtraction follows the conventional decimal subtraction algorithm using borrowing.

Steps:

1. Ensure the first magnitude is greater than or equal to the second magnitude.
2. Start from the tail of both lists.
3. Subtract corresponding digits.
4. Borrow from the next higher digit when necessary.
5. Insert each result digit at the beginning.
6. Remove unnecessary leading zeroes.

Example:

```text
      12345
    -  6789
    -------
       5556
```

### 6.3 Multiplication

Multiplication uses the **Grade-School (Long Multiplication) Algorithm**.

Instead of repeatedly adding one operand, each digit of the second operand is multiplied with all digits of the first operand.

Steps:

1. Start from the least significant digit of the second operand.
2. Multiply it with every digit of the first operand.
3. Handle carry propagation.
4. Generate a partial product.
5. Append zeroes according to the decimal position.
6. Add the partial product to the accumulated result.
7. Repeat for every digit of the second operand.

Example:

```text
        123
      x  45
      -----
        615
       4920
      -----
       5535
```

The addition function is reused to accumulate the partial products.

### 6.4 Division

Division uses the **Long Division Algorithm**.

Steps:

1. Start from the most significant digit of the dividend.
2. Bring down the next digit into the current remainder.
3. Compare the current remainder with the divisor.
4. Repeatedly subtract the divisor until the remainder becomes smaller than the divisor.
5. The number of successful subtractions becomes the next quotient digit.
6. Append the quotient digit to the quotient list.
7. Continue until all dividend digits are processed.

Example:

```text
100 / 3

Quotient  = 33
Remainder = 1
```

The comparison and subtraction functions are reused during division.

The initial implementation supports integer division, not floating-point division.

## 7. Sign Handling

Signs are stored separately from the DLL digits.

For example:

```text
Number: -12345

Sign = -1

DLL:
[1] <-> [2] <-> [3] <-> [4] <-> [5]
```

This keeps the arithmetic functions independent of sign handling.

### Addition and Subtraction

For signed addition and subtraction:

- Same effective signs: add magnitudes.
- Different effective signs: subtract the smaller magnitude from the larger magnitude.
- The sign of the result is determined by the operand with the larger magnitude.
- Equal magnitudes produce zero.

Subtraction can be treated as addition with the sign of the second operand inverted.

### Multiplication and Division

- Same operand signs produce a positive result.
- Different operand signs produce a negative result.
- Zero is always represented as positive zero.

The remainder follows the sign of the dividend in this implementation.

## 8. Important Utility Functions

| Function | Purpose |
|---|---|
| `create_list()` | Initializes an empty DLL. |
| `insert_at_first()` | Inserts a digit at the beginning. |
| `insert_at_last()` | Inserts a digit at the end. |
| `delete_list()` | Frees all nodes in a DLL. |
| `print_list()` | Displays the number. |
| `validate_number()` | Checks whether the digit string is valid. |
| `string_to_list()` | Converts a numeric string into a DLL. |
| `compare_lists()` | Compares two positive magnitudes. |
| `remove_leading_zeroes()` | Removes unnecessary zeroes from the beginning. |
| `addition()` | Adds two magnitudes. |
| `subtraction()` | Subtracts the second magnitude from the first. |
| `multiplication()` | Multiplies two magnitudes. |
| `division()` | Calculates quotient and remainder. |

## 9. Compilation

The project uses a Makefile to automate compilation.

### Compile

```bash
make
```

This generates the executable:

```text
APC.out
```

### Clean

```bash
make clean
```

This removes the generated executable.

## 10. Execution

General syntax:

```bash
./APC.out <operand1> <operator> <operand2>
```

### Addition

```bash
./APC.out 1234 + 123
./APC.out +1234 + +123
./APC.out -1234 + +123
./APC.out -1234 + -123
```

### Subtraction

```bash
./APC.out 1234 - 123
./APC.out +1234 - -123
./APC.out -1234 - +123
```

### Multiplication

```bash
./APC.out 1234 x 123
./APC.out -1234 x +123
./APC.out -1234 x -123
```

### Division

```bash
./APC.out 1234 / 123
./APC.out -1234 / +123
./APC.out -1234 / -123
```

## 11. Input Validation and Edge Cases

The project handles the following cases:

- Invalid characters in operands.
- Empty operands.
- Invalid operators.
- Explicit positive and negative signs.
- Leading zeroes.
- Carry propagation.
- Borrow propagation.
- Addition and subtraction when the first operand is smaller.
- Multiplication by zero.
- Division where the dividend is smaller than the divisor.
- Division by zero.
- Negative operands.
- Large integer operands.

## 12. Learning Outcomes

Through this project, I gained practical experience in:

- Implementing and managing Doubly Linked Lists in C.
- Dynamic memory allocation using `malloc()` and `free()`.
- Managing multiple linked lists simultaneously.
- Understanding pointer manipulation and node connections.
- Performing arithmetic operations manually without relying on native integer arithmetic for complete operands.
- Implementing carry and borrow propagation.
- Understanding grade-school multiplication and long division.
- Separating sign handling from magnitude calculations.
- Working with command-line arguments (`argc` and `argv`).
- Organizing a multi-file C project using header files.
- Using Makefiles to automate compilation.
- Debugging pointer-related and memory-management issues.

## 13. Challenges Faced

Some of the major challenges involved:

1. **DLL Management:** Maintaining correct `head`, `tail`, `prev`, and `next` pointers during insertion and deletion.

2. **Carry and Borrow:** Handling carry propagation during addition and multiplication, and borrowing during subtraction.

3. **Large Number Representation:** Performing arithmetic without converting the complete operands into native integer types.

4. **Signed Arithmetic:** Separating signs from magnitudes and correctly determining the result sign for different combinations of positive and negative operands.

5. **Multiplication:** Generating partial products and accumulating them with the correct decimal shifts.

6. **Division:** Implementing long division using linked-list comparison and subtraction.

7. **Memory Management:** Avoiding memory leaks, dangling pointers, and invalid memory access while creating and replacing temporary lists.

## 14. Possible Future Improvements

- Floating-point division.
- More optimized multiplication algorithms.
- More efficient division algorithms.
- Improved error handling and memory allocation management.
- Automated unit testing.
- Support for additional mathematical operations.

---
