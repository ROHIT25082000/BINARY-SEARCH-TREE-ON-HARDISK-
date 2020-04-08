#include<stdio.h>
#include<stdlib.h>
#include"assignment_4.h"
tree_t mytree;
tree_t *mytree_ptr = &mytree;   // pointer to the global tree.

FILE* init_tree(const char* filename)       // all done
{
    FILE*fp;
    fp = fopen(filename,"r+");
    if(fp == NULL)
    {
        fp = fopen(filename,"w");
        mytree_ptr->free_head = -1;
        mytree_ptr->root = -1;
        fwrite(mytree_ptr,sizeof(tree_t),1,fp);
        fclose(fp);
        fopen(filename,"r+");
    }
   // printf("The size of mytree %ld\n",sizeof(mytree));
    fread(mytree_ptr,sizeof(tree_t),1,fp);
    //printf("%ld\n",ftell(fp));
    return fp;
}

// close the file pointed by fp
void close_tree(FILE *fp)           // all done
{
    fclose(fp);
}
void inorder(int root,FILE *fp)     // all done
{
    if(root == -1)
        return;
    else 
    {
        node_t temp;
        fseek(fp,root,SEEK_SET);
        fread(&temp,sizeof(node_t),1,fp);
        inorder(temp.left_offset,fp);
        printf("%d ",temp.key);
        inorder(temp.right_offset,fp);
    }
}
// Space separated keys. Last key followed by '\n'
void display_inorder(FILE *fp)      // all done
{
    fseek(fp,0,SEEK_SET);
    fread(mytree_ptr,sizeof(tree_t),1,fp);
    inorder(mytree_ptr->root,fp);
    printf("\n");
}

void preorder(int root,FILE *fp)        // all done
{
    if(root == -1)
        return;
    else 
    {
        node_t temp;
        fseek(fp,root,SEEK_SET);
        fread(&temp,sizeof(node_t),1,fp);
        printf("%d ",temp.key);
        preorder(temp.left_offset,fp);
        preorder(temp.right_offset,fp);
    }
   
}
// Space separated keys. Last key followed by '\n'
void display_preorder(FILE *fp)         // all done
{
    fseek(fp,0,SEEK_SET);
    fread(mytree_ptr,sizeof(tree_t),1,fp);
    preorder(mytree_ptr->root,fp);
    printf("\n");
}
// insert the key into the tree in the file
// if the key already exists just return

void insert_key(int key, FILE *fp)
{
    node_t new_node;
    new_node.key = key;
    new_node.left_offset = -1;
    new_node.right_offset = -1;

    int off;
    node_t temp;        // reading the tree structure 
    fseek(fp,0,SEEK_SET);
    fread(mytree_ptr,sizeof(tree_t),1,fp);
    if(mytree_ptr->free_head == -1)
    {
        fseek(fp,0,SEEK_END);   // SEEK to the end;
        off = ftell(fp);
       // printf("off1 :%d\n",off);
    }   
    else
    {                               // find the position of the freenode
        off = mytree_ptr->free_head;
        //printf("off2 %d\n",off);
        fseek(fp,off,SEEK_SET);
        fread(&temp,sizeof(node_t),1,fp);
        mytree_ptr->free_head = temp.right_offset;
    }
   // now traverse the tree 
    if(mytree_ptr->root == -1)
    {
        mytree_ptr->root = off;
    }
    else
    {
        node_t curr_node;
        node_t prev_node;

        int prev = -1;
        int curr = mytree_ptr->root;
        while(curr != -1)
        {
            fseek(fp,curr,SEEK_SET);
            // read only if the value is not = -1
            if(curr!=-1){
                fread(&curr_node,sizeof(node_t),1,fp);
            }
            prev = curr;
            prev_node = curr_node;
            if(key < curr_node.key)
            { 
                curr = curr_node.left_offset;
            }
            else if( key > curr_node.key)
            {   
                curr = curr_node.right_offset;
            }
            else if(key == curr_node.key)
            {
                return;
            } 
        }
        // now we have to attach the new node based on the position
        if(key < prev_node.key)
        {
            prev_node.left_offset = off;
           // updated the link of the prev_node left lin
        }
        else if(key > prev_node.key)
        {
            prev_node.right_offset = off;
        }
        fseek(fp,prev,SEEK_SET);
        fwrite(&prev_node,sizeof(node_t),1,fp);
    }
    fseek(fp, 0, SEEK_SET);
    fwrite(mytree_ptr, sizeof(tree_t), 1, fp);
    fseek(fp,off,SEEK_SET);
    fwrite(&new_node,sizeof(node_t),1,fp);
}

