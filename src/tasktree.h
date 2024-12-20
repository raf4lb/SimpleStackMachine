#ifndef TASKTREE_H
#define TASKTREE_H
#include "task.h"

typedef struct CPU CPU;

typedef struct TaskTreeNode TaskTreeNode;
struct TaskTreeNode
{
    int id;                         // Task ID
    char name[10];                  // Task name
    struct TaskTreeNode **children; // Array of pointers to child nodes
    int childCount;                 // Number of children
    int childCapacity;              // Capacity of the children array
    struct TaskTreeNode *parent;    // Pointer to the parent node
    Task *task;
};

TaskTreeNode *task_tree_create_node(Task *task);
TaskTreeNode *task_tree_add_child(TaskTreeNode *parent, Task *task);
void task_tree_remove_child(TaskTreeNode *node);
void task_tree_print(TaskTreeNode *root, int depth);
void task_tree_traverse_dfs(CPU *cpu, TaskTreeNode *root, void (*process)(CPU *cpu, TaskTreeNode *));
void task_tree_free(TaskTreeNode *root);
#endif