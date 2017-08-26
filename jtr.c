#include "jtre.h"
#include<stdio.h>

insert(int val,struct jtr *t){
struct jtr *te=search(val,t);

}

struct jtr *searchp(int val, char *h, struct jtr *t){
struct jtr *temp=t;
while(t){
temp=t;
if(t->val<val) t=t->left;
 else t=t->right;
if(t->val==val) break;

}
if(temp->val=t->val) return temp;
if(temp->right->val==t->val) *h='r';
else *h='l';
return temp;
}
search(int val,struct jtr *t){
char *h='o';
struct jtr *par= searchp(val,&h,t);
if(h=='r') return par->right;
if(h=='l') return par->left;
return par;
}
jtre(int val,struct jtr *t){
char *h='o';
struct jtr *par=searchp(val,&h,t);
struct jtr *temp1,*temp,*noder,*node_l;
if(h=='r')
 noder=par->right;
if(h=='l')
node_l=par->left;
if(noder){
temp1=noder->left;
temp=noder->right;
noder->left=par;
par->left=temp1;
par->right=temp;
}
if(node_l){
temp=node_l->right;
temp1=node_l->left;
node_l->right=par;
node_l->left=par->right;
par->right=temp;
par->left=temp1;
}
}
