#include <stdio.h>

struct node {
    int data;
    struct node *next;
}*first=NULL,*second=NULL,*third=NULL;

void create(int A[],int n)
{
    int i;
    struct node *t,*last;
    first=(struct node *)malloc(sizeof(struct node));
    first->data=A[0];
    first->next=NULL;
    last=first;

    for(int i=1;i<n;i++)
    {
        t=(struct node *)malloc(sizeof(struct node));
        t->data=A[i];
        t->next=NULL;
        last->next=t;
        last=t;
    }
}


void create2(int A[],int n)
{
    int i;
    struct node *t,*last;
    second=(struct node *)malloc(sizeof(struct node));
    second->data=A[0];
    second->next=NULL;
    last=second;

    for(int i=1;i<n;i++)
    {
        t=(struct node *)malloc(sizeof(struct node));
        t->data=A[i];
        t->next=NULL;
        last->next=t;
        last=t;
    }

    first->next=NULL;
    last=first;

    for(int i=1;i<n;i++)
    {
        t=(struct node *)malloc(sizeof(struct node));
        t->data=A[i];
        t->next=NULL;
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
int count(struct node *p)
{
    int l=0;
    while(p)
    {
        l++;
        p=p->next;
    }
    return l;

}

int Rcount(struct node *p)
{
    if(p!=NULL)
        return RcCount(p->next)+1;
    else
        return 0;
}

int sum(struct node *p)
{
    int s=0;
    while(p!=NULL)
    {
        s+=p->data;
        p=p->next;
    }
    return s;

}

int Rsum(struct node *p)
{
    if(p==NULL)
        return 0;
    else
        return Rsum(p->next)+p->data;
}

int Max(struct node *p)
{
    int max=INT32_MIN;
    while(p)
    {
        if(p->data>max)
            max=p->data;
        p=p->next;
    }
    return max;
}

int RMax(struct node *p)
{
    int x=0;
    if(p==0)
        return INT32_MIN;
    else
    {
        x=RMax(p->next);
        if(x>p->data)
            return x;
        else
            return p->data;
    }
}

struct node* LSearch(struct node *p,int key)
{
    struct node *q;
    while(p!=NULL)
    {
        if(key==p->data)
        {
            q->next=p->next;
            p->next=first;
            first=p;
            return p;
        }
        q=p;
        p=p->next;
    }
    return NULL;
}

struct node* RSearch(struct node *p,int key)
{
    if(p==NULL)
        return NULL;
    if(key==p->data)
        return p;
    return RSearch(p->next,key);
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
        t->next=first;
        first=t;
    }
    else
    {
        for(i=0;i<index-1;i++)
            p=p->next;
        t->next=p->next;
        p->next=t;
    }
}

void SortedInsert(struct node *p,int x)
{
    struct node *t,*q=NULL;
    t=(struct node *)malloc(sizeof(struct node));
    t->data=x;
    t->next=NULL;

    if(first==NULL)
        first=t;
    else
    {
        while(p && p->data<x)
        
            q=p;
            p=p->next;
        
        if(p==first)
        {
            t->next=first;
            first=t;
        }
        else
        {
            t->next=q->next;
            q->next=t;
        }
    }
}

void delete(struct node *p,int index)
{
    struct node *q=NULL;
    int i,x=-1;
    if(index<1 || index>count(p))
        return -1;
    if(index==1)
    {
        q=first;
        x=first->data;
        first=first->next;
        free(q);
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

int isSorted(struct node *p)
{
    int x=-8726762;
    while(p!=NULL)
    {
        if(p->data<x)
            return 0;
        x=p->data;
        p=p->next;
    }
    return 1;
}

void RemoveDuplicate(struct node *p)
{
    struct node *q=p->next;
    while(q!=NULL)
    {
        if(p->data!=q->data)
        {
            p=q;
            q=q->next;
        }
        else
        {
            p->next=q->next;
            free(q);
            q=p->next;
        }
    }
}

void reverse1(struct node *p)
{
    int *A,i=0;
    struct node *q=p;
    A=(int *)malloc(sizeof(int)*count(p));
    while(q!=NULL)
    {
        A[i]=q->data;
        q=q->next;
        i++;
    }
    q=p;
    i--;
    while(q!=NULL)
    {
        q->data=A[i];
        q=q->next;
        i--;
    }
}

void reverse2(struct node *p)
{
    struct node *q=NULL,*r=NULL;
    while(p!=NULL)
    {
        r=q;
        q=p;
        p=p->next;
        q->next=r;
    }
    first=q;
}

void reverse3(struct node *q,struct node *p)
{
    if(p)
    {
        reverse3(p,p->next);
        p->next=q;
    }
    else
        first=q;
}

void Concat(struct node *p,struct node *q)
{
    third=p;
    while(p->next!=NULL)
        p=p->next;
    p->next=q;
}

void merge(struct node *p,struct node *q)
{
    struct node *last;
    if(p->data<q->data)
    {
        third=last=p;
        p=p->next;
        last->next=NULL;
    }
    else
    {
        third=last=q;
        q=q->next;
        last->next=NULL;
    }
    while(p && q)
    {
        if(p->data<q->data)
        {
            last->next=p;
            last=p;
            p=p->next;
            last->next=NULL;
        }
        else
        {
            last->next=q;
            last=q;
            q=q->next;
            last->next=NULL;
        }
    }
    if(p){last->next=p;};
    
    if(q){last->next=q;};
    

}

int isLoop(struct node *f)
{
    struct node *p,*q;
    p=q=f;
    do
    {
        p=p->next;
        q=q->next;
        q=q?q->next:q;
    }while(p && q && p!=q);
    if(p==q)
        return 1;
    else
        return 0;
}
int main ()
{
    struct node *temp; 
    struct node *t1,*t2;
    int A[]={3,5,7,10,15};
    create(A,5);
    printf("Length is %d\n\n",count(first));
    printf("Length is %d\n\n",Rcount(first));
    printf("Sum is %d\n\n",sum(first));
    printf("Sum is %d\n\n",Rsum(first));
    printf("Max is %d\n\n",Max(first));
    printf("Max is %d\n\n",RMax(first));
    temp=LSearch(first,10);
    if(temp)
        printf("Key is Found %d\n\n",temp->data);
    else
        printf("Key is Not Found\n\n");
    temp=RSearch(first,10);
    if(temp)
        printf("Key is Found %d\n\n",temp->data);
    else
        printf("Key is Not Found\n\n");
    Display(first);
    printf("\n");
    t1=first->next->next;
    t2=first->next->next->next->next;
    t2->next=t1;
    printf("Is Loop: %d\n", isLoop(first));
    return 0;
}