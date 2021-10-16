/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: List utils
 * 
 * Change Logs:
 * Date            Author             Notes
 * 2021-10-16     JasonHu            Init
 */

#ifndef __UTILS_LIST__
#define __UTILS_LIST__

#include <XBook.h>

struct List
{
    struct List *prev;
    struct List *next;
};
typedef struct List List;

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
        List name = LIST_HEAD_INIT(name)

INLINE void ListInit(List *list)
{
    list->next = list;
    list->prev = list;  
}

INLINE void __ListAdd(List *list, List *prev, List *next)
{
    next->prev = list; 
    list->next = next; 
    list->prev = prev; 
    prev->next = list; 
}

INLINE void ListAdd(List *list, List *head)
{
    __ListAdd(list, head, head->next);
}

INLINE void ListAddBefore(List *list, List *node)
{
    node->prev->next = list;

    list->prev = node->prev;
    list->next = node;

    node->prev = list;
}

INLINE void ListAddAfter(List *list, List *node)
{
    node->next->prev = list;

    list->prev = node;
    list->next = node->next;

    node->next = list;
}

INLINE void ListAddTail(List *list, List *head)
{
    __ListAdd(list, head->prev, head);
}

INLINE void __ListDel(List *prev, List *next)
{
    next->prev = prev;
    prev->next = next;
}

INLINE void __ListDelNode(List *node)
{
    __ListDel(node->prev, node->next);
}

INLINE void ListDel(List *node)
{
    __ListDelNode(node);
    node->prev = (List *)NULL;
    node->next = (List *)NULL;
}

INLINE void ListDelInit(List *node)
{
    __ListDelNode(node);
    ListInit(node);
}

INLINE void ListReplace(List *old, List *list)
{
    list->next = old->next;
    list->next->prev = list;
    list->prev = old->prev;
    list->prev->next = list;
}

INLINE void ListReplaceInit(List *old, List *list)
{
    ListReplace(old, list);
    ListInit(old);
}

INLINE void ListMove(List *node, List *head)
{
    __ListDelNode(node);
    ListAdd(node, head);    
}

INLINE void ListMoveTail(List *node, List *head)
{
    __ListDelNode(node);
    ListAddTail(node, head);    
}

INLINE int ListIsFirst(const List *node, const List *head)
{
    return (node->prev == head);
}

INLINE int ListIsLast(const List *node, const List *head)
{
    return (node->next == head);
}

INLINE int ListEmpty(const List *head)
{
    return (head->next == head);
}

#define ListOwner(ptr, type, member) PtrOfStruct(ptr, type, member)

#define ListFirstOwner(head, type, member) ListOwner((head)->next, type, member)

#define ListLastOwner(head, type, member) ListOwner((head)->prev, type, member)

#define ListFirstOwnerOrNULL(head, type, member) ({ \
        List *__head = (head); \
        List *__pos = (__head->next); \
        __pos != __head ? ListOwner(__pos, type, member) : NULL; \
})

#define ListLastOwnerOrNULL(head, type, member) ({ \
        List *__head = (head); \
        List *__pos = (__head->prev); \
        __pos != __head ? ListOwner(__pos, type, member) : NULL; \
})

#define ListNextOwner(pos, member) \
       ListOwner((pos)->member.next, typeof(*(pos)), member)

#define ListPrevOnwer(pos, member) \
       ListOwner((pos)->member.prev, typeof(*(pos)), member)

#define ListForEach(pos, head) \
       for (pos = (head)->next; pos != (head); pos = pos->next)

INLINE int ListLength(List *head)
{
    List *list;
    int n = 0;
    ListForEach(list, head)
    {
        if (list == head)
        {
            break;
        }
        n++;
    }
    return n;
}

#define ListForEachPrev(pos, head) \
       for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define ListForEachSafe(pos, _next, head) \
        for (pos = (head)->next, _next = pos->next; pos != (head); \
            pos = _next, _next = pos->next)

#define ListForEachPrevSafe(pos, _prev, head) \
        for (pos = (head)->prev, _prev = pos->prev; pos != (head); \
            pos = _prev, _prev = pos->prev)

#define ListForEachOwner(pos, head, member)                     \
        for (pos = ListFirstOwner(head, typeof(*pos), member);  \
            &pos->member != (head);                             \
            pos = ListNextOwner(pos, member))

#define ListForEachOwnerReverse(pos, head, member)              \
        for (pos = ListLastOwner(head, typeof(*pos), member);   \
            &pos->member != (head);                             \
            pos = ListPrevOnwer(pos, member))

#define ListForEachOwnerSafe(pos, next, head, member)           \
        for (pos = ListFirstOwner(head, typeof(*pos), member),  \
            next = ListNextOwner(pos, member);                  \
            &pos->member != (head);                             \
            pos = next, next = ListNextOwner(next, member))

#define ListForEachOwnerReverseSafe(pos, prev, head, member)    \
        for (pos = ListLastOwner(head, typeof(*pos), member),   \
            prev = ListPrevOnwer(pos, member);                  \
            &pos->member != (head);                             \
            pos = prev, prev = ListPrevOnwer(prev, member))

#endif  /* __UTILS_LIST__ */
