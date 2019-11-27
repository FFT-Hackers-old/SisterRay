#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "types.h"

typedef struct {
    ListNode* next;
    void* element;
} ListNode;

typedef struct {
    bool use_assert_alloc;
    u32 field_4;
    u32 nodeCount;
    ListNode *head;
    ListNode *tail
    void *destructor;
    void *recursiveFindCallback;
    u32 field_1C;
} List;

#endif // !DATA_STRUCTURES_H
