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
	//struct list_head list_node;
};
struct an_test_node{
	char test_ch;
	//struct list_head list_node;
};
void init_head(struct list_head *head);
void add_node(struct list_head *head, struct list_head *new);
int main()
{
	/*printf("%lu\n",sizeof(int));
	printf("%lu\n",sizeof(void *));
	printf("%lu\n",sizeof(struct node));
	printf("%lu\n",sizeof(struct list_head));*/
	struct test_node test_node_1;
	struct an_test_node test_node_2;
	test_node_1.test_int = 10;
	test_node_1.test_array[0]=1;
	test_node_1.test_array[1]=2;
	test_node_2.test_ch = 'a';
	struct node my_node_1,my_node_2;
	my_node_1.data = &test_node_1;
	my_node_1.type = struct_test_node;
	my_node_2.data = &test_node_2;
	my_node_2.type = struct_an_test_node;
	struct list_head *head;
	head = (struct list_head*)malloc(sizeof(struct list_head));
	init_head(head);
	add_node(head, &(my_node_1.list_node));
	add_node(head, &(my_node_2.list_node));
	struct node *temp_node;
	temp_node = container_of(head->next, struct node, list_node);
	printf("%c\n",((struct an_test_node*)(temp_node->data))->test_ch);
	temp_node = container_of(head->next->next, struct node, list_node);
	printf("%d\n", ((struct test_node*)(temp_node->data))->test_int);
	return 0;
}
void init_head(struct list_head *head)
{
	head->next = head;
	head->before = head;
}
void add_node(struct list_head *head,struct list_head* new)
{
	new->next = head->next;
	head->next->before = new;
	head->next = new;
	new->before = head;
}
