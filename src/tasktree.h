#ifndef TASKTREE_H
#define TASKTREE_H
#include "task.h"

typedef struct CPU CPU;

typedef struct TaskTreeNode TaskTreeNode;
typedef struct TaskTreeNode
{
    uint16_t id;
    char name[10];
    struct TaskTreeNode **children;
    uint16_t childCount;
    uint16_t childCapacity;
    struct TaskTreeNode *parent;
    Task *task;
} TaskTreeNode;

TaskTreeNode *task_tree_create_node(Task *task);
TaskTreeNode *task_tree_add_child(TaskTreeNode *parent, Task *task);
void task_tree_remove_child(TaskTreeNode *node);
void task_tree_print(TaskTreeNode *root, int depth);
void task_tree_traverse_dfs(CPU *cpu, TaskTreeNode *root, void (*process)(CPU *cpu, TaskTreeNode *));
void task_tree_free(TaskTreeNode *root);
#endif