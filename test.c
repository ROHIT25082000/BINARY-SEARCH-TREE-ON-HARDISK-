#include<stdio.h>
#include<stdlib.h>
#include"assignment_4.h"

int main()
{
    FILE *fp;
    fp = init_tree("rohitanade.dat");
    int key;
    int op;
    printf("1 .Insert key:\n");
    printf("2  .delete\n");
    printf("3 .traversals\n");

    while(1)
    {
        printf("\nEnter the option:\n");
        scanf("%d",&op);
        switch(op)
        {
            case 1:
                printf("Enter the number :");
                scanf("%d",&key);
                insert_key(key,fp);
                break;
            case 2:
                printf("Enter the number to delete:");
                scanf("%d",&key);
                delete_key(key,fp);
                break;
            case 3:
                printf("Inorder traversal\n");
                display_inorder(fp);
                printf("\n");
                printf("Preorder traveresal\n");
                display_preorder(fp);
                break;
            case 4:
                fclose(fp);
                exit(1);
            default:
                printf("Invalid option\n");
        }

    }
}