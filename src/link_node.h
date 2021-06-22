/*
This is the implementation for a link node that can be used with a variety
of abstract data types. The main idea is to separate the functions that perform
memmory based operation and make it easier for user to code for Stacks, Queues
and Lists.

Author: Rajiv Singh
Date: 15-09-2020
*/

#ifndef LINK_NODE_H
#define LINK_NODE_H

// This node is the one that handles linking.
struct link_node{
    struct link_node *prev, *next;
};

// A way to generate address of node that contains a specific list node
#define ADDRESS(type, name, member) (struct type *)((void *)name - \
        (long unsigned int)&((struct type* )0)->member)

// To check if the link contains node or not
static inline int is_empty(struct link_node *head){
	return (head->prev == head) && (head->next == head);
}

// Separate initializers for head and normal node

#define INIT_HEAD(node) {&node, &node}

static void init_node(struct link_node *node){
    node->prev = node;
	node->next = node;
}

// A set of functions that are useful for Stack, Linked list, Queues.
static inline void add_at_tail(struct link_node *head, \
	   struct link_node *node){
	node->prev = head->prev;
	node->next = head;
	head->prev->next = node;
	head->prev = node;
}

static inline void add_at_head(struct link_node *head, \
	   struct link_node *node){
	add_at_tail(head, node);
	head = node;
}

static inline void remove_at_tail(struct link_node *head){
	head->prev = head->prev->prev;
	head->prev->next = head;
}

static inline void remove_at_head(struct link_node *head){
	head->next->prev = head->prev;
	head->prev->next = head->next;
	head = head->next;
}

#define JUMP(i, head, temp, pos) \
		for(i = 0, temp = head; i < pos && temp->next != head; \
		    i++, temp = temp->next)

static inline void add_in_between(struct link_node *current, \
	   struct link_node *node){
	node->next = current;
	node->prev = current->prev;
	current->prev->next = node;
	current->prev = node;
}

static inline void remove_in_between(struct link_node *current){
    current->prev->next = current->next;
	current->next->prev = current->prev;
}

#endif
