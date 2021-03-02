Brian Huntley

I did not work with classmates, programming partners, or course staff on this assignment. I used Hanson's array.c and bit.c as a guideline to making my UArray2 and Bit2 types. Additionally I used the idioms for using Hanson's arrays, and revisited the Pnmrdr handout to refresh my memory on how to use that interface.

UArray2 works as it is intended. It is a two dimensional array, implemented by one outer array of length width, which contains many inner arrays of the length of the height. I only made minor changes to what I had laid out in the design document, such as changing the order of parameters.

Bit2 works as it is intended. It is designed just like UArray2, but the inner values are Hanson bits instead of arrays. I changed the type of values passed to Bit2's mapping functions, as they can only be integers.

Sudoku works as it is intended. I read the image, testing if it is valid, then insert the values into a UArray2. Once I have done this I test for repeating values in each row, column, and 3x3 square. I do this by passing around an integer array of 10 values. Each value is initialized as a zero. Digits 1-9 are incremented by one at the index of the digit, for each time they are encountered. After reading the whole row/column/3x3 square, I iterate through that array and make sure every value from index 1-9 is equal to 1. If it isn't, that means there was a repeat, and I increment index 0 to keep track of that. Finally, I check index 0 at the end of my program. If it is still 0, I know the input is a correct sudoku board. Any other value and it is incorrect. 

Unblackedges only works in some cases. My unblackedges program works by finding a black pixel along the edge of the image, and writing over black pixels until it either hits the other side of the image, or a white pixel. This leads to cases where my program hits a white pixel and moves on, leaving some black pixels behind. I tried to write a function that would recursively check for black squares and white them out, but I had issues with keeping track of what pixels were already visited. I considered limiting which direction the function could check, so that it wouldn't go back and forth between the same two pixels, but that would lead to some edges not getting whited out.

I spent about 30 hours on this assignment.