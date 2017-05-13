#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct stud_node{
	int num;
	char name[20];
	int english,computer,math;
	struct stud_node*next; 
}; 
struct stud_node*InsertDoc(struct stud_node*head,struct stud_node*stud);
struct stud_node*DeleteDoc(struct stud_node*head,int num);
void Print_Stu_Doc(struct stud_node*head);
struct stud_node*Question(struct stud_node*head); 
struct stud_node*Change(struct stud_node*head); 
struct stud_node*Average(struct stud_node*head);
struct stud_node*Rank(struct stud_node*head);
struct stud_node* INSERT_NODE(struct stud_node*head,struct stud_node*node);

int main(void){
	struct stud_node *p,*ptr;
	struct stud_node *head;
	int choice,num,score,english,math,computer;
	head=NULL;
	char name[20];
	int size=sizeof(struct stud_node);
	FILE *fp;	
	if((fp=fopen("e:\\cj.txt","r+"))==NULL){
		printf("File open error!\n");
		exit(0);
	}
	while(!feof(fp))
	{
		ptr = (struct stud_node*)malloc(size);		
		fscanf(fp,"%d%s%d%d%d",&ptr->num,ptr->name,&ptr->english,&ptr->math,&ptr->computer);	
		head = INSERT_NODE(head,ptr);	
	}
		
	do{
		printf("1:Exit 2:Insert 3:Delete 4:Print\n");
		printf("5:Question 6:Change 7.Average 8.Rank\n");
		scanf("%d",&choice);
		switch(choice){
			case 1:
				break;
			case 2:
				printf("Input num,name and score: \n");
				scanf("%d%s%d%d%d",&num,name,&english,&math,&computer);
				p=(struct stud_node*) malloc(size);
				p->num=num;
				strcpy(p->name,name);
				p->english=english;
				p->math=math;
				p->computer=computer;				
				head=INSERT_NODE(head,p);
				fprintf(fp,"\n%d %s %d %d %d",p->num,p->name,p->english,p->math,p->computer);
				break;
			case 3:
				printf("Input num:\n");
				scanf("%d",&num);
				head=DeleteDoc(head,num);
				break;
			case 4:
				Print_Stu_Doc(head);
				break;
			case 5:
				Question(head);
				break;
			case 6:
				Change(head);
				break;
			case 7:
				Average(head);
				break;
			case 8:
				Rank(head);
				break;
			
		 
		}	
	}while(choice!=1);
		
		if(fclose(fp)){
		printf("Can not close the file!\n");
		exit(0);
	}
}

struct stud_node*InsertDoc(struct stud_node*head,struct stud_node*stud){
	struct stud_node*ptr,*ptr1,*ptr2;
	ptr2=head;
	ptr=stud;
	if(head==NULL){
		head=ptr;
		head->next=NULL;
	}
	else{
		while((ptr->num>ptr2->num)&&(ptr2->next!=NULL)){
			ptr1=ptr2;
			ptr2=ptr2->next;
		}
		if(ptr->num<=ptr2->num){
			if(head==ptr2)head=ptr;
			else ptr1->next=ptr;
			ptr->next=ptr2;
		}
		else{
			ptr2->next=ptr;
			ptr->next=NULL;
		}
	}
	
	return head;
}

struct stud_node*DeleteDoc(struct stud_node*head,int num){
	struct stud_node *ptr1,*ptr2;
	while(head!=NULL&&head->num==num){
		ptr2=head;
		head=head->next;
		free(ptr2);
		}
		
		if(head==NULL)
		return NULL;
		
		ptr1=head;
		ptr2=head->next;
		while(ptr2!=NULL){
			if(ptr2->num==num){
				ptr1->next=ptr2->next;
				free(ptr2);
			}
			else
				ptr1=ptr2;
				ptr2=ptr1->next;
		}
		return head;
}
void Print_Stu_Doc(struct stud_node*head)
{

	struct stud_node*ptr=head;
	if(head==NULL){
		printf("\n No Records\n");
		return;
	}
	printf("\nThe Students' Records Are: \n");
	printf("序号\t姓名\t英语\t数学\tC程\n");
	
	while(ptr!=NULL)
	{
		printf("%d\t%s\t%d\t%d\t%d\t\n",ptr->num,ptr->name,ptr->english,ptr->math,ptr->computer);
		ptr=ptr->next;
	}
}

struct stud_node*Question(struct stud_node*head){
	struct stud_node*ptr;
	int choice,NUMBER;
	char NAME[20];
	if(head==NULL){
		printf("\nNo Records\n");
		
	}
	printf("1:按序号查询 2：按姓名查询\n");
	scanf("%d",&choice);
	ptr=head;
	switch(choice){
	 	case 1:
	 		printf("Your Number:");
	 		scanf("%d",&NUMBER);
			printf("序号\t姓名\t英语\t数学\tC程\n");
	 	    do{
				if(ptr->num==NUMBER) {
				printf("%d\t%s\t%d\t%d\t%d\t\n",ptr->num,ptr->name,ptr->english,ptr->math,ptr->computer);
	 	    		break;
	 	    	}
				ptr=ptr->next;
				
			 }while(ptr->next!=NULL);
			if(ptr->next==NULL) printf("No records\n");
			
	 		break;
	 	case 2:
	 	    printf("Your Name:");
	 		scanf("%s",NAME);
	 		printf("序号\t姓名\t英语\t数学\tC程\n");
	 	    do
	 	    {
	 	    	if(strcmp(ptr->name,NAME)==0) 
	 	    		{
					 printf("%d\t%s\t%d\t%d\t%d\t\n",ptr->num,ptr->name,ptr->english,ptr->math,ptr->computer);
	 	    			break;
					 }
              ptr=ptr->next;
			}while(ptr->next!=NULL);
			if(ptr->next==NULL) printf("No records\n");
	 		break;
	 }
	 printf("\n");
}

