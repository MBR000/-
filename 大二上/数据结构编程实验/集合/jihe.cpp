#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct node
{
	char count;
	struct node *next;
};

char map[]="abcdefghijklmnopqrstuvwxyz";

typedef struct node LIST;
void release(struct node **headptrptr);
struct node* add(int n);
void printfdata(struct node **headptrptr);
void bubble(struct node **headptrptr); //ð����������
void duplicateremove(struct node **headptrptr);//ȥ��
void merge(struct node **headptrptr1,struct node **headptrptr2);
void intersection(struct node **headptrptr1,struct node **headptrptr2);
void deletenode(struct node **headptrptr2,int num);
void insert(struct node **headptrptr1,int num);
void difference(struct node **headptrptr1,struct node **headptrptr2);
void Complement(struct node **headptrptr1);

int main(void)
{
	int n,m;
	printf("�����һ�����ϸ���:");
	scanf("%d",&n);
	LIST *set1=add(n);
	printf("����ڶ������ϸ���:");
	scanf("%d",&m);
	LIST *set2=add(m);
	bubble(&set1);
	bubble(&set2);

	duplicateremove(&set1);
	duplicateremove(&set2);

	merge(&set1,&set2);
	intersection(&set1,&set2);
	difference(&set1,&set2);
	printf("set1���ϵĲ�����");
	Complement(&set1);
	printf("set2���ϵĲ�����");
	Complement(&set2);
	release(&set1);
	release(&set2);
	return 0;
}

struct node* add(int n)
{
	struct node *last=NULL;
	struct node *current=NULL;
	struct node *head=NULL;
	char a[150];
	char *sptr;
	sptr=a;
	printf("�����ַ���:\n");
	getchar();
	gets(a);
	while(n!=0)
	{
		current=(struct node*)malloc(sizeof(struct node));
		if(current!=NULL)
		{
			current->count=*sptr;
			if(head==NULL)
			{
				head=current;
				last=current;
			}
			else
			{
				last->next=current;
				last=current;//����
			}
		}
		n=n-1;
		sptr++;
	}
	if(last!=NULL)
	{
		last->next=NULL;
	}
	return head;
}

void release(struct node **headptrptr)
{
	struct node *temp;
	while(*headptrptr!=NULL)
	{
		temp=*headptrptr;
		*headptrptr=(*headptrptr)->next;
		free(temp);
	}
}

void intersection(struct node **headptrptr1,struct node **headptrptr2)
{
	struct node *temp1,*temp2;
	temp1=*headptrptr1;
	temp2=*headptrptr2;
	char a[100];
	int count=0;
	printf("���ϵĽ���");
	while(temp1!=NULL&&temp2!=NULL)
	{
		if(temp2->count<temp1->count)
		{
			temp2=temp2->next;
		}
		else if(temp2->count==temp1->count)
		{
			a[count]=temp1->count;
			count++;
			temp2=temp2->next;
		}
		else
		{
			temp1=temp1->next;
		}
	}
	for(int i=0;i<strlen(a);i++)
        printf("%c",a[i]);
	printf("\nԪ�ظ�����%d\n",count);
	printf("�Ӽ�������%lf\n",pow(2,count));
}

void printfdata(struct node **headptrptr)
{
	struct node *temp;
	temp=*headptrptr;
	while(temp!=NULL)
	{
		printf("%c ",temp->count);
		temp=temp->next;
	}
	printf("\n");
}

void duplicateremove(struct node **headptrptr)
{
	struct node *temp;
	temp=*headptrptr;
	if(temp->next != NULL)
    {
        while(temp->next->next!=NULL)
        {
            if(temp->next->count == temp->count)
            {
                struct node *te = temp->next;
                temp->next = temp->next->next;
                free(te);
            }
            else
                temp = temp->next;
        }
        if(temp->next!=NULL)
        {
            if(temp->next->count == temp->count)
            {
                free(temp->next);
                temp->next = NULL;
            }

        }
    }


}


void bubble(struct node **headptrptr)
{
	struct node *previous, *current;
	int tempValue;
	int flag=1; //��֤����ѭ���ı��
   	if(headptrptr==NULL)
	{
	   	return ;
	}
    while(flag>0)
	{
        flag=0;
        previous = *headptrptr;
		/*��ǰ�ڵ�ָ����Ч���ݽڵ�*/
   		if(*headptrptr != NULL)
   		{
   			current = (*headptrptr)->next;
		}
		else
		{
			current = NULL;
		}
        while(current!=NULL)
		{
           if(current->count < previous->count)
		   {
              tempValue = current->count;
              current->count = previous->count;
              previous->count = tempValue;
              flag++;
           }
           previous = current;
           current = current->next;
        }
    }
}

