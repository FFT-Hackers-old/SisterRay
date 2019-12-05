#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "types.h"

typedef struct {
    void* next; //Type is GameListNode*, cast before usage
    void* element;
} GameListNode;

typedef struct {
    u32 useAssertAlloc;
    u32 field_4;
    u32 nodeCount;
    GameListNode *head;
    GameListNode *tail;
    void *destructor;
    void *recursiveFindCallback;
    u32 field_1C;
} GameList;

typedef struct {
    void* next; //type is GameHeap*, cast before usage
    u32 size;					
    void* last;	//type is GameHeap*, cast before usage	
    u32 field_C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 field_2C;
    u32 field_30;
    void *callbackData;
    void *callback;
} GameHeap;
#endif // !DATA_STRUCTURES_H
