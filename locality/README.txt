Brian Huntley

I did not work with classmates or programming partners on this assignment. Other than the class time that we spent working on a2plain together, I did not work with course staff on the assignment.

UArray2b works as it is intended. It is a two dimensional array, implemented by one outer array of blocks, and an inner array which contains the values inside each block.

a2plain works as it is intended. It is an implementations of A2Methods which uses my UArray2 from the previous assignment. Most of the code for a2plain is what we wrote together in class.

ppmtrans works as it is intended. I implemented all three rotations, vertical and horizontal flips, and transposition functions. It works by first getting the user input using the starter code that was given to us. Then I create a new image of the same number of bytes as the original. Then, depending on the transformation, I call the appropiate function to map through the image placing pixels where they should go.

Architecture-
	Both a2blocked and a2plain are implementations of A2Methods which use blocked 2d arrays and normal 2d arrays respectively. Ppmtrans then uses a2methods to choose the array implementation that the user requests in their input. Ppmtrans also uses the pnm interface to work on and modify images.

Part E:

	Row major	Col Major	Block Major
180	3.766s		2.610s		3.24s
90	3.012s		2.954s		3.33s

I ran each command three times and took the average for each, in order to get a more accurate timing. 

As I had predicted, column major 180 degree rotations had the best performance. This is because my UArray2 implementation is designed as an array of arrays, where the outer array represents columns. All the values in each column are stored near each other in memory, so reading and writing results in all hits.

For a similar reason, I had also predicted that row major 180 degree rotations would perform the worst. My UArray2 implementation makes it so that reading and writing in this fashion results in all misses.

What I did not expect was for block major to perform worse than both row and column major 90 degree rotations. Hit-rate wise, the block major method should perform well. However in my implementation this seems to be negated by the computation cost per pixel. I needed to use more arithmetic per pixel than I had originally assumed in my estimation. This was because I did not consider that I would need to check whether the contents of a block were out of bounds or not. Combined with all the arithmetic needed to calculate where specific values are located, my blocked arrays did not perform as well as I thought they would.   


I spent about 40 hours on this assignment.
