#include "Imagery/Utils/array_sort.h"

void array_sort(Pixel *arr, int n)
{
    int i, j, min_idx;

    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j].b < arr[min_idx].b)
                min_idx = j;

        Pixel temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}
