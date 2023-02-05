#include "dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/* private functions *********************************************************/

/* Position in the buffer of the array element at position index */
static int index_to_offset ( const DynamicArray * da, int index ) {
    return index + da->origin;
}

/* Position of the element at buffer position 'offset' */
static int offset_to_index ( const DynamicArray * da, int offset ) {
    return offset - da->origin;
}

/* Non-zero if and only if offset lies ouside the buffer */
static int out_of_buffer ( DynamicArray * da, int offset ) {
    return offset < 0 || offset >= da->capacity;
}

/* Makes a new buffer that is twice the size of the old buffer,
   copies the old information into the new buffer, and deletes
   the old buffer */
static void extend_buffer ( DynamicArray * da ) {

    double * temp = (double *) calloc ( 2 * da->capacity, sizeof(double) );
    int new_origin = da->capacity - (da->end - da->origin)/2,
           new_end = new_origin + (da->end - da->origin);

    for ( int i=0; i<DynamicArray_size(da); i++ ) {
        temp[new_origin+i] = DynamicArray_get(da,i);
    }

    free(da->buffer);
    da->buffer = temp;

    da->capacity = 2 * da->capacity;
    da->origin = new_origin;
    da->end = new_end;

    return;

}

/* public functions **********************************************************/

DynamicArray * DynamicArray_new(void) {
    DynamicArray * da = (DynamicArray *) malloc(sizeof(DynamicArray));
    da->capacity = DYNAMIC_ARRAY_INITIAL_CAPACITY;    
    da->buffer = (double *) calloc ( da->capacity, sizeof(double) ); 
    da->origin = da->capacity / 2;
    da->end = da->origin;
    return da;
}

void DynamicArray_destroy(DynamicArray * da) {
    free(da->buffer);
    da->buffer = NULL;
    return;
}

int DynamicArray_size(const DynamicArray * da) {
    assert(da->buffer != NULL);
    return da->end - da->origin;
}

char * DynamicArray_to_string(const DynamicArray * da) {
    assert(da->buffer != NULL);
    char * str = (char *) calloc (20,DynamicArray_size(da)),
         temp[20];
    int j = 1;
    str[0] = '[';
    for ( int i=0; i < DynamicArray_size(da); i++ ) {
        if ( DynamicArray_get(da,i) == 0 ) {
            snprintf ( temp, 20, "0" );
        } else {
            snprintf ( temp, 20, "%.5lf", DynamicArray_get(da,i) ); 
        }
        if ( i < DynamicArray_size(da) - 1 ) {
            sprintf( str + j, "%s,", temp);
            j += strlen(temp) + 1;
        } else {
            sprintf( str + j, "%s", temp);
            j += strlen(temp);
        }

    }
    str[j] = ']';
    return str;
}

void DynamicArray_print_debug_info(const DynamicArray * da) {

    char * s = DynamicArray_to_string(da);
    printf ( "  %s\n", s);
    printf ( "  capacity: %d\n  origin: %d\n  end: %d\n  size: %d\n\n",
      da->capacity, 
      da->origin, 
      da->end,
      DynamicArray_size(da));

    free(s);

}

void DynamicArray_set(DynamicArray * da, int index, double value) {
    assert(da->buffer != NULL);
    assert ( index >= 0 );
    while ( out_of_buffer(da, index_to_offset(da, index) ) ) {
        extend_buffer(da);
    }
    da->buffer[index_to_offset(da, index)] = value;
    if ( index >= DynamicArray_size(da) ) {
        da->end = index_to_offset(da,index+1);
    }

}

double DynamicArray_get(const DynamicArray * da, int index) {
    assert(da->buffer != NULL);
    assert ( index >= 0 );
    if ( index >= DynamicArray_size(da) ) {
        return 0;
    } else {
        return da->buffer[index_to_offset(da,index)];
    }
}

void DynamicArray_push(DynamicArray * da, double value ) {
    DynamicArray_set(da, DynamicArray_size(da), value );
}

void DynamicArray_push_front(DynamicArray * da, double value) {
    assert(da->buffer != NULL);
    while ( da->origin == 0 ) {
        extend_buffer(da);
    }
    da->origin--;
    DynamicArray_set(da,0,value);
}

double DynamicArray_pop(DynamicArray * da) {
    assert(DynamicArray_size(da) > 0);
    double value = DynamicArray_get(da, DynamicArray_size(da)-1);
    DynamicArray_set(da, DynamicArray_size(da)-1, 0.0);
    da->end--;
    return value;
}

double DynamicArray_pop_front(DynamicArray * da) {
    assert(DynamicArray_size(da) > 0);
    double value = DynamicArray_get(da, 0);
    da->origin++;
    return value;    
}

