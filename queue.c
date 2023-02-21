#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *q_head = malloc(sizeof(*q_head));
    if (!q_head)
        return NULL;
    INIT_LIST_HEAD(q_head);
    return q_head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    element_t *cur_node;
    element_t *safe;
    list_for_each_entry_safe (cur_node, safe, l, list) {
        list_del(&cur_node->list);   /* remove link of cur_node */
        q_release_element(cur_node); /* release cur_node */
    }
    free(l);
}

#ifndef strlcpy
#define strlcpy(dst, src, size) snprintf((dst), (size), "%s", (src))
#endif

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *new_element = malloc(sizeof(element_t));
    new_element->value = malloc(sizeof(s));
    strlcpy(new_element->value, s, sizeof(&new_element->value));
    list_add(&new_element->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *new_element = malloc(sizeof(element_t));
    new_element->value = malloc(sizeof(s));
    strlcpy(new_element->value, s, sizeof(&new_element->value));
    list_add_tail(&new_element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *rm_element = list_first_entry(head, element_t, list);
    list_del(&rm_element->list);
    if (sp) {
        strlcpy(sp, rm_element->value, bufsize);
    }
    return rm_element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *rm_element = list_last_entry(head, element_t, list);
    list_del(&rm_element->list);
    if (sp) {
        strlcpy(sp, rm_element->value, bufsize);
    }
    return rm_element;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    int count = 0;
    struct list_head *cur_node;
    list_for_each (cur_node, head)
        count = count + 1;
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    int mid = q_size(head) / 2 + q_size(head) % 2;
    int count = 0;
    struct list_head *cur_node;
    struct list_head *safe;
    list_for_each_safe (cur_node, safe, head) {
        count = count + 1;
        if (count == mid) {
            list_del(cur_node);
            q_release_element(list_entry(cur_node, element_t, list));
            return true;
        }
    }
    return false;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
