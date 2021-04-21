Brian Huntley

I did not work with classmates, programming partners, or course staff on this assignment.

As far as I know everything is working correctly.

My segment data structure is different from what I had originally planned in my design document. I planned to have a segment containing other segments as the main data structure for my universal machine. The inner sequences would represent individual segments while the outer segments would be used in the mapping function. In my current implementation, still used a sequence to contain all the segments, but I created a separate struct for my segments. 

My main file gets the file from the command line, then calls the load function, and finally calls the execute function. The load module loads each word into my data structure which prepares it to be executed. Then the execute module loops until a halt instruction is found. It gets the instruction id using Bitpack, then calls the appropriate function for that instruction. The whole architecture uses the segment type which is a sequence of segment structures. Each segment contains an array of words and its size.

It takes my machine approximately 4.78 seconds to run 50 million instructions. I got this number by first keeping track of how many instructions are ran in a variable. This showed me that midmark runs 85,070,522 instructions. 85,070,522 * 0.58775 is about equal to 50 million. It takes my machine 8.14 seconds to run midmark, so 8.14*0.58775 = 4.78.

I spent 3 hours analyzing the assignment

I spent about an hour on design

I spend about 15 hours on the coding after the analysis.
