#include <stdio.h>
#include <stdlib.h>
typedef struct Array_node{
    int data[2];
    struct Array_node *next;
}link_array;
link_array *create_link();
link_array *insert_node(link_array *head,link_array *array);
void print_link_array(link_array *head);
void clear_link(link_array *head);
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

link_array *create_link()
{
    link_array *head;
    head = NULL;
    head->next = head;
    return head;
}
link_array *insert_node(link_array *head, link_array *array)
{
    if (head == NULL)
    {
        head = array;
        head->next = head;
    }
    else
    {
        link_array *p = head;
        while(p->next != head)
            p = p-> next;
        p->next = array;
        array->next = head;
    }
}
void print_link_array(link_array *head)
{
    link_array *p = head;
    do
    {
        printf("(%d,%d)",p->data[0],p->data[1]);
        p = p->next;
    }
    while (p->next != head);
}
void clear_link(link_array *head)
{
    link_array *p;
    if (head == NULL)
        return;
    while (head != NULL)
    {
        p = head->next;
        free(head);
        head = p;
    }
    head->next = NULL;
}