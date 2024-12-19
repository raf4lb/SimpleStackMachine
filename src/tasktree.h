#ifndef TASKTREE_H
#define TASKTREE_H
#include "task.h"

// Definition of the Task structure
typedef struct TaskTreeNode
{
    int id;                         // Task ID
    char name[10];                  // Task name
    struct TaskTreeNode **children; // Array of pointers to child nodes
    int childCount;                 // Number of children
    int childCapacity;              // Capacity of the children array
    struct TaskTreeNode *parent;    // Pointer to the parent node
    Task *task;
} TaskTreeNode;

TaskTreeNode *task_tree_create_node(Task *task);
TaskTreeNode *task_tree_add_child(TaskTreeNode *parent, Task *task);
void task_tree_remove_child(TaskTreeNode *parent, int id);
void task_tree_print(TaskTreeNode *root, int depth);
void task_tree_traverse_dfs(TaskTreeNode *root, void (*process)(TaskTreeNode *));
void task_tree_free(TaskTreeNode *root);
#endif