int findMax(int curr,int *inner_prev,FILE *fp)
{
    node_t temp;
    int prev = -1;
    while(curr != -1)
    {
        fseek(fp,curr,SEEK_SET);
        fread(&temp,sizeof(node_t),1,fp);
        *inner_prev = prev;
        prev = curr;
        curr = temp.right_offset;
    }
    return prev;
}
// delete the key from the tree in the file
// the key may or may not exist
void insert_free_nodes(int offset,FILE *fp)                        // function to insert the free nodes into this.
{
    node_t temp;

     // reading the mytree pointer 
    fseek(fp,0,SEEK_SET);
    fread(mytree_ptr,sizeof(tree_t),1,fp);

    if(mytree_ptr->free_head == -1)
    {
        mytree_ptr->free_head = offset;
        fseek(fp,0,SEEK_SET);
        fwrite(mytree_ptr,sizeof(tree_t),1,fp);
    }
    else
    {
        fseek(fp,offset,SEEK_SET);                  // join the free node
        fread(&temp,sizeof(node_t),1,fp);
        temp.right_offset = mytree_ptr->free_head;
        mytree_ptr->free_head = offset;

        // update in the mytree
        fseek(fp,0,SEEK_SET);
        fwrite(mytree_ptr,sizeof(tree_t),1,fp);
    }
}