void merge(struct node **headptrptr1,struct node **headptrptr2)
{
	struct node *temp1,*temp2,*temp4;
	char a[100];
	int flag=0;
	int count=0;
	temp1=*headptrptr1;
	temp2=*headptrptr2;
	while(temp1!=NULL&&temp2!=NULL)
	{
		if(temp2->count<temp1->count)
		{
			for(int i=0;i<strlen(a);i++)
			{
				if(a[i]==temp2->count)
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
			{
				a[count]=temp2->count;
				count++;
			}
			temp2=temp2->next;
		}
		else if(temp2->count==temp1->count)
		{
			a[count]=temp2->count;
			count++;
			temp1=temp1->next;
			temp2=temp2->next;
		}
		else
		{
			for(int i=0;i<strlen(a);i++)
			{
				if(a[i]==temp1->count)
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
			{
				a[count]=temp1->count;
				count++;
			}
			temp1=temp1->next;
		}
		flag=0;
	}


	while(temp1)
	{
		a[count]=temp1->count;
		count++;
		temp1=temp1->next;
	}
	while(temp2)
	{
		a[count]=temp2->count;
		count++;
		temp2=temp2->next;
	}
	printf("���ϵĲ���");
	for(int i=0;i<count;i++)
		printf("%c",a[i]);
	printf("\nԪ�ظ�����%d\n",count);
	printf("�Ӽ�������%lf\n",pow(2,count));
}

void deletenode(struct node **headptrptr2,int num)
{
	struct node *previous;
	struct node *current;
	previous=NULL;
	current=*headptrptr2;
	while(current!=NULL&&current->count!=num)
	{
		previous=current;
		current=current->next;
	}
	if(current!=NULL)//����
	{
		if(previous==NULL)
		{
			*headptrptr2=current->next;
		}
		else
		{
			previous->next=current->next;
		}
	}
}

void insert(struct node **headptrptr1,int num)
{
	struct node *previous;//��ǰ��
	struct node *current;//���ڵ�
	struct node *news;//Ҫ�����
	previous = NULL;//��ֹҪ�����Ԫ���ڵ�һλ��
	current = *headptrptr1;
	while(current!=NULL&&current->count<num)
	{
		previous=current;
		current=current->next;
	}
	news=(struct node*)malloc(sizeof(struct node));
	if(news==NULL)
	{
		return ;
	}
	news->count=num;
	news->next=current;
	if(previous==NULL)
	{
		*headptrptr1=news;
	}
	else
	{
		previous->next=news;
	}
}

void difference(struct node **headptrptr1,struct node **headptrptr2)
{
	struct node *temp1,*temp2;
	temp1=*headptrptr1;
	temp2=*headptrptr2;
	char a[100];
	int count=0,flag=0;
	printf("���ϵĲ");
	while(temp1!=NULL&&temp2!=NULL)
	{
		if(temp2->count<temp1->count)
		{
			temp2=temp2->next;
		}
		else if(temp2->count==temp1->count)
		{
			temp1=temp1->next;
			temp2=temp2->next;
		}
		else
		{
			for(int i=0;i<strlen(a);i++)
			{
				if(a[i]==temp2->count)
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
			{
				a[count]=temp1->count;
				count++;
			}
			temp1=temp1->next;
		}
		flag=0;
	}
	while(temp1)
	{
		a[count]=temp1->count;
		count++;
		temp1=temp1->next;
	}
	for(int i=0;i<strlen(a);i++)
	printf("%c",a[i]);
	printf("\nԪ�ظ�����%d\n",count);
	printf("�Ӽ�������%lf\n",pow(2,count));
}

void Complement(struct node **headptrptr1)
{
	LIST *temp1;
	temp1=*headptrptr1;
	int count=0;
	for(int i=0;i<26;i++)
	{
		if(((temp1->count)>map[i])&&temp1)
		{
			printf("%c",map[i]);
			count++;
		}
		else if(((temp1->count)==map[i])&&temp1)
		{
			temp1=temp1->next;
			if(temp1==NULL)
			{
				for(int j=i+1;j<26;j++)
				{
					printf("%c",map[j]);
					count++;
				}
				i=25;
			}
		}
	}
	printf("\n");
}
