#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
typedef struct Node{
    int num;
    struct Node * before;
    struct Node * next;
}list;
list * init_list();//initilize one list
void insert_node(list * head, list * node);//insert one node 
void print_list(list* head);//print list
list * merge_list(list *head_1,list *head_2);//merge two list
void input_list(list* head);//get input
int main(void)
{
    list * head_1, *head_2;
    head_1 = init_list();
    head_2 = init_list();
    input_list(head_1);
    input_list(head_2);
    /*
    list *test_node;
    int loop;
    for (loop = 5;loop <= 9; loop++)
    {
        test_node = (list*)malloc(sizeof(list));
        test_node -> num = loop;
        insert_node(head_1, test_node);
    }
    for (loop = 6;loop <= 10; loop++)
    {
        test_node = (list*)malloc(sizeof(list));
        test_node -> num = loop;
        insert_node(head_2, test_node);
    }   
    */
    //test code
    list *head;
    head = merge_list(head_1, head_2);
    print_list(head);
    return 0;
}
list* init_list()
{
    list *head;
    head = (list *)malloc(sizeof(list));
    head->num = -1;
    //init head num with -1;
    head->before = head;
    head->next = NULL;
    return head;
}
void insert_node(list *head, list *node)
{
    list *p = head;
    while(p->next != NULL)
        p = p->next;
    p->next = node;
    node->next = NULL;
    node->before = p;
}
void print_list(list * head)
//see if list work normally
{
    list * p=head->next;
    while(p != NULL)
    {
        printf("%d ", p->num);
        p = p->next;
    }
}
list * merge_list(list * head_1, list *head_2)
{
    list *p = head_1;
    list* loop_p = head_1->next;
    int free_node = 0;
    while (p -> next != NULL)
        p = p->next;
    head_2->next->before = p;
    p->next = head_2->next;
    p = head_1->next;
    //print_list(head_1);
    while(p->next != NULL)
    {
        while(loop_p != p)
        {
            //printf("loop_p:%d\n", loop_p->num);
            if (loop_p -> num == p -> num)
               {
                    free_node = 1;
                    break;
                }
            loop_p = loop_p -> next;
        }
        loop_p = head_1->next;
        if (free_node == 1)
        {
            //delete current node
            p->before->next = p->next;
            p->next->before = p->before;
            list *q = p->next;
            free(p);
            p = q;
            free_node = 0;
        }
        else 
            p = p->next;
        //printf("%d  ", p->num);
    }
    return head_1;
}
void input_list(list *head)
{
    int num;
    list *node;
    while(1)
    {
        scanf("%d", &num);
        if (num == -1)
            break;
        node = (list*)malloc(sizeof(list));
        node->num = num;
        insert_node(head, node);
    }
}