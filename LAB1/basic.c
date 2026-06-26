#include <stdio.h>
#include "pico/stdlib.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main() {
    stdio_init_all();

    printf("Hello, Embedded World!\n");
 
while (true) {
    sleep_ms(3000);

    // Arithmetic operators
    printf("Arithmetic operators (a = 10, b = 20)\n");
    int a = 10, b = 20;
    printf("a + b = %d\n", a + b);
    printf("a - b = %d\n", a - b);
    printf("a * b = %d\n", a * b);
    printf("b / a = %d\n", b / a);
    printf("b %% a = %d\n", b % a);

    // Relational operators
    printf("Relational operators\n");
    printf("a > b is %d\n", a > b);
    printf("a < b is %d\n", a < b);
    printf("a >= b is %d\n", a >= b);
    printf("a <= b is %d\n", a <= b);
    printf("a == b is %d\n", a == b);
    printf("a != b is %d\n", a != b);

    // Logical operators
    printf("Logical operators (x = 1, y = 0)\n");
    int x = 1, y = 0;
    printf("x && y is %d\n", x && y);
    printf("x || y is %d\n", x || y);
    printf("!x is %d\n", !x);

    // Increment and decrement operators
    printf("Increment and decrement operators (c = 5)\n");
    int c = 5;
    printf("c++ is %d\n", c++);
    printf("++c is %d\n", ++c);
    printf("c-- is %d\n", c--);
    printf("--c is %d\n", --c);

    // Assignment operators
    printf("Assignment operators (d = 10)\n");
    int d = 10;
    d += 5;
    printf("d after += 5: %d\n", d);

    // Ternary conditional operator
    printf("Ternary conditional operators (someNumber = 15)\n");
    int someNumber = 15;
    const char* result = (someNumber > 10) ? "Greater than 10" : "Not greater than 10";
    printf("Ternary result: %s\n", result);

    // Bitwise operators
    printf("Bitwise operators (p = 5, q = 3)\n");
    int p = 5, q = 3;
    printf("p & q = %d\n", p & q); // Bitwise AND
    printf("p | q = %d\n", p | q); // Bitwise OR
    printf("p ^ q = %d\n", p ^ q); // Bitwise XOR
    printf("~p = %d\n", ~p);       // Bitwise NOT
    printf("q << 1 = %d\n", q << 1); // Left shift
    printf("q >> 1 = %d\n", q >> 1); // Right shift

    // Macros
    printf("Macros (num1 = 10, num2 = 15)\n");
    int num1 = 10, num2 = 15;
    int max = MAX(num1, num2);
    printf("The maximum of %d and %d is %d\n", num1, num2, max);
}
    return 0;
}
