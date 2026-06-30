#include <stdio.h>
#include <stdlib.h>

struct node {
    int coeff;
    int exp;
    struct node *next;
}*poly=NULL;

void create()
{
    struct node *t,*last;
    int num, i;

    printf("Enter Number of Terms");
    scanf("%d",&num);
    printf("enter each term's coefficient and exponent");
    for(i=0;i<num;i++)
    {
        t=(struct node *)malloc(sizeof(struct node));
        scanf("%d%d",&t->coeff,&t->exp);
        t->next=NULL;
        if(poly==NULL)
            poly=last=t;
        else
            last->next=t;
        last=t;
    }
}

void display(struct node *p)
{
    while(p!=NULL)
    {
        printf("%dx^%d + ",p->coeff,p->exp);
        p=p->next;
    }
}

long Eval(struct node *p,int x)
{
    long val=0;
    while(p!=NULL)
    {
        val+=p->coeff*pow(x,p->exp);
        p=p->next;
    }
    return val;
}

int main()
{
    create();
    display(poly);
    printf("%d\n",Eval(poly,1));

    return 0;
}