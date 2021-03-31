Brian Huntley

I did not work with classmates, programming partners, or course staff on this assignment.

As far as I know, both the image program and the bitpack program work as they are supposed to. 

Architecture:

My implementation reuses A2Methods from the last assignment. Originally in my design document I planned to use blocked arrays to navigate through the pixels of the image, but I ended up finding it much easier to work with them in row major order.

First my program gets the input from the user in input.c. This was provided in the handout for the assignment. From there it goes to the compress module, which will either compress or decompress the input. 

To compress an image I first trim the image if there is an odd amount of rows or columns. Next I take the pixel array from the image and convert it to an array of component video values. These values are stored in a struct called Component_video. That array is then converted to a, b, c, and d coefficients by the descrete cosine transform. Finally, those coefficients are packed using the bitpack module and printed.

To decompress an image I first read the file into a 2d array and unpack the values. The unpacked values are then converted back to component video using the inverse descrete cosine transform. Finally I convert the component video to RGB and print them using Pnmwrite.

I spent about 10 hours on analysis and understanding the problem.

I spent about 20 hours writing and debugging the code.