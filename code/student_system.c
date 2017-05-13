#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Stu_Size sizeof(Student)
enum{Name, Number};
typedef struct student{
    int number;
    char name[12];
    int score[6];
    int total_score;
    double average_score;
    struct student *next,*before;
} Student;
int course_total = 6;
Student * init_head(Student *head);
void insert_node(Student *pre, Student *nex, Student *new);
Student * query_student(const Student* head, int type,void *args);
void switch_node(Student *p);
void print_sort_score(const Student *head, int is_low_to_high);
void print_info(const Student *head);
void print_score_list(const Student *head);
int get_total_score(const Student *node);
void print_sort_number(const Student *head);
void print_sort_name(const Student *head);
void print_choice();
double get_course_score(const Student * head,int course_num, int is_average);
void print_score_statistics(const Student* head);
void sort_by_score(const Student * head, int is_low_to_high);
void print_rank_score(const Student *temp, const Student *head);
int main(void)
{
    Student *head;
    head = (Student *)malloc(Stu_Size);
    head=init_head(head);
    /*
    Student *test_node_1,*test_node_2;
    test_node_1 = (Student*)malloc(Stu_Size);
    test_node_1->number = 10;
    test_node_1->name = "Sha Gou!";
    for (i = 0; i < 6;++i)
        test_node_1->score[i] = 10;
    test_node_1->total_score = get_total_score(test_node_1);
    test_node_1->average_score = test_node_1->total_score*1.0/course_total;

    test_node_2 = (Student*)malloc(Stu_Size);
    test_node_2->number = 9;
    test_node_2->name = "Sha Gou!lv";
    for (i = 0; i < 6;++i)
        test_node_2->score[i] = 11;
    test_node_2->total_score = get_total_score(test_node_2);
    test_node_2->average_score = test_node_2->total_score*1.0/course_total;
    
    printf("total_score:%d\n",test_node_1->total_score);
    //test get_total_score
    insert_node(head,head->next,test_node_1);
    insert_node(head,head->next,test_node_2);
    Student * get_node;
    int number_choice = 10;
    get_node = query_student(head, Number,(void*)&number_choice);
    //test query_student
    puts(get_node->name);
    print_sort_score(head, 1);
    //test print_sort_score
    print_sort_number(head);
    //test print_sort_number
    double average = 0;
    average = get_course_score(head, 0, 0);
    printf("average=%lf\n",average);
    print_sort_name(head);
    //test print_sort_name
    print_score_statistics(head);
    //test print_score_statistics
    print_info(head);
    //test print_info
    */

    int choice;
    print_choice();
    for (;;)
    {
        printf("Please enter your choice:\n");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
            {
                Student *node;
                node = (Student*)malloc(Stu_Size);
                printf("Please input student's number:\n");
                scanf("%d", &(node->number));
                printf("Please input student's name:\n");
                scanf("%s",node->name);
                printf("Please input the number of courses:\n");
                scanf("%d",&course_total);
                printf("Please input each course's score:\n");
                int i;
                for(i = 0; i != course_total; ++i)
                    scanf("%d",&(node->score[i]));
                node->total_score = get_total_score(node);
                node->average_score = node->total_score*1.0/course_total;
                insert_node(head,head->next, node);
                printf("Insert succeeded!\n");
            }
            break;
            case 2:
            {
                int i;
                for(i = 0;i < course_total;++i)
                {
                    printf("course%d:total: %d,average: %.1lf",i,(int)get_course_score(head, i,0),get_course_score(head, i,1));
                    printf("\n");
                }
            }
            break; 
            case 3:
            {
                Student *p = head->next;
                while(p != head)
                {
                    printf("name:%s; total_score: %d; average score: %.1lf",p->name,p->total_score,p->average_score);
                    printf("\n");
                    p = p->next;
                }
            }
            break;
            case 4:
            {
                print_sort_score(head, 0);
            }
            break;
            case 5:
            {
                print_sort_score(head, 1);
            }
            break;
            case 6:
            {
                print_sort_number(head);
            }
            break;
            case 7:
            {
                print_sort_name(head);
            }
            break;
            case 8:
            {
                Student * temp;
                int number_choice;
                printf("Please input number:\n");
                scanf("%d",&number_choice);
                temp = query_student(head,Number, (void*)&number_choice);
                sort_by_score(head, 0);
                print_rank_score(temp, head);
            }
            break;
            case 9:
            {
                Student * temp;
                char *p;
                p = (char*)malloc(sizeof(12));
                printf("Please input name:\n");
                scanf("%s",p);
                temp = query_student(head,Name, (void*)p);
                if (!temp)
                {
                    printf("No such record!\n");
                    break;
                }
                sort_by_score(head, 0);
                print_rank_score(temp, head);
            }
            break;
            case 10:
            {
                print_score_statistics(head);
            }
            break; 
            case 11:
            {
                print_info(head);
            }
            break;
            case 12:
            break;
            case 13:
            break;
            case 0:
                exit(1);
                break;     
        }
    }
    return 0;
}

Student * init_head(Student *head)
{
    head->next = head;
    head->before = head;
    return head;
}

void insert_node(Student *pre, Student *nex, Student *new)
{
    pre->next = new;
	new->before = pre;
	new->next = nex;
	nex->before = new;
}

Student * query_student(const Student *head,int type, void * args)
{
    Student * specific_student = head->next;
    switch (type)
    {
        case Name:
        {
            while(specific_student != head)
            {
                if (strcmp(specific_student->name,(char*)args) == 0)
                    return specific_student;
                specific_student = specific_student->next;
            }
        }
        break;
        case Number:
        {
            while(specific_student != head)
            {
                if (specific_student->number == *((int*)args))
                    return specific_student;
                specific_student = specific_student->next;
            }
        }
        break;
    }
    return (Student*)0;
}

