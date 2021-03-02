/* Brian Huntley
 * Assignment 2 - Part B
 */

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED
#define T Bit2_T

typedef struct T *T;

extern T Bit2_new (int width, int height);
/* Bit2_new
Creates a new 2d bit array with the specified dimensions
*/

extern void Bit2_free(T *bit2);
/* Bit2_free
Deallocates the memory used by the bit array.
*/

extern int Bit2_width(T bit2);
/* Bit2_width
Returns the number of values in each row of bit2
*/

extern int Bit2_height(T bit2);
/* Bit2_height
Returns the number of values in each column of bit2
*/

extern int Bit2_put(T bit2, int col, int row, int val);
/* Bit2_put
Stores val at (row, col) inside of bit2.
*/

extern int Bit2_get(T bit2, int col, int row);
/* Bit2_get
Retrieves the value stored at (row, col) inside of bit2.
*/

extern void Bit2_map_row_major(T bit2, void apply(int col, int row, int val, void *cl), void *cl);
/* Bit2_map_row_major
Calls the apply function for each value row by row.
*/

extern void Bit2_map_col_major(T bit2, void apply(int col, int row, int val, void *cl), void *cl);
/* Bit2_map_col_major
Calls the apply function for each value column by column.
*/

#undef T
#endif
