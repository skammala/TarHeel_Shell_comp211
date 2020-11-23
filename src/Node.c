#include "Node.h"
#include "Guards.h"

Node* ErrorNode_new(const char *msg)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = ERROR_NODE;
    node->data.error = msg;
    return node;
}

Node* CommandNode_new(StrVec words)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = COMMAND_NODE;
    node->data.command = words;
    return node;
}

Node* PipeNode_new(Node *left, Node *right)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = PIPE_NODE;
    node->data.pipe.left = left;
    node->data.pipe.right = right;
    return node;
}

void* Node_drop(Node *self)
{
    switch (self->type) {
        case PIPE_NODE:
            Node_drop(self->data.pipe.left);
            Node_drop(self->data.pipe.right);
            free(self);
        case COMMAND_NODE:
            StrVec_drop(&self->data.command);
            free(self);
        case ERROR_NODE:
            free(self); 
    }
    return self;
}