struct stud_node*Change(struct stud_node*head){
	struct stud_node*ptr;
	int newscore,choice,NUMBER;
	if(head==NULL){
		printf("\nNo Records\n");	
	}
		printf("请输入你的序号：\n"); 
		scanf("%d",&NUMBER);
	 	    for(ptr=head;ptr->next!=NULL;ptr=ptr->next)
	 	    {//printf("1\n");
	 	    	if(ptr->num==NUMBER) 	 	    		
	 	    	break;	 
			 }
			if(ptr==NULL) printf("No records.\n"); 
		printf("1:改英语 2：改数学 3.改C程\n");
		scanf("%d",&choice);
		printf("请输入新成绩:\n");
		scanf("%d",&newscore);
	switch(choice){
	 	case 1:
			ptr->english=newscore;
	 		break;
	 	case 2:
	 	    ptr->math=newscore;
	 		break;
	 	case 3:
	 	    ptr->computer=newscore;
	 		break;
	}
	printf("\n");
}

struct stud_node*Average(struct stud_node*head){
	int choice,average;
	int total=0;
	int j=0;
	struct stud_node*ptr;
	printf("1.AverageEnglish 2.AverageMath 3.AverageComputer\n");
	scanf("%d",&choice);
	switch(choice){
		case 1:
			for(ptr=head;ptr->next!=NULL;ptr=ptr->next){
			total+=ptr->english;
			j++;	
			}
			average=total*1.0/j;
			printf("\n英语平均成绩：%d\n",average);
			break;
		case 2:
			for(ptr=head;ptr->next!=NULL;ptr=ptr->next){
			total=ptr->math+total;
			j++;	
			}
			average=total*1.0/j;
			printf("\n数学平均成绩：%d\n",average);
			break;
		case 3:
			for(ptr=head;ptr->next!=NULL;ptr=ptr->next){
			total=ptr->computer+total;
			j++;	
			}
			average=total*1.0/j;
			printf("\nC程平均成绩%d\n",average);
			
			break;
			
			}				
		printf("\n");	
	}
	



struct stud_node*Rank(struct stud_node*head){
	struct stud_node*ptr,*ptr1,temp,*TEMP;
	int choice;
	
	printf("1.RankEnglish 2.RankMath 3.RankComputer\n") ;
	scanf("%d",&choice);
	switch(choice){
		case 1:
			for(ptr=head;ptr!=NULL;ptr=ptr->next){
	
				for(ptr1=ptr->next;ptr1!=NULL;ptr1=ptr1->next){
					if(ptr1->english>ptr->english){
						temp=*ptr;
						*ptr=*ptr1;
						*ptr1=temp;
						TEMP = ptr->next;
						ptr->next = ptr1->next;
						ptr1->next = TEMP;
					 } 
				}
			}
			printf("英语排名：\n");
			printf("序号\t姓名\t成绩\n");
			for(ptr=head;ptr->next!=NULL;ptr=ptr->next){
				printf("%d\t%s\t%d\n",ptr->num,ptr->name,ptr->english);
			}
			break;
		case 2:
			for(ptr=head;ptr!=NULL;ptr=ptr->next){
	
				for(ptr1=ptr->next;ptr1!=NULL;ptr1=ptr1->next){
					if(ptr1->english>ptr->english){
						temp=*ptr;
						*ptr=*ptr1;
						*ptr1=temp;
						TEMP = ptr->next;
						ptr->next = ptr1->next;
						ptr1->next = TEMP;
					 } 
				}
			}
			printf("数学排名：\n");
			printf("序号\t姓名\t成绩\n");
			for(ptr=head;ptr->next!=NULL;ptr=ptr->next){
				printf("%d\t%s\t%d\n",ptr->num,ptr->name,ptr->math);
			}
			break;
		case 3:
			for(ptr=head;ptr!=NULL;ptr=ptr->next){
	
				for(ptr1=ptr->next;ptr1!=NULL;ptr1=ptr1->next){
					if(ptr1->english>ptr->english){
						temp=*ptr;
						*ptr=*ptr1;
						*ptr1=temp;
						TEMP =ptr->next;
						ptr->next = ptr1->next;
						ptr1->next = TEMP;
					 } 
				}
			}
			printf("C程排名：\n");
			printf("序号\t姓名\t成绩\n");
			for(ptr=head;ptr->next!=NULL;ptr=ptr->next){
				printf("%d\t%s\t%d\n",ptr->num,ptr->name,ptr->computer);
			}
			break;
						
	}
}
struct stud_node* INSERT_NODE(struct stud_node*head,struct stud_node*node)
{
	struct stud_node *p = head;
	if (head == NULL)
	{
		head = node;
		head->next = NULL;
	}
	else	
	{
		while (p->next != NULL)
			p = p->next;
		p->next = node;
		node->next = NULL;
	}
	return head;
}


