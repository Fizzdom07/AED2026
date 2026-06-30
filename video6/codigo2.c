#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
}*Head=NULL;

void create(int A[],int n)
{
    int i;
    struct node *t,*last;
    Head=(struct node *)malloc(sizeof(struct node));
    Head->data=A[0];
    Head->next=Head;
    last=Head;

    for(int i=1;i<n;i++)
    {
        t=(struct node *)malloc(sizeof(struct node));
        t->data=A[i];
        t->next=last->next;
        last->next=t;
        last=t;
    }
}


void Display(struct node *h)
{
    struct node *p=h;
    do
    {
        printf("%d ",p->data);
        p=p->next;
    }while(p!=h);
    printf("\n");
}


void RDisplay(struct node *h)
{
    static int flag=0;
    if(h!=Head || flag==0)
    {
        flag=1;
        printf("%d ",h->data);
        RDisplay(h->next);
    }
    flag=0;
}


int Length(struct node *h)
{
    int len=0;
    struct node *p=h;
    do
    {
        len++;
        p=p->next;
    }while(p!=h);
    return len;
}
void Insert(struct node *p,int index,int x)
{
    struct node *t;
    int i;
    if(index<0 || index>count(p))
        return;
    t=(struct node *)malloc(sizeof(struct node));
    t->data=x;
    if(index==0)
    {
        if(Head==NULL)
        {
            Head=t;
            Head->next=Head;
        }
        else
        {
            while(p->next!=Head)
                p=p->next;
            p->next=t;
            t->next=Head;
            Head=t;
        }
    }
    else
    {
        for(i=0;i<index-1;i++)
            p=p->next;
        t->next=p->next;
        p->next=t;
    }
}

int delete(struct node *p,int index)
{
    struct node *q=NULL;
    int i,x=-1;
    if(index<1 || index>count(p))
        return -1;
    if(index==1)
    {
        while(p->next!=Head)
            p=p->next;
        x=Head->data;
        if(Head==p)
        {
            free(Head);
            Head=NULL;
        }
        else
        {
            p->next=Head->next;
            free(Head);
            Head=p->next;
        }
        return x;
    }
    else
    {
        for(i=0;i<index-1;i++)
        
            q=p;
            p=p->next;
        
        q->next=p->next;
        x=p->data;
        free(p);
        return x;
    }
}
int main ()
{
    int A[]={3,5,7,10,15};
    create(A,5);
    insert(Head,0,2);

    delete(Head,1);

    Display(Head);
    RDisplay(Head);
    return 0;


    return 0;
}