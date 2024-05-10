#include <stdio.h>
#include <assert.h>
#include "fox.h"

void test_Fox() {
    // Initialize a GridInfo structure
    GridInfo grid;
    grid.p = 4; // for example
    // ... initialize other fields of grid as necessary

    // Initialize matrices a, b, and c
    int a[2][2] = {{1, 2}, {3, 4}};
    int b[2][2] = {{5, 6}, {7, 8}};
    int c[2][2] = {{0, 0}, {0, 0}};

    // Call the Fox function
    Fox(2, &grid, a, b, c);

    // Check if the result is as expected
    assert(c[0][0] == 19);
    assert(c[0][1] == 22);
    assert(c[1][0] == 43);
    assert(c[1][1] == 50);
}

int main() {
    test_Fox();
    printf("Fox function is correct\n")
    return 0;
}

