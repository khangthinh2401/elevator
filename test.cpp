#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>

#define ll long long
#define vdb vector<double>
#define vll vector<long long>
using namespace std;

struct node
{
    int data;
    node *next;
};

void createNode(node *&, int);
void deleteNode(node *&, int);
node *insertNode(node *&, int);
vector<int> getuplist(node *&head);
void solve(node *&head);
int main()
{
    node *list_head = nullptr;
    list_head = insertNode(list_head, 5);
    list_head = insertNode(list_head, 6);
    solve(list_head);
    return 0;
}

void createNode(node *&h, int x)
{
    h = new node{x, nullptr};
}
void deleteNode(node *&head, int position)
{
    if (position == 0)
    {
        node *x = head;
        head = head->next;
        free(x);
    }
}
node *insertNode(node *&head, int x)
{
    if (head == nullptr)
    {
        createNode(head, x);
        return head;
    }
    node *p = head;
    while (p->next != nullptr)
    {
        p = p->next;
    }
    node *temp = new node{x, nullptr};
    p->next = temp;
    return head;
}
void solve(node *&head)
{
    cout << head->data << endl;
    vector<int> vt = getuplist(head);
    cout << head->data << endl;
}
vector<int> getuplist(node *&head)
{
    vector<int> a = {1, 2, 3};
    deleteNode(head, 0);
    return a;
}