#include <stdio.h>
#include <stdlib.h>

struct node {
    struct node *prev;
    int data;
    struct node *next;
}*first=NULL,*second=NULL,*third=NULL;

void create(int A[],int n)
{
    int i;
    struct node *t,*last;
    first=(struct node *)malloc(sizeof(struct node));
    first->data=A[0];
    first->prev=first->next=NULL;
    last=first;

    for(int i=1;i<n;i++)
    {
        t=(struct node *)malloc(sizeof(struct node));
        t->data=A[i];
        t->next=last->next;
        t->prev=last;
        last->next=t;
        last=t;
    }
}

void Display(struct node *p)
{
    while(p!=NULL)
    {
        printf("%d ",p->data);
        p=p->next;
    }
}

int Length(struct node *p)
{
    int len=0;
    while(p!=NULL)
    {
        len++;
        p=p->next;
    }
    return len;
}

void insert(struct node *p,int index,int x)
{
    struct node *t;
    int i;
    if(index<0 || index>count(p))
        return;
    
    if(index==0)
    {
        t=(struct node *)malloc(sizeof(struct node));
        t->data=x;
        t->prev=NULL;
        t->next=first;
        first->prev=t;
        first=t;
    }
    else
    {
        for(i=0;i<index-1;i++)
            p=p->next;
        t=(struct node *)malloc(sizeof(struct node));
        t->data=x;    
        t->next=p->next;
        t->prev=p;
        if(p->next)
            p->next->prev=t;
        p->next=t;
    }
}

int Delete(struct node *p,int index)
{
    struct node *q=NULL;
    int i,x=-1;
    if(index<1 || index>count(p))
        return -1;
    if(index==1)
    {
        q=first;
        
        first=first->next;
        if(first)
            first->prev=NULL;
        x=p->data;    
        free(q);

        return x;
    }
    else
    {
        for(i=0;i<index-1;i++)
        
            q=p;
            p=p->next;
        
        q->next=p->next;
        if(p->next)
            p->next->prev=q;
        x=p->data;
        free(p);
        return x;
    }
}


void Reverse(struct node *p)
{
    struct node *temp;
    while(p!=NULL)
    {
        temp=p->next;
        p->next=p->prev;
        p->prev=temp;
        p=p->prev;
        if(p!=NULL && p->next==NULL)
            first=p;
    }
}
int main ()
{
    int A[]={3,5,7,10,15};
    create(A,5);
    Display(first);
    printf("\nLength is %d\n",Length(first));
    insert(first,0,2);
    Display(first);
    Delete(first,1);
    Display(first);
    Reverse(first);
    return 0;
}