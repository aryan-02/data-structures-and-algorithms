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

    printf("\n\nBefore Rotation\n\n");

    printf("========== Tree view ==========\n");
    printTree(tree, INDENT_STEP);
    printf("===== In-Order Traversal ======\n");
    inOrderTraversal(tree);
    printf("\nLeft rotate on 12\n");

    leftRotate(&(tree -> left -> right));
    assert(validateParents(tree));

    printf("\nAfter Rotation\n\n");
    printf("========== Tree view ==========\n");
    printTree(tree, INDENT_STEP);
    printf("===== In-Order Traversal ======\n");
    inOrderTraversal(tree);

    printf("\nRight rotate on 50\n");

    rightRotate(&tree);
    assert(validateParents(tree));

    printf("========== Tree view ==========\n");
    printTree(tree, INDENT_STEP);
    printf("===== In-Order Traversal ======\n");
    inOrderTraversal(tree);


    return 0;
}