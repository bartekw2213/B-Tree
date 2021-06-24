# B-Tree :evergreen_tree:

Implementation of B-Tree structure. Definition from [Wikipedia](https://en.wikipedia.org/wiki/B-tree):

> In [computer science](https://en.wikipedia.org/wiki/Computer_science), a **B-tree** is a self-balancing [tree data structure](https://en.wikipedia.org/wiki/Tree_data_structure) that maintains sorted data and allows searches, sequential access, insertions, and deletions in [logarithmic time](https://en.wikipedia.org/wiki/Logarithmic_time). The B-tree generalizes the [binary search tree](https://en.wikipedia.org/wiki/Binary_search_tree), allowing for nodes with more than two children.[[2\]](https://en.wikipedia.org/wiki/B-tree#cite_note-FOOTNOTEComer1979-2) Unlike other [self-balancing binary search trees](https://en.wikipedia.org/wiki/Self-balancing_binary_search_tree), the B-tree is well suited for storage systems that read and write relatively large blocks of data, such as disks. It is commonly used in [databases](https://en.wikipedia.org/wiki/Database) and [file systems](https://en.wikipedia.org/wiki/File_system).

## Operations that can be perfomed :hammer_and_wrench:
- I x - Construct a tree of order x.
- A x - Add the value x to the tree, x is a signed integer.
- ? x - Check if the value x is in the tree. If it is present print "x +", otherwise print "x -".
- P   - Print all values that are in the tree in increasing order.
- L t - Load a tree of order t. In the next line there is a tree in a format given as follows. A pair of brackets ( ) signify a node. A value signify a record (key in the tree).
- ( )   Hence ( ( ( 1 2 ) 3 ( 4 ) 5 ( 6 7 ) ) 8 ( ( 9 ) 10 ( 11 ) ) ) can be interpreted as the following tree:
                                        (         .           8       .    )
                                        (  . 3    .  5   .  )   ( . 10  .  )
                                        ( 1 2 ) ( 4 ) ( 6 7 )   ( 9 ) ( 11 )
- S   - Save the tree in the format described above.
- R x - Remove the element x from the tree.
- X   - End the program.
