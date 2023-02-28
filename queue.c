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
    if (!new_element) {
        return false;
    }
    int length = strlen(s) + 1;
    new_element->value = malloc(sizeof(char) * length);
    if (!new_element->value) {
        free(new_element);
        return false;
    }
    strlcpy(new_element->value, s, length);
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
    if (!new_element) {
        return false;
    }
    int length = strlen(s) + 1;
    new_element->value = malloc(sizeof(char) * length);
    if (!new_element->value) {
        free(new_element);
        return false;
    }
    strlcpy(new_element->value, s, length);
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
    int mid = (q_size(head) - 1) / 2; /*middle of 0-based indexing*/
    int count = -1;                   /*count 0 at first node*/
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
    if (!head || list_empty(head))
        return false;
    bool is_dup = false; /*check if cur_node is duplicate*/
    element_t *cur_node;
    element_t *safe;
    list_for_each_entry_safe (cur_node, safe, head, list) {
        if (&safe->list != head && strcmp(cur_node->value, safe->value) == 0) {
            is_dup = true;
            list_del(&cur_node->list);
            q_release_element(cur_node);
        } else if (is_dup) { /*cur_node is duplicate, delete it*/
            is_dup = false;
            list_del(&cur_node->list);
            q_release_element(cur_node);
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;
    struct list_head *cur_node;
    struct list_head *safe;
    for (cur_node = (head)->next, safe = cur_node->next;
         cur_node != (head) && safe != (head);
         cur_node = cur_node->next, safe = cur_node->next) {
        list_move(cur_node, safe);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;
    struct list_head *cur_node;
    struct list_head *safe;
    list_for_each_safe (cur_node, safe, head) {
        list_move(cur_node, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head))
        return;
    LIST_HEAD(temp_head);
    struct list_head *cur_node;
    struct list_head *safe;
    struct list_head *from = head;
    int count = 0;
    list_for_each_safe (cur_node, safe, head) {
        count = count + 1;
        if (count == k) {
            list_cut_position(&temp_head, from, cur_node);
            q_reverse(&temp_head);
            list_splice_init(&temp_head, from);
            count = 0;
            from = safe->prev;
        }
    }
}

/* Sort elements of queue in ascending order */
struct list_head *merge_two_list(struct list_head *l1, struct list_head *l2)
{
    struct list_head *temp = NULL;
    struct list_head **indirect = &temp;
    for (struct list_head **node = NULL; l1 && l2; *node = (*node)->next) {
        element_t *e1 = list_entry(l1, element_t, list);
        element_t *e2 = list_entry(l2, element_t, list);
        if (strcmp(e1->value, e2->value) < 0)
            node = &l1;
        else
            node = &l2;
        *indirect = *node;
        indirect = &(*indirect)->next;
    }
    *indirect = (struct list_head *) ((u_int64_t) l1 | (u_int64_t) l2);
    return temp;
}
struct list_head *merge_sort(struct list_head *head)
{
    if (!head || !head->next) {
        return head;
    }
    struct list_head *slow = head;
    for (struct list_head *fast = head->next; fast && fast->next;
         fast = fast->next->next) {
        slow = slow->next;
    }
    struct list_head *mid;
    mid = slow->next;
    slow->next = NULL;
    struct list_head *left = merge_sort(head);
    struct list_head *right = merge_sort(mid);
    return merge_two_list(left, right);
}
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    head->prev->next = NULL;
    head->next = merge_sort(head->next);
    struct list_head *current = head, *next = head->next;
    while (next) {
        next->prev = current;
        current = next;
        next = next->next;
    }
    current->next = head;
    head->prev = current;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head) {
        return 0;
    }

    element_t *cur_element;
    element_t *prev_element;
    for (cur_element = list_entry((head)->prev, element_t, list),
        prev_element = list_entry(cur_element->list.prev, element_t, list);
         &cur_element->list != (head); cur_element = prev_element,
        prev_element = list_entry(prev_element->list.prev, element_t, list)) {
        if (&prev_element->list != head &&
            strcmp(cur_element->value, prev_element->value) > 0) {
            element_t *temp = prev_element;
            prev_element = cur_element;
            list_del(&temp->list);
            q_release_element(temp);
        }
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head)) {
        return 0;
    }
    if (list_is_singular(head)) {
        return list_entry(head->next, queue_contex_t, chain)->size;
    }
    queue_contex_t *merged_list = list_entry(head->next, queue_contex_t, chain);
    struct list_head *node = NULL, *safe = NULL;
    list_for_each_safe (node, safe, head) {
        if (node == head->next) {
            continue;
        }
        queue_contex_t *temp = list_entry(node, queue_contex_t, chain);
        list_splice_init(temp->q, merged_list->q);
    }
    q_sort(merged_list->q);
    return merged_list->size;
}