DynamicArray * DynamicArray_map(const DynamicArray * da, double (*f) (double)) {
    assert(da->buffer != NULL);
    DynamicArray * result = DynamicArray_new();
    for ( int i=0; i<DynamicArray_size(da); i++ ) {
        DynamicArray_set(result, i, f(DynamicArray_get(da, i)));
    }
    return result;
}

DynamicArray * DynamicArray_subarray(DynamicArray * da, int a, int b) {

  assert(da->buffer != NULL);
  assert(b >= a);

  DynamicArray * result = DynamicArray_new();

  for (int i=a; i<b; i++) {
      DynamicArray_push(result,DynamicArray_get(da, i));
  }

  return result;

}

double _min(DynamicArray *arr) {
    assert(DynamicArray_size(arr) > 0);
    double min = arr->buffer[0];
    for (int i = 1; i < DynamicArray_size(arr); i++) {
        if (arr->buffer[i] < min) {
            min = arr->buffer[i];
        }
    }
    return min;
}



double _max(DynamicArray *arr) {
    assert(DynamicArray_size(arr) > 0);
    double max = arr->buffer[0];
    for (int i = 1; i < DynamicArray_size(arr); i++) {
        if (arr->buffer[i] > max) {
            max = arr->buffer[i];
        }
    }
    return max;
}



double _mean(DynamicArray *arr) {
    assert(DynamicArray_size(arr) > 0);
    double sum = 0;
    for (int i = 0; i < DynamicArray_size(arr); i++) {
        sum += arr->buffer[i];
    }
    return sum / DynamicArray_size(arr);
}




double _median(DynamicArray *arr) {
    assert(DynamicArray_size(arr) > 0);

    int size = DynamicArray_size(arr);
    int mid = size / 2;
    double median;
    if (size % 2 == 0) {
        median = (arr->buffer[mid - 1] + arr->buffer[mid]) / 2.0;
    } else {
        median = arr->buffer[mid];
    }
    return median;
}




double _sum(DynamicArray *arr) {
    double sum = 0;
    for (int i = 0; i < DynamicArray_size(arr); i++) {
        sum += arr->buffer[i];
    }
    return sum;
}



int DynamicArray_last(DynamicArray *dynArray) {
assert(dynArray->end != dynArray->origin);
return dynArray->buffer[dynArray->end - 1];
}



int DynamicArray_first(DynamicArray *dynArray) {
assert(dynArray->end != dynArray->origin);
return dynArray->buffer[dynArray->origin];
}



DynamicArray DynamicArray_copy(DynamicArray *dynArray) {
    DynamicArray copy;
    copy.capacity = dynArray->capacity;
    copy.origin = dynArray->origin;
    copy.end = dynArray->end;
    copy.buffer = (double*) malloc(copy.capacity * sizeof(double));
    memcpy(copy.buffer, dynArray->buffer, copy.capacity * sizeof(double));
    return copy;
}




DynamicArray * DynamicArray_range(double start, double end, double step) {
    int size = ceil((end - start) / step) + 1;
    DynamicArray *arr = (DynamicArray*) malloc(sizeof(DynamicArray));
    arr->capacity = size;
    arr->origin = 0;
    arr->end = size;
    arr->buffer = (double*) malloc(size * sizeof(double));
    for (int i = 0; i < size; i++) {
        arr->buffer[i] = start + i * step;
    }
    return arr;
}


DynamicArray * DynamicArray_concat(DynamicArray * a, DynamicArray * b) {
    int size_a = DynamicArray_size(a);
    int size_b = DynamicArray_size(b);

    DynamicArray * concat = (DynamicArray *) malloc(sizeof(DynamicArray));
    concat->end = size_a + size_b;
    concat->capacity = concat->end - concat->origin;
    concat->buffer = (double *) malloc(concat->capacity * sizeof(double));

    int i;
    for (i = 0; i < size_a; i++) {
        concat->buffer[i] = a->buffer[i];
    }

    for (int j = 0; j < size_b; j++, i++) {
        concat->buffer[i] = b->buffer[j];
    }

    return concat;
}



DynamicArray * DynamicArray_take(DynamicArray * arr, int n) {
    DynamicArray * result = (DynamicArray *) malloc(sizeof(DynamicArray));

    int numElements = arr->end - arr->origin;
    int start = n >= 0 ? arr->origin : arr->end - abs(n);
    int count = n >= 0 ? n : abs(n);
    count = count < numElements ? count : numElements;
    result->capacity = count + (n > numElements ? n - numElements : 0);
    result->origin = 0;
    result->end = count;
    result->buffer = (double *) malloc(sizeof(double) * result->capacity);

    for (int i = 0; i < count; i++) {
        result->buffer[i] = arr->buffer[start + i];
    }
    for (int i = count; i < result->capacity; i++) {
        result->buffer[i] = 0;
    }

    return result;
}
