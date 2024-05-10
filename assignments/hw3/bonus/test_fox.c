#include <stdio.h>
#include <assert.h>
#include "fox.h"

void test_matmul() {
    int a[2][2] = {{1, 2}, {3, 4}};
    int b[2][2] = {{5, 6}, {7, 8}};
    int c[2][2] = {{0, 0}, {0, 0}};

    matmul(a, b, c, 2);

    assert(c[0][0] == 19);
    assert(c[0][1] == 22);
    assert(c[1][0] == 43);
    assert(c[1][1] == 50);
}

void test_transfer_data_from_buff() {
    int buff[4] = {1, 2, 3, 4};
    int a[2][2] = {{0, 0}, {0, 0}};

    transfer_data_from_buff(buff, (int **)a, 4, 2, 2);

    assert(a[0][0] == 1);
    assert(a[0][1] == 2);
    assert(a[1][0] == 3);
    assert(a[1][1] == 4);
}

void test_transfer_data_to_buff() {
    int buff[4] = {0, 0, 0, 0};
    int a[2][2] = {{1, 2}, {3, 4}};

    transfer_data_to_buff(buff, (int **)a, 4, 2, 2);

    assert(buff[0] == 1);
    assert(buff[1] == 2);
    assert(buff[2] == 3);
    assert(buff[3] == 4);
}

void test_Fox() {
    int a[2][2] = {{1, 2}, {3, 4}};
    int b[2][2] = {{5, 6}, {7, 8}};
    int c[2][2] = {{0, 0}, {0, 0}};

    GridInfo grid;
    grid.q = 2;

    Fox(2, &grid, a, b, c);

    assert(c[0][0] == 19);
    assert(c[0][1] == 22);
    assert(c[1][0] == 43);
    assert(c[1][1] == 50);
}

int main() {
    //test_matmul();
    //test_transfer_data_from_buff();
    //test_transfer_data_to_buff();
    test_Fox();

    printf("All tests passed!\n");

    return 0;
}