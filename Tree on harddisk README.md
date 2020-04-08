# BINARY-SEARCH-TREE-ON-HARDISK-
Making a binary search tree on harddisk

This project is basically all about constructing a binary search tree on the harddisk which we implement using varies file writing fucntions
given below (inbuilt )
1. fseek()
2. fwrite()
3. fread()
I have used the following functions mentioned in this project 


FILE* init_tree(const char* filename);

// close the file pointed by fp
void close_tree(FILE *fp);

// Space separated keys. Last key followed by '\n'
void display_inorder(FILE *fp);

// Space separated keys. Last key followed by '\n'
void display_preorder(FILE *fp);

// insert the key into the tree in the file
// if the key already exists just return
void insert_key(int key, FILE *fp);

// delete the key from the tree in the file
// the key may or may not exist
void delete_key(int key, FILE *fp);


In the delete functions we use the delete policy 

for two child connect the largest element in the left subtree .

inorder predecessor .


In addition to the above mentioned functions 

There are some suppoting functions which get called internally to help the above functions .


int findMax(int curr,int *inner_prev,FILE *fp)  // returns they offset of the maximum in the left subtree of a node.


// the key may or may not exist
void insert_free_nodes(int offset,FILE *fp); // function to insert the free nodes into linked list  by 

// utilizing the nodes of the tree and its right_offset.
    
    1.Understand the way a tree can be made on the harddisk 
    2.It also explains the way modern DBMS work 
HOW TO COMPILE AND EXECUTE

$ gcc client_file.c A4_PES1201800152.c
$ ./a.out                                                                       on Linux.

