#include <stdio.h>
#include <stdlib.h>
typedef struct STRUCT_MAZE{
    int map[10][10];
    int out_X;
    int out_Y;
}MAZE;
typedef struct Array_node{
    int data[2];
    struct Array_node *next;
}link_array;
link_array *create_link();
link_array *insert_node(link_array *head,link_array *array);
void print_link_array(link_array *head);
/*
int main(void)
{
    int i;
    link_array *head,*p;
    head = create_link();
    for (i = 0;i < 10;i++)
    {
        p = (link_array*)malloc(sizeof(link_array));
        p->data[0] = i;
        p->data[1] = i;
        insert_node(head,p);
    }
    print_link_array(head);
    return 0;
}
test link_array
*/
link_array *create_link()
{
    link_array *head;
    head = (link_array*)malloc(sizeof(link_array));
    head->data[0] = -1;
    head->data[1] = -1;
    //init data to show it is head.
    head->next = head;
    return head;
}
link_array *insert_node(link_array *head, link_array *array)
{
    link_array *p = head;
    while (p->next != head)
        p = p->next;
    p->next = array;
    array->next = head;
}
void print_path(link_array *head,MAZE *maze)
{
    link_array *p = head;
    do
    {
        p = p->next;
        printf("(%d,%d)",p->data[0],p->data[1]);
    }
    while (p->next != head && ((p->data[0] != maze->out_X) || (p->data[1] != maze->out_Y)));
}