void print_info(const Student *head)
{
    Student *p = head->next;
    int course_sq;
    while(p != head)
    {
        printf("Number:%d; Name:%s; ",p->number, p->name);
        printf("every course: ");
        for (course_sq = 0; course_sq < course_total; ++course_sq)
            printf("%d ",p->score[course_sq]);
        printf(";total score: %d; average score: %.1lf;",p->total_score, p->average_score);
        printf("\n");
        p = p->next;
    }
    puts("\n");
}
void sort_by_score(const Student * head, int is_low_to_high)
{
    Student *p_1 = head->next,*p_2 = head->next;
    while(p_1 != head)
    {
        while(p_2 != head)
        {
            if (p_2->total_score > p_2->next->total_score)
                if (is_low_to_high)
                    switch_node(p_2->next);
                else;
            else if (!is_low_to_high)
                switch_node(p_2->next);
            p_2 = p_2->next;
        }
        p_1 = p_1->next;
        p_2 = p_1;
    }
}
void print_sort_score(const Student * head, int is_low_to_high)
{
    sort_by_score(head,is_low_to_high);
    Student *p_1;
    p_1 = head->next;
    int num = 1;
    while(p_1 != head)
    {
        printf("%d: %s\n",num, p_1->name);
        num++;
        p_1 = p_1->next;
    }
}

void switch_node(Student *p)
{
    Student* q=p->before; 
    q->before->next=p;
    p->before=q->before;
    q->next=p->next;
    q->before=p;
    p->next->before=q;
    p->next=q;
}

void print_choice()
{
    printf("1. Input record\n2. Caculate total and adverage score of every course\n");
    printf("3. Caculate total and adverage score of every student\n");
    printf("4. Sort in descending order by total score of every student\n");
    printf("5. Sort in ascending order by total score of every student\n");
    printf("6. Sort in ascending order by number\n7. Sort in dictionary order by name");
    printf("8. Search by number\n9. Search by name\n10. Statistic analysis for every course\n");
    printf("11.List record\n12.Write to a file\n13.Read from a file\n0. Exit\n");
}

int get_total_score(const Student *node)
{
    int i,sum = 0;
    for (i = 0; i < course_total;++i)
        sum += node->score[i];
    return sum;
}
void print_score_list(const Student *head)
{
    Student *p = head->next;
    int loop;
    while(p != head)
    {
        printf("%s:",p->name);
        for(loop = 0; loop < course_total; loop++)
            printf("%d ",p->score[loop]);
        printf("\n");
        p = p->next;
    }
}

void print_sort_number(const Student *head)
{
    Student *p_1 = head->next,*p_2 = head->next;
    while(p_1 != head)
    {
        while(p_2 != head)
        {
            if (p_2->number > p_2->next->number)
                switch_node(p_2->next);
            p_2 = p_2->next;
        }
        p_1 = p_1->next;
        p_2 = p_1;
    }
    print_score_list(head);
}
double get_course_score(const Student * head,int course_num,int is_average)
{
    Student *p = head->next;
    int sum = 0,count = 0;
    while(p != head)
    {
        sum += p->score[course_num];
        p = p->next;
        count++;
    }
    if (is_average)
        return sum/(count*1.0);
    else
        return sum;
}

void print_sort_name(const Student *head)
{
    Student *p_1 = head->next,*p_2 = head->next;
    while(p_1 != head)
    {
        while(p_2 != head)
        {
            if (stricmp(p_2->name, p_2->next->name) > 0)
                switch_node(p_2->next);
            p_2 = p_2->next;
        }
        p_1 = p_1->next;
        p_2 = p_1;
    }
    print_score_list(head);
}

void print_score_statistics(const Student* head)
{
    int section[5]={0};
    int course_sq,count = 0,i;
    Student *p = head->next;
    for (course_sq = 0; course_sq < course_total; ++course_sq)
    {
        while(p != head)
        {
            if(p->score[course_sq] <= 100 && p->score[course_sq] >= 90)
                section[0]++;
            else if(p->score[course_sq] >= 80 && p->score[course_sq] <= 89)
                section[1]++;
            else if(p->score[course_sq] >= 70 && p->score[course_sq] <= 79)
                section[2]++;
            else if(p->score[course_sq] >= 60 && p->score[course_sq] <= 69)
                section[3]++;
            else if(p->score[course_sq] >= 0 && p->score[course_sq] <= 59)
                section[4]++;
            p = p->next;
            count++;
        }
        printf("course%d: 100~90: %d,  %.1lf%%; 80~89: %d,  %.1lf%%; 70~79: %d,  %.1lf%%; 60~69: %d,  %.1lf%%; 0~59: %d,  %.1lf%%",\
            course_sq+1, section[0], section[0]*1.0/count*100, section[1],section[1]*1.0/count*100,\
            section[2],section[2]*1.0/count*100, section[3],section[3]*1.0/count*100, \
            section[4], section[4]*1.0/count*100);
        printf("\n");
        count = 0;
        for(i = 0; i < 5; i++)
            section[i] = 0;
        p = head->next;
    }
}
void print_rank_score(const Student *temp, const Student *head)
{
    int rank = 1;
    Student *p = head->next;
    while(p != temp)
    {
        rank++;
        p = p->next;
    }
    printf("Rank:%d ",rank);
    printf("every course: ");
    int course_sq;
    for (course_sq = 0; course_sq < course_total; ++course_sq)
        printf("%d ",p->score[course_sq]);
    printf("\n");
}