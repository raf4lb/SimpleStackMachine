#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys.h"
#include "tasktree.h"

// Global variable to generate incremental IDs
int nextId = 1;

// Function to create a new task
TaskTreeNode *task_tree_create_node(Task *task)
{
    TaskTreeNode *newTask = (TaskTreeNode *)vmmalloc(sizeof(TaskTreeNode));
    if (!newTask)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newTask->id = nextId++;
    snprintf(newTask->name, sizeof(newTask->name), "Task %d", newTask->id);
    newTask->children = NULL;
    newTask->childCount = 0;
    newTask->childCapacity = 0;
    newTask->parent = NULL; // Initialize parent pointer
    newTask->task = task;
    return newTask;
}

// Function to add a child to a task
TaskTreeNode *task_tree_add_child(TaskTreeNode *parent, Task *task)
{
    TaskTreeNode *child = task_tree_create_node(task);
    child->parent = parent; // Set the parent reference
    if (parent->childCount == parent->childCapacity)
    {
        parent->childCapacity = (parent->childCapacity == 0) ? 2 : parent->childCapacity * 2;
        parent->children = (TaskTreeNode **)realloc(parent->children, parent->childCapacity * sizeof(TaskTreeNode *));
        if (!parent->children)
        {
            printf("Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    parent->children[parent->childCount++] = child;
    return child;
}

// Function to remove a child from a task by ID
void task_tree_remove_child(TaskTreeNode *parent, int id)
{
    for (int i = 0; i < parent->childCount; i++)
    {
        if (parent->children[i]->id == id)
        {
            task_tree_free(parent->children[i]);
            for (int j = i; j < parent->childCount - 1; j++)
            {
                parent->children[j] = parent->children[j + 1];
            }
            parent->childCount--;
            return;
        }
    }
    printf("Child with ID %d not found.\n", id);
}

// Function to print the tree recursively
void task_tree_print(TaskTreeNode *root, int depth)
{
    if (root != NULL)
    {
        for (int i = 0; i < depth; i++)
        {
            printf("  ");
        }
        printf("Task ID: %d, Name: %s\n", root->id, root->name);
        for (int i = 0; i < root->childCount; i++)
        {
            task_tree_print(root->children[i], depth + 1);
        }
    }
}

// Function to perform a Depth-First Search traversal
void task_tree_traverse_dfs(TaskTreeNode *root, void (*process)(TaskTreeNode *))
{
    if (root == NULL)
    {
        return;
    }
    process(root); // Process the current node
    for (int i = 0; i < root->childCount; i++)
    {
        task_tree_traverse_dfs(root->children[i], process);
    }
}

// Function to free the tree recursively
void task_tree_free(TaskTreeNode *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->childCount; i++)
        {
            task_tree_free(root->children[i]);
        }
        task_free(root->task);
        free(root->children);
        free(root);
    }
}

// Main function for testing
// int example()
// {
//     TaskTreeNode *root = task_tree_create_node();

//     TaskTreeNode *child1 = task_tree_add_child(root);
//     TaskTreeNode *child2 = task_tree_add_child(root);
//     TaskTreeNode *child3 = task_tree_add_child(child1);

//     printf("Tree structure:\n");
//     task_tree_print(root, 0);

//     if (child3->parent != NULL)
//     {
//         printf("\nParent of Task %d: ID = %d, Name = %s\n", child3->id, child3->parent->id, child3->parent->name);
//     }

//     task_tree_remove_child(root, child2->id);

//     printf("\nTree structure after removing Child 2:\n");
//     task_tree_print(root, 0);

//     task_tree_free(root);
//     return 0;
// }