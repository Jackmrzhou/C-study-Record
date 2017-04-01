#ifndef _UNSIZE_ARRAY_H
#define _UNSIZE_ARRAY_H

typedef struct Array_node{
    int data[2];
    struct Array_node *next;
}link_array;
link_array *create_link();
link_array *insert_node(link_array *head,link_array *array);
void print_link_array(link_array *head);

#endif