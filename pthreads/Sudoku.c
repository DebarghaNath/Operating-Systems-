/*
    Team :
    22CS01060
    22CS01070
    22CS01075

We Use 27 Threads For Calculation Here:
9 for columns => O(9)
9 for rows O(9)
9 for 3x3 grids O(9)
However All the 9 thread for a particular task are simultaneous, which means 27 task are done in O(9)
effectively time complexity is O(9)

**Draw back**
here hardware limitaion is a major issue. Hardware may not support 27 simultaneous threads which can lead to synchronization issues
Also extra waiting time for each loops. Hence we may always not get the time complexity of o(9) depending over the hardware specifications
    
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

int sudoku[9][9];

typedef struct {
    int row;
    int col;
} data;/*
    Team :
    22CS01060
    22CS01070
    22CS01075
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

int sudoku[9][9];

typedef struct {
    int row;
    int col;
} data;
void *check_column(void *column) {
    int col = *(int *)column;
    free(column); 
    int count[10] = {0};  
    for (int i = 0; i < 9; i++) {
        count[sudoku[i][col]]++;
    }
    for (int i = 1; i < 10; i++) {
        if (count[i] == 0 || count[i] > 1) {
            bool *result = malloc(sizeof(bool));
            *result = false;
            return result;
        }
    }
    bool *result = malloc(sizeof(bool));
    *result = true;
    return result;
}
void *check_row(void *row) {
    int ro = *(int *)row;
    free(row); 
    int count[10] = {0}; 
    for (int i = 0; i < 9; i++) {
        count[sudoku[ro][i]]++;
    }
    for (int i = 1; i < 10; i++) {
        if (count[i] == 0 || count[i] > 1) {
            bool *result = malloc(sizeof(bool));
            *result = false;
            return result;
        }
    }
    bool *result = malloc(sizeof(bool));
    *result = true;
    return result;
}

void *check3x3(void *arg) {
    data *new_data = (data *)arg;
    int count[10] = {0};  
    for (int r = new_data->row; r < new_data->row + 3; r++) {
        for (int c = new_data->col; c < new_data->col + 3; c++) {
            count[sudoku[r][c]]++;
        }
    }
    free(new_data); 
    for (int i = 1; i < 10; i++) {
        if (count[i] == 0 || count[i] > 1) {
            bool *result = malloc(sizeof(bool));
            *result = false;
            return result;
        }
    }
    bool *result = malloc(sizeof(bool));
    *result = true;
    return result;
}

int main() {
    printf("Enter the Sudoku puzzle (9x9 grid with values 1-9):\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            scanf("%d", &sudoku[i][j]);
        }
    }

    pthread_t column_threads[9];
    pthread_t row_threads[9];
    pthread_t square_threads[9];

    for (int i = 0; i < 9; i++) {
        int *col = malloc(sizeof(int));
        *col = i;
        pthread_create(&column_threads[i], NULL, check_column, col);
    }
    for (int i = 0; i < 9; i++) {
        int *ro = malloc(sizeof(int));
        *ro = i;
        pthread_create(&row_threads[i], NULL, check_row, ro);
    }

    int idx = 0;
    for (int r = 0; r < 9; r += 3) {
        for (int c = 0; c < 9; c += 3) {
            data *new_data = malloc(sizeof(data));
            new_data->row = r;
            new_data->col = c;
            pthread_create(&square_threads[idx], NULL, check3x3, new_data);
            idx++;
        }
    }

    int res = true;
    bool *var_res;
    for (int i = 0; i < 9; i++) {
        pthread_join(column_threads[i], (void **)&var_res);
        res &= *var_res;
        free(var_res); 
    }
    for (int i = 0; i < 9; i++) {
        pthread_join(row_threads[i], (void **)&var_res);
        res &= *var_res;
        free(var_res); 
    }

    for (int i = 0; i < 9; i++) {
        pthread_join(square_threads[i], (void **)&var_res);
        res &= *var_res;
        free(var_res); 
    }

    if (res) {
        printf("CORRECT SUDOKU SOLUTION\n");
    } else {
        printf("INCORRECT SUDOKU SOLUTION\n");
    }

    return 0;
}

void *check_column(void *column) {
    int col = *(int *)column;
    free(column); 
    int count[10] = {0};  
    for (int i = 0; i < 9; i++) {
        count[sudoku[i][col]]++;
    }
    for (int i = 1; i < 10; i++) {
        if (count[i] == 0 || count[i] > 1) {
            bool *result = malloc(sizeof(bool));
            *result = false;
            return result;
        }
    }
    bool *result = malloc(sizeof(bool));
    *result = true;
    return result;
}
void *check_row(void *row) {
    int ro = *(int *)row;
    free(row); 
    int count[10] = {0}; 
    for (int i = 0; i < 9; i++) {
        count[sudoku[ro][i]]++;
    }
    for (int i = 1; i < 10; i++) {
        if (count[i] == 0 || count[i] > 1) {
            bool *result = malloc(sizeof(bool));
            *result = false;
            return result;
        }
    }
    bool *result = malloc(sizeof(bool));
    *result = true;
    return result;
}

void *check3x3(void *arg) {
    data *new_data = (data *)arg;
    int count[10] = {0};  
    for (int r = new_data->row; r < new_data->row + 3; r++) {
        for (int c = new_data->col; c < new_data->col + 3; c++) {
            count[sudoku[r][c]]++;
        }
    }
    free(new_data); 
    for (int i = 1; i < 10; i++) {
        if (count[i] == 0 || count[i] > 1) {
            bool *result = malloc(sizeof(bool));
            *result = false;
            return result;
        }
    }
    bool *result = malloc(sizeof(bool));
    *result = true;
    return result;
}

int main() {
    printf("Enter the Sudoku puzzle (9x9 grid with values 1-9):\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            scanf("%d", &sudoku[i][j]);
        }
    }

    pthread_t column_threads[9];
    pthread_t row_threads[9];
    pthread_t square_threads[9];

    for (int i = 0; i < 9; i++) {
        int *col = malloc(sizeof(int));
        *col = i;
        pthread_create(&column_threads[i], NULL, check_column, col);
    }
    for (int i = 0; i < 9; i++) {
        int *ro = malloc(sizeof(int));
        *ro = i;
        pthread_create(&row_threads[i], NULL, check_row, ro);
    }

    int idx = 0;
    for (int r = 0; r < 9; r += 3) {
        for (int c = 0; c < 9; c += 3) {
            data *new_data = malloc(sizeof(data));
            new_data->row = r;
            new_data->col = c;
            pthread_create(&square_threads[idx], NULL, check3x3, new_data);
            idx++;
        }
    }

    int res = true;
    bool *var_res;
    for (int i = 0; i < 9; i++) {
        pthread_join(column_threads[i], (void **)&var_res);
        res &= *var_res;
        free(var_res); 
    }
    for (int i = 0; i < 9; i++) {
        pthread_join(row_threads[i], (void **)&var_res);
        res &= *var_res;
        free(var_res); 
    }

    for (int i = 0; i < 9; i++) {
        pthread_join(square_threads[i], (void **)&var_res);
        res &= *var_res;
        free(var_res); 
    }

    if (res) {
        printf("CORRECT SUDOKU SOLUTION\n");
    } else {
        printf("INCORRECT SUDOKU SOLUTION\n");
    }

    return 0;
}