void delete_key(int key, FILE *fp)
{
    node_t temp;
    
    // reading the mytree pointer 
    fseek(fp,0,SEEK_SET);
    fread(mytree_ptr,sizeof(tree_t),1,fp);

    int prev = -1;
    int curr = mytree_ptr->root;

    // 1. step find the leafnode;
    if(curr == -1)  // the root is null.
        return;


    while(curr!=-1)
    {
        fseek(fp,curr,SEEK_SET);
        fread(&temp,sizeof(node_t),1,fp);
        if(key < temp.key)
        {
            prev = curr;
            curr = temp.left_offset;
        }
        else if(key > temp.key)
        {
            prev = curr;
            curr = temp.right_offset;
        }
        else if(key == temp.key)
        {
            break;
        }
    }
    if(curr == -1)   // element not found .
    {
        return;
    }
    if(curr != -1)  // element found here
    {
        node_t prev_temp;
        fseek(fp,curr,SEEK_SET);
        fread(&temp,sizeof(node_t),1,fp);
        if(temp.left_offset == -1 && temp.right_offset == -1)   // leaf node case       1.)***********************************
        {
            //printf("leaf node\n");
            if(prev == -1)              // only the root node present
            {
                fseek(fp,0,SEEK_SET);
                fread(mytree_ptr,sizeof(tree_t),1,fp);
                mytree_ptr->free_head = -1;
                mytree_ptr->root = -1;
                fseek(fp,0,SEEK_SET);
                fwrite(mytree_ptr,sizeof(tree_t),1,fp);
                
                insert_free_nodes(curr,fp);

            }
            else{
            fseek(fp,prev,SEEK_SET);
            fread(&prev_temp,sizeof(node_t),1,fp);
            if(prev_temp.left_offset == curr)   // determining the prev_temps right or left child .
            {
                //printf("I bcam left\n");
                prev_temp.left_offset = -1;
            }
            else if(prev_temp.right_offset == curr) // making the prev_temp as null (-1)
            {
                //printf("I became right\n");

                prev_temp.right_offset = -1;
            }
            fseek(fp,prev,SEEK_SET);
            fwrite(&prev_temp,sizeof(node_t),1,fp);

            insert_free_nodes(curr,fp);
            }
            // we  have to free the curr and attach it in the free list .
        }
        
        // If the node is has one child                                                 2.)***********************************
        if(temp.right_offset == -1 && temp.left_offset != -1)
        {
            //printf("One child\n");
            if(prev == -1)              // only one root and a child 
            {
                fseek(fp,0,SEEK_SET);
                fread(mytree_ptr,sizeof(tree_t),1,fp);
                mytree_ptr->root = temp.left_offset;
                fseek(fp,0,SEEK_SET);
                fwrite(mytree_ptr,sizeof(tree_t),1,fp);
                temp.left_offset = -1;
                fseek(fp,curr,SEEK_SET);
                fwrite(&temp,sizeof(node_t),1,fp);
                insert_free_nodes(curr,fp);
            }
            else{
            fseek(fp,prev,SEEK_SET);
            fread(&prev_temp,sizeof(node_t),1,fp);
            if(curr == prev_temp.right_offset)
            {
                prev_temp.right_offset = temp.left_offset;
            }
            if(curr == prev_temp.left_offset)
            {
                prev_temp.left_offset = temp.left_offset;
            }
            fseek(fp,prev,SEEK_SET);
            fwrite(&prev_temp,sizeof(node_t),1,fp);


            temp.left_offset = -1;
            fseek(fp,curr,SEEK_SET);
            fwrite(&temp,sizeof(node_t),1,fp);
            insert_free_nodes(curr,fp);
            }
        }
        else if(temp.right_offset !=-1 && temp.left_offset== -1)    // Both are null in nature. has two children.
        {
            //sprintf("Two children\n");
            if(prev == -1)
            {
                fseek(fp,0,SEEK_SET);
                fread(mytree_ptr,sizeof(tree_t),1,fp);
                mytree_ptr->root = temp.right_offset;
                fseek(fp,0,SEEK_SET);
                fwrite(mytree_ptr,sizeof(tree_t),1,fp);
                temp.right_offset = -1;
                fseek(fp,curr,SEEK_SET);
                fwrite(&temp,sizeof(node_t),1,fp);
                insert_free_nodes(curr,fp);
            }
            else{
            fseek(fp,prev,SEEK_SET);
            fread(&prev_temp,sizeof(node_t),1,fp);
            if(curr == prev_temp.right_offset)
            {
                prev_temp.right_offset = temp.right_offset;
            }
            if(curr == prev_temp.left_offset)
            {
                prev_temp.left_offset = temp.right_offset;
            }
            fseek(fp,prev,SEEK_SET);
            fwrite(&prev_temp,sizeof(node_t),1,fp);
            temp.right_offset = -1;
            fseek(fp,curr,SEEK_SET);
            fwrite(&temp,sizeof(node_t),1,fp);
            insert_free_nodes(curr,fp);
            }
        }
        if(temp.right_offset != -1 && temp.left_offset != -1)
        {   /*
            node_t k;                                      
            node_t joker;
            int see_large = findMax(temp.left_offset,fp);
            fseek(fp,see_large,SEEK_SET);
            fread(&joker,sizeof(node_t),1,fp);
            delete_key(joker.key,fp);
            fseek(fp,curr,SEEK_SET);
            fread(&k,sizeof(node_t),1,fp);
            k.key = joker.key;
            fseek(fp,curr,SEEK_SET);
            fwrite(&k,sizeof(node_t),1,fp);*/
           /* if(prev == -1)
            {
                node_t joker;
                int inner_prev;
                int *t = &inner_prev;
                int inner_curr = findMax(temp.left_offset,t,fp);

                fseek(fp,inner_curr,SEEK_SET);
                fread(&joker,sizeof(node_t),1,fp);
                temp.key = joker.key;

                fseek(fp,curr,SEEK_SET);
                fwrite(&temp,sizeof(node_t),1,fp);

                if(inner_curr == temp.left_offset && inner_prev == -1)


            }
            else{*/
                node_t joker;
                int inner_prev;
                int *t = &inner_prev;
                int inner_curr = findMax(temp.left_offset,t,fp);
                
                fseek(fp,inner_curr,SEEK_SET);
                fread(&joker,sizeof(node_t),1,fp);
                temp.key = joker.key;               // copy the largest value in leftsubtree.
                
                fseek(fp,curr,SEEK_SET);
                fwrite(&temp,sizeof(node_t),1,fp);

                int parent_left_subtree = curr;     // offset of the parent of the left sub tree    PARENT POINTER OFFSET
                
                if(inner_curr == temp.left_offset  && inner_prev == -1)
                {
                    fseek(fp,inner_curr,SEEK_SET);
                    fread(&joker,sizeof(node_t),1,fp);

                    temp.left_offset = joker.left_offset;

                    fseek(fp,curr,SEEK_SET);
                    fwrite(&temp,sizeof(node_t),1,fp);

                    joker.left_offset = -1;
                    fseek(fp,inner_curr,SEEK_SET);
                    fwrite(&joker,sizeof(node_t),1,fp);
                    insert_free_nodes(inner_curr,fp);
                }
                else if(inner_curr != temp.left_offset && inner_prev !=-1)
                {
                    node_t last;
                    fseek(fp,inner_curr,SEEK_SET);
                    fread(&last,sizeof(node_t),1,fp);   // read the inner_curr in last

                    fseek(fp,inner_prev,SEEK_SET); 
                    fread(&joker,sizeof(node_t),1,fp);
                    joker.right_offset = last.left_offset;
                    last.left_offset = -1;
                    
                    fseek(fp,inner_curr,SEEK_SET);
                    fwrite(&last,sizeof(node_t),1,fp);

                    fseek(fp,inner_prev,SEEK_SET);
                    fwrite(&joker,sizeof(node_t),1,fp);

                    insert_free_nodes(inner_curr,fp);
                }
        }
    }
}
