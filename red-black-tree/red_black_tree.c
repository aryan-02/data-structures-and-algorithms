#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#define INDENT_STEP 4

#define DEBUG 1

typedef enum _color
{
    RED, BLACK
}
COLOR;

typedef struct NODE
{
    int data;
    struct NODE *left;
    struct NODE *right;
    struct NODE *parent;
    COLOR color;
}
NODE;

// Function signatures
int max(int a, int b);
void rightRotate(NODE **root);
void leftRotate(NODE **root);
void printTree(NODE *root, int indent);
bool validateParents(NODE *root);
NODE **getPointerToNode(NODE **root, NODE *node);
NODE *newNode(int data, NODE *left, NODE *right, NODE *parent);
COLOR color(NODE *node);
void fixRedBlackPropertyUp(NODE **root, NODE *node);

COLOR color(NODE *node)
{
    if(node == NULL)
    {
        return BLACK;
    }
    else
    {
        return node -> color;
    }
}

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
        // Impossible case, if parent-child pointers are consistent
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
    node -> color = RED;
    return node;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

NODE *successor(NODE *root)
{
    if(root -> right == NULL)
    {
        return NULL;
    }
    else
    {
        // Successor is the smallest element in the right subtree
        // Go as far down left as possible in the root -> right subtree
        NODE *tempPtr = root -> right;
        while(tempPtr -> left != NULL)
        {
            tempPtr = tempPtr -> left;
        }
        return tempPtr;
    }
}



void insert(NODE **root, int value)
{
    printf("\nINSERT %d\n", value);
    // Tree is empty
    if(*root == NULL)
    {
        *root = newNode(value, NULL, NULL, NULL);
        fixRedBlackPropertyUp(root, *root);
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
            else
            {
                fprintf(stderr, "Duplicate value %d found. Not inserting.\n", value);
                return;
            } 
        }

        // Value inserted
        fixRedBlackPropertyUp(root, tempPtr);
        
    }
}

