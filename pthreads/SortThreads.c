/*
    Author:Debargha Nath
    Roll:22CS01070
    Date: 26/08/2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define mx 1000000000

int a[mx];

typedef struct 
{
    int start;
    int end;
} data;

void merge(int start, int mid, int end) 
{
    int L = mid - start + 1;
    int R = end - mid;
    int l[L];
    int r[R];

    for (int i = 0; i < L; i++)
        l[i] = a[start + i];
    for (int j = 0; j < R; j++)
        r[j] = a[mid + 1 + j];

    int i = 0, j = 0, k = start;
    while (i < L && j < R) 
    {
        if (l[i] <= r[j]) {
            a[k] = l[i];
            i++;
        } else {
            a[k] = r[j];
            j++;
        }
        k++;
    }

    while (i < L) {
        a[k] = l[i];
        i++;
        k++;
    }

    while (j <R) 
    {
        a[k] = r[j];
        j++;
        k++;
    }
}

void *mergesort(void *arg) 
{
    data *newdata = (data *)arg;
    int start = newdata->start;
    int end = newdata->end;
    if (start >= end) return NULL;
    int mid = (start + end) / 2;
    data left_data = {start, mid};
    data right_data = {mid + 1, end};
    mergesort(&left_data);
    mergesort(&right_data);
    merge(start, mid, end);
    return NULL;
}

int main() {
    int n;
    printf("ENTER SIZE OF ARRAY: \n");
    scanf("%d", &n);

    if (n > mx) {
        printf("Error: Array size exceeds maximum allowed size (%d).\n", mx);
        return 1;
    }

    printf("ENTER ELEMENTS OF ARRAY: \n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    int mid = n/2;
    data initial_data1 = {0,mid};
    data initial_data2 = {mid+1,n-1};
    pthread_t initial_thread1,initial_thread2;

    pthread_create(&initial_thread1, NULL, mergesort, &initial_data1);
    pthread_create(&initial_thread2, NULL, mergesort, &initial_data2);
    int * s1,*s2;
    pthread_join(initial_thread1, (void *)&s1);
    pthread_join(initial_thread2,(void *)&s2);
    
    merge(0, mid, n - 1);
    printf("SORTED ARRAY: \n");
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
