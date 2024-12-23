#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "sys.h"
#include "tasktree.h"

// Global variable to generate incremental IDs
int nextId = 0;

// Function to create a new task
TaskTreeNode *task_tree_create_node(Task *task)
{
    TaskTreeNode *new_task = (TaskTreeNode *)vmmalloc(sizeof(TaskTreeNode));
    if (!new_task)
    {
        vmprintf("Memory allocation failed!\n");
        return NULL;
    }
    new_task->id = nextId++;
    snprintf(new_task->name, sizeof(new_task->name), "Task %d", new_task->id);
    new_task->children = NULL;
    new_task->childCount = 0;
    new_task->childCapacity = 0;
    new_task->parent = NULL; // Initialize parent pointer
    new_task->task = task;
    // vmprintf("Created task %d\n", new_task->id);
    return new_task;
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
            vmprintf("Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    parent->children[parent->childCount++] = child;
    return child;
}

// Function to remove a child from a task by ID
void task_tree_remove_child(TaskTreeNode *node)
{
    TaskTreeNode *parent = node->parent;
    int id = node->id;
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
            // vmprintf("Ended task %d\n", id);
            return;
        }
    }
}

// Function to print the tree recursively
void task_tree_print(TaskTreeNode *root, int depth)
{
    if (root != NULL)
    {
        for (int i = 0; i < depth; i++)
        {
            vmprintf("  ");
        }
        vmprintf("Task ID: %d, Name: %s\n", root->id, root->name);
        for (int i = 0; i < root->childCount; i++)
        {
            task_tree_print(root->children[i], depth + 1);
        }
    }
}

// Function to perform a Depth-First Search traversal
void task_tree_traverse_dfs(CPU *cpu, TaskTreeNode *root, void (*process)(CPU *cpu, TaskTreeNode *))
{
    if (root == NULL)
    {
        return;
    }
    process(cpu, root); // Process the current node
    for (int i = 0; i < root->childCount; i++)
    {
        task_tree_traverse_dfs(cpu, root->children[i], process);
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
        vmfree(root->children);
        vmfree(root);
    }
}