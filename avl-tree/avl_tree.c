#include <stdio.h>
#include <stdlib.h>
#define INDENT_STEP 4

typedef struct NODE
{
    int data;
    struct NODE *left;
    struct NODE *right;
    struct NODE *parent;
}
NODE;

NODE *newNode(int data, NODE *left, NODE *right, NODE *parent)
{
    NODE *node = malloc(sizeof(NODE));
    node -> data = data;
    node -> left = left;
    node -> right = right;
    node -> parent = parent;
    return node;
}

int height(NODE *root)
{
    if(root == NULL)
    {
        return 0;
    }
    else
    {
        int leftHeight = height(root -> left);
        int rightHeight = height(root -> right);
        int nodeHeight;
        if(leftHeight > rightHeight)
        {
            nodeHeight = leftHeight + 1;
        }
        else
        {
            nodeHeight = rightHeight + 1;
        }
        return nodeHeight;
    }
}

void insert(NODE **root, int value)
{
    // Tree is empty
    if(*root == NULL)
    {
        *root = newNode(value, NULL, NULL, NULL);
    }
    else
    {
        // Tree is nonempty
        NODE *tempPtr = *root;
        while(1)
        {
            if(value < (tempPtr -> data))
            {
                // Insert in left subtree
                if(tempPtr -> left == NULL)
                {
                    tempPtr -> left = newNode(value, NULL, NULL, tempPtr);
                    break;
                }
                else
                {
                    tempPtr = tempPtr -> left;
                }
            }
            else if(value > (tempPtr -> data))
            {
                // Insert in right subtree
                if(tempPtr -> right == NULL)
                {
                    tempPtr -> right = newNode(value, NULL, NULL, tempPtr);
                    break;
                }
                else
                {
                    tempPtr = tempPtr -> right;
                }
            }
        }
    }
}



void inOrderTraversal(NODE *root)
{
    if(root != NULL)
    {
        inOrderTraversal(root -> left);
        printf("%d [h = %d]\n", root -> data, height(root));
        inOrderTraversal(root -> right);
    }
}

// Recursive function to print tree sideways
void printTree(NODE* root, int indent) 
{
    if (root == NULL)
        return;

    // Print right subtree first
    printTree(root->right, indent + INDENT_STEP);

    // Print current node
    for (int i = 0; i < indent; i++)
        printf(" ");
    printf("%d\n", root->data);

    // Then print left subtree
    printTree(root->left, indent + INDENT_STEP);
}


int main(void)
{
    NODE *tree = NULL;
    insert(&tree, 50);
    insert(&tree, 10);
    insert(&tree, 12);
    insert(&tree, 9);
    insert(&tree, 35);
    insert(&tree, 72);
    insert(&tree, 48);
    // inOrderTraversal(tree);
    printf("========== Tree view ==========\n");
    printTree(tree, INDENT_STEP);
    printf("===== In-Order Traversal ======\n");
    inOrderTraversal(tree);

    return 0;
}