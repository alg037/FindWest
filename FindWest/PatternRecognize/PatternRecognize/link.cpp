#include "link.h"
#include <iostream>

using namespace std;

void insertNode(Node *p,pixel value)
{
	Node *node=new Node;
	Node *q=p;
	if (p->next==NULL) //head
	{
		p->value=value;
		p->next=node;
		node->next=NULL;
	}
	else
	{
		while (q->next!=NULL) q=q->next;
		q->value=value;
		q->next=node;
		node->next=NULL;
	}
}

void printNode(Node *p)
{
	Node *node=p;
	int no=0;
	while (node->next!=NULL)
	{
		no++;
		cout<<"("<<node->value.col<<","<<node->value.row<<") ";
		node=node->next;
	}
		cout<<endl;
}
