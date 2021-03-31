/* Brian Huntley
 * Assignment 2 - Part A
 */

#ifndef UARRAY2_INCLUDED
#define ARRAY_INCLUDED
#define T UArray2_T

typedef struct T *T;

extern T UArray2_new (int width, int height, int size);
/* UArray2_new
Creates a new 2d array with the specified dimensions
Size is the number of bytes used by each value in the array.
*/

extern void UArray2_free(T *uarray2);
/* UArray2_free
Deallocates the memory used by the array.
*/

extern int UArray2_width(T uarray2);
/* UArray2_width
Returns the number of values in each row of uarray2
*/

extern int UArray2_height(T uarray2);
/* UArray2_height
Returns the number of values in each column of uarray2
*/

extern int UArray2_size(T uarray2);
/* UArray2_size
Returns the size of values in uarray2
*/

extern void *UArray2_at(T uarray2, int col, int row);
/* UArray2_at
Returns a pointer to the values located at (row, col) in uarray2.
*/

extern void UArray2_map_row_major(T uarray2, void apply(int col, int row, T uarray2, void *val, void *cl), void *cl);
/* UArray2_map_row_major
Calls the apply function for each value row by row.
*/

extern void UArray2_map_col_major(T uarray2, void apply(int col, int row, T uarray2, void *val, void *cl), void *cl);
/* UArray2_map_col_major
Calls the apply function for each value column by column.
*/

#undef T
#endif
