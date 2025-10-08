#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#define INDENT_STEP 4

typedef struct NODE
{
    int data;
    struct NODE *left;
    struct NODE *right;
    struct NODE *parent;
    int height;
}
NODE;

void rightRotate(NODE **root);
void leftRotate(NODE **root);
void printTree(NODE *root, int indent);

NODE **getPointerToNode(NODE **root, NODE *node)
{
    if(node -> parent == NULL)
    {
        return root;
    }
    else if(node -> parent -> left == node)
    {
        return &(node -> parent -> left);
    }
    else if(node -> parent -> right == node)
    {
        return &(node -> parent -> right);
    }
    else
    {
        assert(0);
        return NULL;
    }
}


NODE *newNode(int data, NODE *left, NODE *right, NODE *parent)
{
    NODE *node = malloc(sizeof(NODE));
    node -> data = data;
    node -> left = left;
    node -> right = right;
    node -> parent = parent;
    node -> height = 0;
    return node;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int height(NODE *root)
{
    if(root == NULL)
    {
        return -1;
    }
    else
    {
        return root -> height;
    }
}

int balanceFactor(NODE *root)
{
    if(root == NULL)
    {
        return 0;
    }
    else
    {
        return height(root -> right) - height(root -> left);
    }
}

void updateHeightsUp(NODE *node)
{
    NODE *currentNode = node;
    while(currentNode != NULL)
    {
        int leftHeight = height(currentNode -> left);
        int rightHeight = height(currentNode -> right);
        currentNode -> height = max(leftHeight, rightHeight) + 1;
        
        currentNode = currentNode -> parent;
    }
}

/*
void rebalanceUp(NODE **node)
{
    NODE **tempPtr = node;
    while(*tempPtr != NULL)
    {
        int bf = balanceFactor(*tempPtr);
        if(bf > 1) // Right-heavy violation
        {
            printf("\nFound violation at node with key %d\n", (*tempPtr) -> data);
            printf("Balance factor: %d\n", bf);
            if(balanceFactor((*tempPtr) -> right) > 0) // Right child is right heavy
            {
                // Left Rotate
                leftRotate(tempPtr);
            }
            else // Right child is left-heavy
            {
                // Right-left Rotate
                rightRotate(&((*tempPtr) -> right));
                leftRotate(tempPtr);
            }
        }
        else if(bf < -1) // Left-heavy violation
        {
            printf("\nFound violation at node with key %d\n", (*tempPtr) -> data);
            printf("Balance factor: %d\n", bf);
            if(balanceFactor((*tempPtr) -> left) < 0)
            {
                // Right Rotate
                rightRotate(tempPtr);
            }
            else // Left child is right-heavy
            {
                // Left-right Rotate
                leftRotate(&((*tempPtr) -> left));
                rightRotate(tempPtr);
            }
        }
        tempPtr = &((*tempPtr) -> parent);
    }
}
*/

void insert(NODE **root, int value)
{
    printf("\nINSERT %d\n", value);
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
                    tempPtr = tempPtr -> left;
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
                    tempPtr = tempPtr -> right;
                    break;
                }
                else
                {
                    tempPtr = tempPtr -> right;
                }
            } 
        }

        // Value inserted

        // Propagate up, updating heights
        updateHeightsUp(tempPtr);

        // New node is inserted in place. Now, rebalance tree
        while(tempPtr != NULL)
        {
            int bf = balanceFactor(tempPtr);
            if(bf > 1) // Right heavy violation
            {
                printf("\n");
                printTree(*root, INDENT_STEP);
                printf("\nViolation at %d, balance factor is %d\n", tempPtr -> data, bf);
                if(balanceFactor(tempPtr -> right) > 0) // Right child is right heavy
                {
                    printf("Left rotate\n");
                    leftRotate(getPointerToNode(root, tempPtr));
                }
                else // Right child is left heavy
                {
                    // Right-left rotate
                    printf("Right-left rotate\n");
                    rightRotate(getPointerToNode(root, tempPtr -> right));
                    leftRotate(getPointerToNode(root, tempPtr));
                }
            }
            else if(bf < -1) // Left heavy violation
            {
                printf("\n");
                printTree(*root, INDENT_STEP);
                printf("\nViolation at %d, balance factor is %d\n", tempPtr -> data, bf);

                if(balanceFactor(tempPtr -> left) < 0) // Left child is left heavy
                {
                    printf("Right rotate\n");
                    rightRotate(getPointerToNode(root, tempPtr));
                }
                else // Left chold is right heavy
                {
                    // Left-right rotate
                    printf("Left-right rotate\n");
                    leftRotate(getPointerToNode(root, tempPtr -> left));
                    rightRotate(getPointerToNode(root, tempPtr));
                }
            }
            tempPtr = tempPtr -> parent;
        }
        
        
    }
}

/*

        x                                         y
      /   \            Left rotate on x         /   \         
    alpha  y           ------------------>     x   gamma
         /   \                               /   \
       beta  gamma                       alpha   beta
*/
void leftRotate(NODE **root)
{
    if((*root) -> right == NULL)
    {
        return; // Can't left-rotate if no right child
    }
    else
    {
        NODE *x = *root;
        NODE *parentOfRoot = x -> parent;
        NODE *y = x -> right;
        NODE *beta = y -> left;

        *root = y;
        y -> left = x;
        x -> right = beta;

        // Fix parent pointers
        if(beta != NULL)
            beta -> parent = x;
        x -> parent = y;
        (*root) -> parent = parentOfRoot;

        updateHeightsUp(x);

    }
}

/*

             y                                           x
           /   \                                       /   \
         x    gamma     Right rotate on y          alpha    y
       /   \           ------------------>                /   \
   alpha   beta                                        beta   gamma

*/
void rightRotate(NODE **root)
{
    if((*root) -> left == NULL)
    {
        return; // Can't right-rotate if no left child
    }
    NODE *y = *root;
    NODE *parentOfRoot = y -> parent;
    NODE *x = y -> left;
    NODE *beta = x -> right;

    *root = x;
    x -> right = y;
    y -> left = beta;

    // Fix parent pointers
    if(beta != NULL)
    {
        beta -> parent = y;
    }
    y -> parent = x;
    (*root) -> parent = parentOfRoot;

    updateHeightsUp(y);
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

bool validateParents(NODE *root)
{
    if(root == NULL)
        return true;
    if(root -> left != NULL && root -> left -> parent != root)
        return false;
    if(root -> right != NULL && root -> right -> parent != root)
        return false;
    
    if(! validateParents(root -> left) || ! validateParents(root -> right))
        return false;
    return true;
    
}

// Recursive function to print tree sideways
void printTree(NODE *root, int indent) 
{

    if (root == NULL)
        return;

    if(root -> parent == NULL)
    {
        printf("========== Tree view ==========\n");
    }
    // Print right subtree first
    printTree(root->right, indent + INDENT_STEP);

    // Print current node
    for (int i = 0; i < indent; i++)
        printf(" ");
    printf("%d\n", root->data);

    // Then print left subtree
    printTree(root->left, indent + INDENT_STEP);
}

void freeTree(NODE *root)
{
    if(root != NULL)
    {
        freeTree(root -> left);
        freeTree(root -> right);
        free(root);
    }
}

int main(void)
{
    int keys[] = {12, 24, 14, 27, 35, 17, 19, 22};
    NODE *tree = NULL;
    for(int i = 0; i < 8; i++)
    {
        insert(&tree, keys[i]);
        printTree(tree, INDENT_STEP);
    }
    
    printf("===== In-Order Traversal ======\n");
    inOrderTraversal(tree);

    freeTree(tree);
    return 0;
}