void delete(NODE **root, NODE *node)
{
    NODE **link = getPointerToNode(root, node);
    NODE *parentOfDeletedChild = node -> parent;

    if(node -> left == NULL && node -> right == NULL) // deleting leaf node
    {
        free(*link);
        *link = NULL;
        if(parentOfDeletedChild != NULL)
        {
            //updateHeightsUp(parentOfDeletedChild);
            //rebalanceUp(root, parentOfDeletedChild);
        }
    }
    else // deleting non-leaf node
    {
        if(node -> left == NULL || node -> right == NULL) // non leaf node but only one child
        {
            NODE *newBranch;
            if(node -> right == NULL) // only has left child
            {
                newBranch = node -> left;
            }
            else // only has right child
            {
                newBranch = node -> right;
            }
            newBranch -> parent = node -> parent;
            *link = newBranch;
            free(node);
            node = NULL;

            if(parentOfDeletedChild != NULL)
            {
                //updateHeightsUp(parentOfDeletedChild);
                //rebalanceUp(root, parentOfDeletedChild);
            }
        }
        else // internal node with two children
        {
            NODE *s = successor(node);
            NODE *successorsParent = s -> parent;
            // successor can't have left child; it can only have a right child
            NODE **successorLink = getPointerToNode(root, s);
            if(s == node -> right) // Successor is the immediate right child
            {
                s -> parent = node -> parent;
                s -> left = node -> left;
                s -> left -> parent = s;
                *link = s;
                free(node);
                //updateHeightsUp(s);
                //rebalanceUp(root, s);
            }
            else // Successor is deeper in the right subtree
            {
                *successorLink = s -> right;
                if(*successorLink != NULL)
                {
                    (*successorLink) -> parent = successorsParent;
                }
                

                // Now replace the node to delete with successor
                s -> parent = node -> parent;
                s -> left = node -> left;
                if(s -> left != NULL)
                {
                    s -> left -> parent = s;
                }
                s -> right = node -> right;
                if(s -> right != NULL)
                {
                    s -> right -> parent = s;
                }
                free(*link);
                *link = s;
                //updateHeightsUp(successorsParent);
                //rebalanceUp(root, successorsParent);
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

        //updateHeightsUp(x);

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

    //updateHeightsUp(y);
}

void inOrderTraversal(NODE *root)
{
    if(root != NULL)
    {
        inOrderTraversal(root -> left);
        printf("%d\n", root -> data);
        inOrderTraversal(root -> right);
    }
}

bool validateParents(NODE *root)
{
    if(root == NULL)
        return true;
    if(root -> left != NULL && root -> left -> parent != root)
    {
        printf("FAILED left AT node %p, value %d\n", root, root -> data);
        return false;
    }
        
    if(root -> right != NULL && root -> right -> parent != root)
    {
        printf("FAILED right AT node %p, value %d\n", root, root -> data);
        return false;
    }
    
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
    if(root -> color == RED)
        printf("\033[31m%d\033[0m\n", root->data);
    else
        printf("%d\n", root -> data);

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

NODE *search(NODE *root, int value)
{
    NODE *tempPtr = root;
    while(tempPtr != NULL && tempPtr -> data != value)
    {
        if(value < tempPtr -> data)
        {
            tempPtr = tempPtr -> left;
        }
        else if(value > tempPtr -> data)
        {
            tempPtr = tempPtr -> right;
        }
    }
    return tempPtr;
}

void fixRedBlackPropertyUp(NODE **root, NODE *node)
{
    if(node -> parent == NULL)
    {
        node -> color = BLACK;
    }
    if(color(node) == BLACK)
    {
        return;
    }
    if(color(node) == RED)
    {
        if(node -> parent == NULL)
        {
            // root node must be black
            node -> color = BLACK;
            return;
        }
        else if(color(node -> parent) == RED)
        {
            if(node -> parent -> parent != NULL) // node has a grandparent
            {
                if(node -> parent == node -> parent -> parent -> left) // node's parent is a left child
                {
                    NODE *uncle = node -> parent -> parent -> right;
                    if(color(uncle) == RED) // Uncle is red ==> Recolor
                    {
                        node -> parent -> color = BLACK;
                        uncle -> color = BLACK;
                        node -> parent -> parent -> color = RED;
                        fixRedBlackPropertyUp(root, node -> parent -> parent);
                    }
                    else // Uncle is black
                    {
                        if(node == node -> parent -> right) // node is a right child
                        {
                            node = node -> parent;
                            leftRotate(getPointerToNode(root, node));
                        }
                        node -> parent -> color = BLACK;
                        node -> parent -> parent -> color = RED;
                        rightRotate(getPointerToNode(root, node -> parent -> parent));
                    }
                }
                else if(node -> parent == node -> parent -> parent -> right) // node's parent is a right child
                {
                    NODE *uncle = node -> parent -> parent -> left;
                    if(color(uncle) == RED) // Uncle is red ==> Recolor
                    {
                        // Make parent and uncle black, grandparent red
                        node -> parent -> color = BLACK;
                        uncle -> color = BLACK;
                        node -> parent -> parent -> color = RED;
                        fixRedBlackPropertyUp(root, node -> parent -> parent);
                        (*root) -> color = BLACK;
                    }
                    else // Uncle is black
                    {
                        if(node == node -> parent -> left) // node is a left child
                        {
                            node = node -> parent;
                            rightRotate(getPointerToNode(root, node));
                        }
                        node -> parent -> color = BLACK;
                        node -> parent -> parent -> color = RED;
                        leftRotate(getPointerToNode(root, node -> parent -> parent));
                        (*root) -> color = BLACK;
                    }
                    
                }
            }
        }
    }

}

int main(void)
{
    NODE *tree = NULL;
    
    printf("---------------- AVL TREE ----------------\n\n");
    printf("List of commands:\ni K: insert number K in AVL Tree\ns K: search for number K in AVL Tree\nd: delete K if it exists\nq: quit\n\n");
    char commandType = '\0';
    while(1)
    {
        scanf(" %c", &commandType);
        int key;
        if(commandType == 'q')
        {
            break;
        }
        else if(commandType == 'i')
        {
            scanf("%d", &key);
            insert(&tree, key);
        }
        else if(commandType == 's')
        {
            scanf("%d", &key);
            NODE *searchResult = search(tree, key);
            printf("%s\n", (searchResult != NULL) ? "FOUND" : "NOT FOUND");
        }
        else if(commandType == 'd')
        {
            scanf("%d", &key);
            NODE *searchResult = search(tree, key);
            if(searchResult == NULL)
            {
                printf("NOT FOUND -- Can't delete.\n");
            }
            else
            {
                delete(&tree, searchResult);
            }
        }
        else
        {
            printf("Invalid command\n");
        }

        printTree(tree, INDENT_STEP);
        assert(validateParents(tree));
        
    }
    freeTree(tree);


    return 0;
}