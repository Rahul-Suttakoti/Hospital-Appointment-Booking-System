#include <stdio.h>
#include <stdlib.h>




static void swap_int(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}


void min_heapify(int arr[], int n, int i) {
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] < arr[smallest]) smallest = l;
    if (r < n && arr[r] < arr[smallest]) smallest = r;

    if (smallest != i) {
        swap_int(&arr[i], &arr[smallest]);
        min_heapify(arr, n, smallest);
    }
}


void build_min_heap(int arr[], int n) {
    
    for (int i = n / 2 - 1; i >= 0; --i) {
        min_heapify(arr, n, i);
    }
}


void heap_sort_min(int arr[], int n, int out[]) {
    if (n <= 0) return;

    
    build_min_heap(arr, n);

    
    int heap_size = n;
    for (int k = 0; k < n; ++k) {
        out[k] = arr[0];
        arr[0] = arr[heap_size - 1];
        heap_size--;
        if (heap_size > 0)
            min_heapify(arr, heap_size, 0);
    }
}

void print_array(const char *label, int a[], int n) {
    printf("%s", label);
    for (int i = 0; i < n; ++i) printf("%d ", a[i]);
    printf("\n");
}


int main(void) {
    int arr[] = { 12, 3, 17, 8, 34, 7, 1, 22 };
    int n = sizeof(arr) / sizeof(arr[0]);

    int heap_work[n];
    for (int i = 0; i < n; ++i) heap_work[i] = arr[i];

    int sorted[n];

    printf("Original array:\n");
    print_array("  ", arr, n);

    heap_sort_min(heap_work, n, sorted);

    printf("Sorted ascending (using min-heap extract):\n");
    print_array("  ", sorted, n);

    return 0;
}
