
#include <stdio.h>
#include <stdlib.h>

#define MAX 3

struct Node {
    int keys[MAX + 1];
    struct Node *child[MAX + 2];
    int n;
    int leaf;
    struct Node *next;
};

struct Node *root = NULL;

struct Node *createNode(int leaf) {
    struct Node *node = malloc(sizeof(struct Node));

    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    node->n = 0;
    node->leaf = leaf;
    node->next = NULL;

    for (int i = 0; i < MAX + 2; i++)
        node->child[i] = NULL;

    return node;
}

struct Node *findLeaf(int key) {
    struct Node *cur = root;

    while (cur != NULL && !cur->leaf) {
        int i = 0;

        while (i < cur->n && key >= cur->keys[i])
            i++;

        cur = cur->child[i];
    }

    return cur;
}

void insertLeaf(struct Node *leaf, int key) {
    int i = leaf->n - 1;

    while (i >= 0 && leaf->keys[i] > key) {
        leaf->keys[i + 1] = leaf->keys[i];
        i--;
    }

    leaf->keys[i + 1] = key;
    leaf->n++;
}

void insert(int key) {
    if (root == NULL) {
        root = createNode(1);
        root->keys[0] = key;
        root->n = 1;
        return;
    }

    struct Node *leaf = findLeaf(key);

    for (int i = 0; i < leaf->n; i++) {
        if (leaf->keys[i] == key)
            return;
    }

    if (leaf->n < MAX) {
        insertLeaf(leaf, key);
        return;
    }

    int temp[MAX + 1];
    int i = 0, j = 0;

    while (i < leaf->n && leaf->keys[i] < key)
        temp[j++] = leaf->keys[i++];

    temp[j++] = key;

    while (i < leaf->n)
        temp[j++] = leaf->keys[i++];

    struct Node *newLeaf = createNode(1);
    leaf->n = (MAX + 1) / 2;

    for (i = 0; i < leaf->n; i++)
        leaf->keys[i] = temp[i];

    newLeaf->n = MAX + 1 - leaf->n;

    for (i = 0; i < newLeaf->n; i++)
        newLeaf->keys[i] = temp[leaf->n + i];

    newLeaf->next = leaf->next;
    leaf->next = newLeaf;

    int separator = newLeaf->keys[0];

    if (root == leaf) {
        struct Node *newRoot = createNode(0);
        newRoot->keys[0] = separator;
        newRoot->child[0] = leaf;
        newRoot->child[1] = newLeaf;
        newRoot->n = 1;
        root = newRoot;
    } else {
        printf("Internal-node split required; insertion stopped.\n");
    }
}

void displayLeaves(void) {
    if (root == NULL) {
        printf("Tree is empty\n");
        return;
    }

    struct Node *cur = root;

    while (!cur->leaf)
        cur = cur->child[0];

    printf("B+ Tree leaf nodes: ");

    while (cur != NULL) {
        printf("[");
        for (int i = 0; i < cur->n; i++)
            printf("%d ", cur->keys[i]);
        printf("] ");
        cur = cur->next;
    }

    printf("\n");
}

void search(int key) {
    struct Node *leaf = findLeaf(key);

    if (leaf != NULL) {
        for (int i = 0; i < leaf->n; i++) {
            if (leaf->keys[i] == key) {
                printf("%d found\n", key);
                return;
            }
        }
    }

    printf("%d not found\n", key);
}

int main(void) {
    int choice, key;

    while (1) {
        printf("\n1. Insert\n");
        printf("2. Display leaves\n");
        printf("3. Search\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
            break;

        switch (choice) {
            case 1:
                printf("Enter key: ");
                if (scanf("%d", &key) != 1)
                    return 1;
                insert(key);
                break;

            case 2:
                displayLeaves();
                break;

            case 3:
                printf("Enter key to search: ");
                if (scanf("%d", &key) != 1)
                    return 1;
                search(key);
                break;

            case 4:
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}
