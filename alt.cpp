#include <iostream>
#include <cstdlib>

using namespace std;

// LINKED LIST (Confirmed Orders)
struct ListNode
{
    CustomerRecord customer      // Stores the value of the node
    ListNode* next;     // Stores the address of the next node
};

ListNode* head = nullptr;

bool isListEmpty()
{
    return head == nullptr;
}

int countLIst()
{
    int count = 0;
    ListNode* current = head;

    while (Current != nullptr)
    {
        count++;
        current = current->next;
    }
    return count;
}

//INSERT AT THE BEGINNNING
void insertAtBeginning(CustomerRecord c)
{
    //Create a new node
    ListNode* newNode = new ListNode;
    newNode->customer = c;

    newNode->next = head;
    head = newNode;

    cout<<"\nConfirmed Order: "<<newNode->customer.fullName<<endl;
    cout<<"                                      ---Order Details---"<<endl;
    printOrders(newNode->customer, orders);
}

//REMOVE A NODE AT A POSITION
bool removeAtPosition(int position, CustomerRecord &removed)
{
    if (position < 1 || position > countList())
    {
        return false;
    }

    if (position == 1)
    {
        ListNode* temp = head;
        removed = temp->customer;
        head = head->next;
        delete temp;
    }
    else
    {
        ListNode* current = head;

        for(int i = 1; i < position - 1; i++)
        {
            current = current->next;
        }

        ListNode* temp = current->next;
        removed = temp->customer;
    }
}