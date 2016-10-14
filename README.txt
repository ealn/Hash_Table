This project contains the implementation of a Hash table.
When a collision is detected this hash table will use an 
AVL tree to handle this collision

All code was wrote in C.

The hash value is calculated with the following mathematical
formula:

Hash_value = (ID % N)

Where:

ID: Identifier for a register in the table
N:  Hash table size
