#include <iostream>
#include <cstdlib>
#include <iomanip>

using namespace std;

enum Menu
{
    CROWN,
    USDA,
    HONEY,
    PIT,
    XTRA
};

enum PaymentMode 
{
    CASH, 
    MARIBANK, 
    GCASH 
};
enum OrderMethod 
{
    DELIVERY, 
    PICKUP 
};

struct Order 
{
        Menu item;
        int quantity;
        double price;
        double subtotal;
};

struct CustomerRecord
{
    string fullName;
    string purok;
    string unitFloorHouseNo;
    string brgy;
    string municipality;
    string prov;
    string contactNo;
    OrderMethod orderMet;
    PaymentMode payMet;
    string timeSlot;
    int numOrders;  
    double total;
    double shippingFee;
};

struct QueueNode {
    CustomerRecord customer;
    QueueNode* next;
};

//QUEUE
struct Queue{
    QueueNode* front;
    QueueNode* rear;
    
    Queue() {front = rear = nullptr;}
    bool isEmpty() {return front == nullptr;}

    void enqueue(CustomerRecord c)
    {
        QueueNode* newNode = new QueueNode {c, nullptr};
        if (rear == nullptr)
        {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    void dequeue()
    {
        if (isEmpty()){
            cout << "\nQueue is Empty! No pickups to process.\n";
            return;
        }
        QueueNode* temp = front;
        cout<<"\n[SERVED]: "<<temp->customer.fullName<<"\n";
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
    }

};

//STACK
struct StackNode {
    CustomerRecord customer;
    StackNode* next;
};

StackNode* top = nullptr;

bool isStackEmpty() 
{
 return top == nullptr;
}

void push(CustomerRecord c) 
{
    StackNode* newStackNode = new StackNode;
    newStackNode->customer = c;
    newStackNode->next = top; 
    top = newStackNode;

    cout<<"\nConfirmed Orders: "<<newStackNode->customer.fullName<<endl;
}

void pop()
{
    if(isStackEmpty() == true)
    {
        cout<<"Stack is empty! No confirmed orders to undo."<<endl;
        return;
    }
    StackNode* temp = top;
    cout<<"Confirmation Undone: "<<top->customer.fullName<<endl;
    top = top->next;
    delete temp;
}

void peekStack()
{
    if(isStackEmpty() == true)
    {
        cout<<"No confirmed orders yet."<<endl;
    }
    else
    {
        cout<<"Most Recently Confirmed Order: "<<top->customer.fullName<<endl;
    }
}

void displayStack()
{
    cout<<"Confirmed Orders: "<<endl;
    if(isStackEmpty() == true)
    {
        cout<<"No confirmed orders yet."<<endl;
    }
    else
    {
        StackNode* current = top;
        while(current != nullptr)
        {
            cout<<current->customer.fullName<<endl;
            current = current->next;
        }
        cout<<endl;
    }
}
//-------------------------------------------------



//MENU
const int ITEMS = 5;
string menu[ITEMS][2] = 
{
    {"[CROWN] Crown Jewels Pork Belly", "500g "},
    {"[USDA] USDA Beef Shortplate    ", "500g "},
    {"[HONEY] Honey Cured Bacon      ", "500g "},
    {"[PIT] Pitmasters Sampler       ", "500g "},
    {"[XTRA] Extra BBQ sauce         ", "100mL"}
};

const double PRICES[ITEMS] = { 
    650.00, 
    900.00, 
    750.00, 
    1295.00, 
    70.00 
};

int stock[ITEMS] = { 
    10, 
    10, 
    10, 
    10, 
    10 
};

void displayMenu() 
{
    cout<<"\n-------------------------------------PITMASTER'S MENU--------------------------------------"<<endl;
    cout<<"No\t\t\tProduct\t\t\t\tPortion\tPrice\t Stock"<<endl;
    cout<<"-------------------------------------------------------------------------------------------"<<endl;

    for (int i = 0; i < ITEMS; i++)
    {
        cout<<i + 1<<"\t"<<menu[i][0]<<"\t\t"<<menu[i][1]<<"\t";
        cout<<"₱"<<fixed<<setprecision(2)<<PRICES[i];

        if (PRICES[i] < 100)// para lang pumantay sa column
        {
            cout<<"  ";
        }
        else
        {
            if (PRICES[i] < 1000)   
            {
                cout<<" ";
            }
        }
        
        if (stock[i] > 0)
        {
            cout<<"  "<<stock[i]<<endl;

            if (stock[i] <= 3)
            {
                cout<<" (Low stock)"<<endl;
            }
        }
        else
        {
            cout<<"  SOLD OUT"<<endl;
        }
    }
    cout<<"-------------------------------------------------------------------------------------------"<<endl;
}

void printOrders(CustomerRecord customers[], Order orders[100][100], int y, int x, double subTb4Sf, double itemTot)
{
    cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
    cout<<"🏷️ Name : "<<customers[y].fullName<<endl;
    cout<<"📍 Address: Purok "<<customers[y].purok<<", #"<<customers[y].unitFloorHouseNo<<", Barangay "<<customers[y].brgy<<", "<<customers[y].municipality<<", "<<customers[y].prov<<endl;
    cout<<"📞 Contact: "<<customers[y].contactNo<<endl;

    cout<<"────────────────────────────────────── ∘₊✧ ORDER ✧₊ ───────────────────────────────────────\n";
    cout<<"\tNo\t\t\t\t\tItem\t\tQuantity\tPrice\tPer Item Total\n";
    cout<<"────────────────────────────────────────────────────────────────────────────────────────────"<<endl;

    // Loop through each order of a customer
    for(x = 0; x < customers[y].numOrders; x++)
    {
        cout<<"\t"<<x + 1<<"\t\t\t";

        // Display item name based on enum value
        if (orders[y][x].item == CROWN)
        {
            cout<<"THE CROWN JEWELS Smoked Pork Belly\t\t";
        }

        else if (orders[y][x].item == USDA)
        {
            cout<<"USDA Smoked Beef Shortplate       \t\t";
        }

        else if (orders[y][x].item == HONEY)
        {
            cout<<"Artisan Honey Cured Smoked Bacon  \t\t";
        }

        else if (orders[y][x].item == PIT)
        {
            cout<<"The Pitmaster's Sampler           \t\t";
        }

        else
        {
            cout<<"Extra Signature BBQ Sauce         \t\t";
        }

        cout<<orders[y][x].quantity<<"\t";
        cout<<"₱"<<fixed<<setprecision(2)<<orders[y][x].price<<" \t";

        //Used displaying each item total by multiplying quantity and price
        itemTot = orders[y][x].quantity * orders[y][x].price;
        cout << fixed << setprecision(2) << " ₱" << itemTot << endl;
       
    }
    cout<<"──────────────────────────────────────── ˗ˏˋ⛁$ˎˊ˗ ─────────────────────────────────────────"<<endl;
    //Subtotal after adding the total per item and before shipping fee
    subTb4Sf = customers[y].total - customers[y].shippingFee;
    cout<<"💲 Subtotal :                                                                   ₱"<<fixed<<setprecision(2)<<subTb4Sf<<endl;
    cout<<"🚚 Shipping Fee:                                                                ₱"<<fixed<<setprecision(2)<<customers[y].shippingFee<<endl;
    cout<<"💸 Total:                                                                       ₱"<<fixed<<setprecision(2)<<customers[y].total<<endl;
    cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;

    // Display order method
    cout<<"Order Method : ";

    if (customers[y]. orderMet == PICKUP)
    {
        cout<<"Pick-up\n";
        cout<<"🕒 Pick-up Time : "<<customers[y].timeSlot<<endl;
    }

    else
    {
        cout<<"📦 Delivery\n";
        cout<<"🕒 Delivery Time : "<<customers[y].timeSlot<<endl;
    }

    // Display payment method
    cout<<"🧾 Payment Method: ";

    if (customers[y].payMet == CASH)
    {
        cout<<"💰 Cash\n";
    }

    else if (customers[y].payMet == MARIBANK)
    {
        cout<<"🏦 Maribank\n";
    }

    else
    {
        cout<<"💳 G-Cash\n";
    }

    cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
};


//OPTION 1
void registerPreOrder(CustomerRecord customers[], Order orders[][100], int &numOfCus, Queue &pickupQueue) {

    displayMenu();

    cout << "\n--- REGISTER PRE-ORDER ---\n";
    cout << "Full Name: ";
    getline(cin, customers[numOfCus].fullName);
    cout<<"Full Name: ";
    getline(cin, customers[numOfCus].fullName);
    cout<<"Purok (No.): ";
    getline(cin, customers[numOfCus].purok);
    cout<<"Unit No/Floor/House No.: ";
    getline(cin, customers[numOfCus].unitFloorHouseNo);
    cout<<"Baranggay: ";
    getline(cin, customers[numOfCus].brgy);
    cout<<"Municipality: ";
    getline(cin, customers[numOfCus].municipality);
    cout<<"Povince: ";
    getline(cin, customers[numOfCus].prov);
    cout << "Contact No.: ";
    getline(cin, customers[numOfCus].contactNo);

    cout << "How many items in order?: ";
    cin >> customers[numOfCus].numOrders;
    cin.ignore();

    customers[numOfCus].total = 0;

    for (int x = 0; x < customers[numOfCus].numOrders; x++) {
        cout << "Select Item [1] Pork Belly | [2] Beef Shortplate | [3] Bacon | [4] Sampler | [5] Sauce: ";
        int choice;
        cin >> choice;
        
        if (choice == 1) { orders[numOfCus][x].item = CROWN; orders[numOfCus][x].price = 650.00; }
        else if (choice == 2) { orders[numOfCus][x].item = USDA; orders[numOfCus][x].price = 900.00; }
        else if (choice == 3) { orders[numOfCus][x].item = HONEY; orders[numOfCus][x].price = 750.00; }
        else if (choice == 4) { orders[numOfCus][x].item = PIT; orders[numOfCus][x].price = 1295.00; }
        else { orders[numOfCus][x].item = XTRA; orders[numOfCus][x].price = 70.00; }

        cout << "Quantity: ";
        cin >> orders[numOfCus][x].quantity;
        cin.ignore();

        orders[numOfCus][x].subtotal = orders[numOfCus][x].price * orders[numOfCus][x].quantity;
        customers[numOfCus].total += orders[numOfCus][x].subtotal;
    }

    cout << "Order Method ([1] Delivery | [2] Pick-up): ";
    int method;
    cin >> method;
    cin.ignore();

    if (method == 1) {
        customers[numOfCus].orderMet = DELIVERY;
        cout << "Delivery Time (HH:MM): ";
        getline(cin, customers[numOfCus].timeSlot);
        cout << "Shipping Fee: P";
        cin >> customers[numOfCus].shippingFee;
        cin.ignore();
        customers[numOfCus].total += customers[numOfCus].shippingFee;
    } else {
        customers[numOfCus].orderMet = PICKUP;
        cout << "Pick-up Time (HH:MM): ";
        getline(cin, customers[numOfCus].timeSlot);
        customers[numOfCus].shippingFee = 0;
    }

    pickupQueue.enqueue(customers[numOfCus]);

    cout << "\n[SUCCESS] Pre-order registered, queued, and pushed to stack!\n";
    numOfCus++;

    cout<<"Payment Mode [1] | [2] Maribank | [3] G-Cash: ";
    int pChoice;
    cin >> pChoice;
    cin.ignore();
    if (pChoice==1)
    {
        customers[numOfCus].payMet = CASH;
    }else if (pChoice==2)
    {
        customers[numOfCus].payMet=MARIBANK;
    }else
    {
        customers[numOfCus].payMet=GCASH;
    }
    
    pickupQueue.enqueue(customers[numOfCus]);
    push(customers[numOfCus]);

    cout<<"\n [SUCCESS] Pre-order registered!\n";
    numOfCus++;
}

//OPTION 2
void processNextOrder(Queue &pickupQueue)
{
    pickupQueue.dequeue();
}

//OPTION 3

//OPTION 4

//OPTION 5

//OPTION 6
void undoLastOrder()
{
    pop();
}

//OPTION 7
void viewLastConfirmedOrder()
{
    peekStack();
}

int main()
{
    CustomerRecord customers[100];
    Order orders [100][100];
    int numOfCus = 0, option;
    Queue pickupQueue;

    cout<<"╔═══════════════════════════════════════════════════════════════════════════════════════════╗"<<endl;
    cout<<"                                       ♨️ VICEROY ♨️                                       "<<endl;
    cout<<"                                  LOW AND SLOW SINCE 2019                                  "<<endl;
    cout<<"                     FB: Viceroy Smoked BBQ | Contact No.: 09279855649                     "<<endl;
    cout<<"                                 Quezon City, Philippines                                  "<<endl;
    cout<<"╚═══════════════════════════════════════════════════════════════════════════════════════════╝"<<endl<<endl;

    cout<<"                                         WELCOME!                                          "<<endl;

    do{
        //Program or System Menu
        //1. Register Pre-order (Queue)             5. Sort Order (Bubble or Merge)
        //2. Process Next Order (Dequeu, Push)      6. Undo Last Confirmed Order (Pop, Queue)
        //3. Display Orders (Peek Queue)            7. View Last Confirmed Order (Peek Stack)
        //4. Search Order (Linear or Binary)        8. EXIT
        
        cout<<"─────────────────────────────────────── VICEROY|MENU ──────────────────────────────────────"<<endl;
        cout<<"                 [1] Register Pre-order           [5] Sort Orders                          "<<endl;
        cout<<"                 [2] Process Next Order           [6] Undo Last Confirmed Order            "<<endl;
        cout<<"                 [3] Display Orders               [7] View Last Confirmed Order            "<<endl;
        cout<<"                 [4] Search Order                 [8] EXIT                                 "<<endl;
        cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;

        cout<<"Pitmaster, select adn option (1 - 8): ";
        cin>>option;

        switch (option)
        {
            case 1:
            {
                break;
            }
            case 2:
            {
                break;
            }
            case 3:
            {
                break;
            }
            case 4:
            {
                break;
            }
            case 5:
            {
                break;
            }
            case 6:
            {
                break;
            }
            case 7:
            {
                break;
            }
            case 8:
            {
                cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;
                cout<<"                                        THANK YOU!                                         "<<endl;
                break;
            }
            default:
            {
                cout<<"Invalid Option, Pitmaster."<<endl;
                cout<<"Pick Again."<<endl;
            }
        }

    }while (option != 8);
    
    return EXIT_SUCCESS;

}