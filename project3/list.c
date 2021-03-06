#include <stdio.h>
#include <stdlib.h>
#define container_of(ptr, type, member)({\
	const typeof(((type*)0)->member)*__ptr = ptr;\
	(type*)((char*)__ptr-(size_t)(&((type*)0)->member));})
enum {struct_test_node, struct_an_test_node};
struct list_head{
	struct list_head *before,*next;
};
struct node{
	void * data;
	int type;
	struct list_head list_node;
};
struct test_node{
	int test_int;
	int test_array[2];
};
struct an_test_node{
	char test_ch;
};
inline void init_head(struct list_head *head);
inline void head_insert(struct list_head *head, struct list_head * new);
inline void tail_insert(struct list_head *tail, struct list_head * new);
inline void list_insert(struct list_head *pre,struct list_head *nex, struct list_head *new);
inline void node_delete(struct list_head *pre, struct list_head *nex);
inline int is_list_empty(const struct list_head * head);
inline void pop_head(struct list_head *head);
inline void pop_tail(struct list_head *head);
void pop_certain_node(struct list_head *head, int i);
void merge_list(struct list_head *head_1, struct list_head * head_2);
int get_list_lenth(const struct list_head *head);
struct node* get_item(const struct list_head *head,int item_type,int count);
int main()
{
	/*printf("%lu\n",sizeof(int));
	printf("%lu\n",sizeof(void *));
	printf("%lu\n",sizeof(struct node));
	printf("%lu\n",sizeof(struct list_head));*/
	struct test_node *test_node_1;
	struct an_test_node *test_node_2,*test_node_3;
	test_node_1 = (struct test_node *)malloc(sizeof(struct test_node));
	test_node_2 = (struct an_test_node*)malloc(sizeof(struct an_test_node));
	test_node_3 = (struct an_test_node*)malloc(sizeof(struct an_test_node));
	(*test_node_1).test_int = 10;
	(*test_node_1).test_array[0]=1;
	(*test_node_1).test_array[1]=2;
	(*test_node_2).test_ch = 'a';
	(*test_node_3).test_ch = 'b';
	struct node *my_node_1,*my_node_2,*my_node_3;
	my_node_1 = (struct node*)malloc(sizeof(struct node));
	my_node_2 = (struct node*)malloc(sizeof(struct node));
	my_node_3 = (struct node*)malloc(sizeof(struct node));
	(*my_node_1).data = test_node_1;
	(*my_node_1).type = struct_test_node;
	(*my_node_2).data = test_node_2;
	(*my_node_2).type = struct_an_test_node;
	(*my_node_3).data = test_node_3;
	(*my_node_3).type = struct_an_test_node;
	struct list_head *head;
	head = (struct list_head*)malloc(sizeof(struct list_head));
	init_head(head);
	head_insert(head, &(my_node_1->list_node));
	head_insert(head, &(my_node_2->list_node));
	//test insert
	struct node *temp_node;
	temp_node = container_of(head->next, struct node, list_node);
	printf("%c\n",((struct an_test_node*)(temp_node->data))->test_ch);

	temp_node = container_of(head->next->next, struct node, list_node);
	printf("%d\n", ((struct test_node*)(temp_node->data))->test_int);

	node_delete(head,head->next->next);
	//test delete
	temp_node = container_of(head->next, struct node, list_node);
	printf("%d\n", ((struct test_node*)(temp_node->data))->test_int);
	temp_node = get_item(head,struct_test_node,1);
	//test get_item
	printf("%d\n", ((struct test_node*)(temp_node->data))->test_int);
	struct list_head * head_2;
	head_2 = (struct list_head *)malloc(sizeof(struct list_head));
	init_head(head_2);
	head_insert(head_2, &(my_node_3->list_node));
	merge_list(head, head_2);
	//test merge_list
	int i;
	i = get_list_lenth(head);
	//test get_list_lenth
	printf("#%d#\n",i);
	return 0;
}
inline void init_head(struct list_head *head)
{
	head->next = head;
	head->before = head;
}
inline void head_insert(struct list_head *head,struct list_head* new)
{
	list_insert(head, head->next, new);
}
inline void tail_insert(struct list_head *tail, struct list_head * new)
{
	list_insert(tail->before, tail, new);
}
inline void list_insert(struct list_head *pre,struct list_head *nex, struct list_head *new)
{
	pre->next = new;
	new->before = pre;
	new->next = nex;
	nex->before = new;
}
inline void node_delete(struct list_head * pre,struct list_head *nex)
{
	pre->next = nex;
	free(container_of(nex->before,struct node, list_node)->data);
	free(container_of(nex->before,struct node, list_node));
	nex->before = pre;
}
inline int is_list_empty(const struct list_head *head)
{
	return head->next == head;
}
int get_list_lenth(const struct list_head *head)
{
	int count = 0;
	struct list_head *p = head->next;
	while(p != head)
	{
		++count;
		p = p->next;
	}
	return count;
}
struct node* get_item(const struct list_head *head,const int item_type, int count)
{
	struct list_head *p = head->next;
	while(p != head)
	{
		if ((container_of(p, struct node, list_node))->type == item_type)
			--count;
		if (count == 0)
			return container_of(p, struct node, list_node);
		p = p->next;
	}
	return (struct node*)(0);
	//not found
}
void merge_list(struct list_head *head_1, struct list_head * head_2)
{
	struct list_head *tail_1 = head_1->before, * tail_2 = head_2->before;
	tail_1->next = head_2->next;
	head_2->next->before = tail_1;
	tail_2->next = head_1;
	head_1->before = tail_2;
}
inline void pop_head(struct list_head *head)
{
	node_delete(head, head->next->next);
}
inline void pop_tail(struct list_head *head)
{
	node_delete(head->before->before, head);
}
void pop_certain_node(struct list_head *head, int i)
{
	struct list_head *p =head->next;
	while(--i > 0)
		p = p->next;
	node_delete(p->before, p->next);
}