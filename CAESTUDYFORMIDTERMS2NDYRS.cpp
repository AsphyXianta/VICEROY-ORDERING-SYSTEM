#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>

using namespace std;

enum Menu
{
    CROWN,
    USDA,
    HONEY,
    PIT,
    XTRA
};

enum BookingType
{
    SECURED,        //Paid
    SHORTLISTED     //Not paid. Fulfillment depends sa stock and FIFO.
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

enum TimePeriod
{
    AM,  // Morning
    PM   // Afternoon / evening
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
    BookingType bookType;
    BookingType origBookType;   // Booking type chosen at registration (bookType becomes SECURED once promoted)
    PaymentMode payMet;
    string timeSlot;
    TimePeriod period;   // AM or PM of the timeSlot
    int numOrders;  
    int idx;        // Row of a customer in the customers[] and orders[][] arrays (used to find their orders)
    double total;
    double shippingFee;
};

CustomerRecord customers[100];
Order orders[100][100];

//MENU
const int ITEMS = 5;        // Number of items on the menu
string menu[ITEMS][2] =     // Menu table: [i][0] = product name, [i][1] = portion size
{
    {"[CROWN] Crown Jewels Pork Belly", "500g "},
    {"[USDA] USDA Beef Shortplate    ", "500g "},
    {"[HONEY] Honey Cured Bacon      ", "500g "},
    {"[PIT] Pitmasters Sampler       ", "500g "},
    {"[XTRA] Extra BBQ sauce         ", "100mL"}
};

const double PRICES[ITEMS] = {      // Price of each menu item (same order as the Menu enum)
    650.00, 
    900.00, 
    750.00, 
    1295.00, 
    70.00 
};

int stock[ITEMS] = {        // Remaining stock of each menu item (same order as the Menu enum)
    10, 
    10, 
    10, 
    10, 
    10 
};

void displayMenu() 
{
    cout<<"\n-------------------------------------PITMASTER'S MENU--------------------------------------"<<endl;
    cout << left << setw(5) << "No"
         << setw(35) << "Product"
         << setw(12) << "Portion"
         << right << setw(12) << "Price"
         << setw(18) << "Stock" << endl;
    cout << string(82, '-') << endl;

    for (int i = 0; i < ITEMS; i++)
    {
        string stockLabel;
        if (stock[i] > 0)
        {
            stockLabel = to_string(stock[i]);
            if (stock[i] <= 3)
            {
                stockLabel += " (Low stock)";
            }
        }
        else
        {
            stockLabel = "SOLD OUT";
        }

        cout << left << setw(5) << i + 1
             << setw(35) << menu[i][0]
             << setw(12) << menu[i][1]
             << right << setw(2) << "₱" << setw(11) << fixed << setprecision(2) << PRICES[i]
             << setw(18) << stockLabel << endl;
    }
    cout << string(82, '-') << endl;
}

bool takeStock(CustomerRecord c)
{
    for (int k = 0; k < c.numOrders; k++)
    {
        int requestedQuantity = orders[c.idx][k].quantity;
        for (int previous = 0; previous < k; previous++)
        {
            if (orders[c.idx][previous].item == orders[c.idx][k].item)
            {
                requestedQuantity += orders[c.idx][previous].quantity;
            }
        }

        if (requestedQuantity > stock[orders[c.idx][k].item])
        {
            return false;
        }
    }
 
    for (int k = 0; k < c.numOrders; k++)
    {
        stock[orders[c.idx][k].item] =  stock[orders[c.idx][k].item] - orders[c.idx][k].quantity;
    }
    return true;
}

// Gives the items of a customer back to the stock (opposite of takeStock)
void returnStock(CustomerRecord c)
{
    for (int k = 0; k < c.numOrders; k++)
    {
        stock[orders[c.idx][k].item] = stock[orders[c.idx][k].item] + orders[c.idx][k].quantity;
    }
}

void printOrders(CustomerRecord c, Order orders[100][100])
{
    double subTb4Sf;
    double itemTot;

    cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
    cout<<"🏷️ Name : "<<c.fullName<<endl;
    cout<<"📍 Address: Purok "<<c.purok<<", #"<<c.unitFloorHouseNo<<", Barangay "<<c.brgy<<", "<<c.municipality<<", "<<c.prov<<endl;
    cout<<"📞 Contact: "<<c.contactNo<<endl;

    cout<<"────────────────────────────────────── ∘₊✧ ORDER ✧₊ ───────────────────────────────────────\n";
    cout << left << setw(6) << "No"
         << setw(38) << "Item"
         << right << setw(12) << "Quantity"
         << setw(14) << "Price"
         << setw(18) << "Per Item Total" << endl;
    cout << string(88, '-') << endl;

    // Loop through each order of a customer
    for(int x = 0; x < c.numOrders; x++)
    {
        // Display item name based on enum value
        string itemName;
        if (orders[c.idx][x].item == CROWN)
        {
            itemName = "THE CROWN JEWELS Smoked Pork Belly";
        }

        else if (orders[c.idx][x].item == USDA)
        {
            itemName = "USDA Smoked Beef Shortplate";
        }

        else if (orders[c.idx][x].item == HONEY)
        {
            itemName = "Artisan Honey Cured Smoked Bacon";
        }

        else if (orders[c.idx][x].item == PIT)
        {
            itemName = "The Pitmaster's Sampler";
        }

        else
        {
            itemName = "Extra Signature BBQ Sauce";
        }

        //Used displaying each item total by multiplying quantity and price
        itemTot = orders[c.idx][x].quantity * orders[c.idx][x].price;
        cout << left << setw(6) << x + 1
             << setw(38) << itemName
             << right << setw(12) << orders[c.idx][x].quantity
             << setw(2) << "₱" << setw(13) << fixed << setprecision(2) << orders[c.idx][x].price
             << setw(2) << "₱" << setw(17) << itemTot << endl;
       
    }
    cout<<"──────────────────────────────────────── ˗ˏˋ⛁$ˎˊ˗ ─────────────────────────────────────────"<<endl;
    //Subtotal after adding the total per item and before shipping fee
    subTb4Sf = c.total - c.shippingFee;
    cout<<"💲 Subtotal :                                                                    "<<fixed<<setprecision(2)<<"₱"<<subTb4Sf<<endl;
    cout<<"🚚 Shipping Fee:                                                                 "<<fixed<<setprecision(2)<<"₱"<<c.shippingFee<<endl;
    cout<<"💸 Total:                                                                        "<<fixed<<setprecision(2)<<"₱"<<c.total<<endl;
    cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;

    // Display order method
    cout<<"Order Method : ";

    if (c.orderMet == PICKUP)
    {
        cout<<"Pick-up\n";
        cout<<"🕒 Pick-up Time : "<<c.timeSlot<<" ";

        if (c.period == AM)
        {
            cout<<"AM"<<endl;
        }
        else
        {
            cout<<"PM"<<endl;
        }
    }

    else
    {
        cout<<"📦 Delivery\n";
        cout<<"🕒 Delivery Time : "<<c.timeSlot<<" ";

        if (c.period == AM)
        {
            cout<<"AM"<<endl;
        }
        else
        {
            cout<<"PM"<<endl;
        }
    }

    // Display payment method
    cout<<"🧾 Payment Method: ";

    if (c.payMet == CASH)
    {
        cout<<"💰 Cash";
    }

    else if (c.payMet == MARIBANK)
    {
        cout<<"🏦 Maribank";
    }

    else
    {
        cout<<"💳 G-Cash";
    }

    // Show the booking type beside the payment method
    if (c.bookType == SECURED)
    {
        cout<<" [Secured]"<<endl;
    }
    else
    {
        cout<<" [Shortlisted]"<<endl;
    }

    cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
}

void printEmptyList()
{
    cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;
    cout<<"                              Stack Status: EMPTY"<<endl;
    cout<<"-------------------------------------------------------------------------------------------"<<endl;
    cout<<"                              📦 NO CONFIRMED ORDERS"<<endl;
    cout<<"                       There are currently no orders in the stack."<<endl;
    cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;
}

void printEmptySecuredQueue()
{
    cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;
    cout<<"                              QUEUE STATUS: EMPTY"<<endl;
    cout<<"-------------------------------------------------------------------------------------------"<<endl;
    cout<<"                              📦 NO PENDING SECURED ORDERS"<<endl;
    cout<<"                       There are currently no secured customers in the queue."<<endl;
    cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;
}

void printEmptyShortlistedQueue()
{
    cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;
    cout<<"                              QUEUE STATUS: EMPTY"<<endl;
    cout<<"-------------------------------------------------------------------------------------------"<<endl;
    cout<<"                              📦 NO PENDING SHORTLISTED ORDERS"<<endl;
    cout<<"                       There are currently no shortlisted customers in the queue."<<endl;
    cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;
}


//QUEUE 
struct QueueNode {
    CustomerRecord customer;    // The customer data stored in this node
    QueueNode* next;            // Points to the next node in the queue (nullptr if last)
};

//Secured Queue
QueueNode* securedFront =  nullptr;     // First customer in the Secured queue (nullptr = empty)
QueueNode* securedRear =  nullptr;      // Last customer in the Secured queue (nullptr = empty)

bool isSecuredQueueEmpty()
{
    return securedFront == nullptr && securedRear == nullptr;
}

void enqueueSecured(CustomerRecord c)
{
    QueueNode* newQueueNode = new QueueNode; // New node that will hold the customer
    newQueueNode->customer = c;
    newQueueNode->next = nullptr;

    if(isSecuredQueueEmpty() == true)
    {
        securedFront = newQueueNode;
        securedRear = newQueueNode;
    }
    else
    {
        securedRear->next = newQueueNode;
        securedRear = newQueueNode;
    }
    cout<<"\nAdded to Secured Queue: "<<newQueueNode->customer.fullName<<endl;
    cout<<"                                      ---Order Details---"<<endl;
    printOrders(newQueueNode->customer, orders);
}

void dequeueSecured()
{
    if(isSecuredQueueEmpty() == true)
    {
        cout<<"                            ---No Secured Order to Serve---"<<endl;
        printEmptySecuredQueue();
        return;
    }
    QueueNode* temp = securedFront;     // Holds the old front node so it can be deleted
    cout<<"\n[SERVED]: "<<securedFront->customer.fullName<<endl;
    securedFront = securedFront->next;
 
    if (securedFront == nullptr)
    {
        securedRear = nullptr;
    }
    delete temp;
}

void peekSecuredQueue()
{
    if(isSecuredQueueEmpty() == true)
    {
        printEmptySecuredQueue();
    }
    else
    {
        cout<<"Next Secured Customer: "<<securedFront->customer.fullName<<endl;
        cout<<"                                      ---Order Details---"<<endl;
        printOrders(securedFront->customer, orders);
    }
}

void displaySecuredQueue()
{
    cout<<"Secured Orders: "<<endl;
    if(isSecuredQueueEmpty() == true)
    {
        printEmptySecuredQueue();
    }
    else 
    {
        QueueNode* current = securedFront;  // Walker pointer that moves through the queue
        int securedCount = 0;               // Counts customers in the Secured queue
 
        while(current != nullptr)
        {
            securedCount++;
            current = current->next;
        }
 
        cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
        cout<<"                              SECURED QUEUE: "<<securedCount<<" Pending Customers"<<endl;
        cout<<"-------------------------------------------------------------------------------------------"<<endl;
 
        current = securedFront;
        int countingSecured = 1;        // Numbering shown beside each secured customer
 
        while(current != nullptr)
        {
            cout<<"\n["<<countingSecured<<"] "<<current->customer.fullName<<endl;
            printOrders(current->customer, orders);
 
            current = current->next;
            countingSecured++;
        }
        cout<<endl;
    }
}

//SHORTLISTED QUEUE
QueueNode* shortlistedFront = nullptr;      // First customer in the Shortlisted queue (nullptr = empty)
QueueNode* shortlistedRear = nullptr;       // Last customer in the Shortlisted queue (nullptr = empty)

bool isShortlistedQueueEmpty()
{
    return shortlistedFront == nullptr && shortlistedRear == nullptr;
}

void enqueueShortlisted(CustomerRecord c)
{
    QueueNode* newQueueNode = new QueueNode; // New node that will hold the customer
    newQueueNode->customer = c;
    newQueueNode->next = nullptr;

    if(isShortlistedQueueEmpty() == true)
    {
        shortlistedFront = newQueueNode;
        shortlistedRear = newQueueNode;
    }
    else
    {
        shortlistedRear->next = newQueueNode;
        shortlistedRear = newQueueNode;
    }
    cout<<"\nAdded to Shortlisted Queue: "<<newQueueNode->customer.fullName<<endl;
    cout<<"                                      ---Order Details---"<<endl;
    printOrders(newQueueNode->customer, orders);
}

void dequeueShortlisted()
{
    if(isShortlistedQueueEmpty() == true)
    {
        cout<<"                            ---No Shortlisted Order to Remove---"<<endl;
        printEmptyShortlistedQueue();
        return;
    }
    else
    {
        QueueNode* temp = shortlistedFront;
        cout<<"\nRemoved from Shortlisted: "<<shortlistedFront->customer.fullName<<endl;
        shortlistedFront = shortlistedFront->next;
 
        if (shortlistedFront == nullptr)
        {
            shortlistedRear = nullptr;
        }
        delete temp;
    }
}

void peekShortlistedQueue()
{
    if(isShortlistedQueueEmpty() == true)
    {
        printEmptyShortlistedQueue();
    }
    else
    {
        cout<<"\nNext Shortlisted Customer: "<<shortlistedFront->customer.fullName<<endl;
        cout<<"                                      ---Order Details---"<<endl;
        printOrders(shortlistedFront->customer, orders);
    }
}

void displayShortlistedQueue()
{
    cout<<"Shortlisted Orders: "<<endl;
    if(isShortlistedQueueEmpty() == true)
    {
        printEmptyShortlistedQueue();
    }
    else
    {
        QueueNode* current = shortlistedFront;  // Walker pointer that moves through the queue
        int shortlistedCount = 0;               // Counts customers in the Shortlisted queue
 
        while(current != nullptr)
        {
            shortlistedCount++;
            current = current->next;
        }
 
        cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
        cout<<"                              SHORTLISTED QUEUE: "<<shortlistedCount<<" Pending Customers"<<endl;
        cout<<"-------------------------------------------------------------------------------------------"<<endl;
 
        current = shortlistedFront;
        int countingShortlisted = 1;    // Numbering shown beside each shortlisted customer
 
        while(current != nullptr)
        {
            cout<<"\n["<<countingShortlisted<<"] "<<current->customer.fullName<<endl;
            printOrders(current->customer, orders);
 
            current = current->next;
            countingShortlisted++;
        }
        cout<<endl;
    }
}
//-------------------------------------

// LINKED LIST (Confirmed Orders)
struct ListNode
{
    CustomerRecord customer;   // Stores the value of the node
    ListNode* next;     // Stores the address of the next node
};

ListNode* head = nullptr;

bool isListEmpty()
{
    return head == nullptr;
}

int countList()
{
    int count = 0;
    ListNode* current = head;

    while (current != nullptr)
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
    ListNode* newListNode = new ListNode;
    newListNode->customer = c;

    newListNode->next = head;
    head = newListNode;

    cout<<"\nConfirmed Order: "<<newListNode->customer.fullName<<endl;
    cout<<"                                      ---Order Details---"<<endl;
    printOrders(newListNode->customer, orders);
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
        current->next = temp->next;
        delete temp;
    }
    return true;
}

void peekList()
{
    if (isListEmpty())
    {
        printEmptyList();
    }
    else
    {
        cout<<"Most Recently Confirmed Order: "<<head->customer.fullName<<endl;
        cout<<"                                      ---Order Details---"<<endl;
        printOrders(head->customer, orders);
    }
}

void displayList()
{
    cout<<"Confirmed Orders: "<<endl;
    if (isListEmpty())
    {
        printEmptyList();
    }
    else
    {
         cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
        cout<<"                              CONFIRMED LIST: "<<countList()<<" Confirmed Customer"<<endl;
        cout<<"-------------------------------------------------------------------------------------------"<<endl;

        ListNode* current = head;
        int position = 1;

        while (current != nullptr)
        {
            cout<<"Position "<<position<<": "<<current->customer.fullName<<endl;
            cout<<"                                      ---Order Details---"<<endl;
            printOrders(current->customer, orders);
            current = current->next;
            position++;
        }
        cout<<endl;
    }
}
//-------------------------------------------------

//OPTION 1
void registerPreOrder(CustomerRecord customers[], Order orders[][100], int &numOfCus) 
{ 
    if (numOfCus >= 100)
    {
        cout<<"\nCannot register more customers. The record limit has been reached.\n";
        return;
    }

    cout << "\n--- REGISTER PRE-ORDER ---\n";
    
    cout<<"Full Name: ";
    getline(cin, customers[numOfCus].fullName);
    
    cout<<"Purok (No.): ";
    getline(cin, customers[numOfCus].purok);
    
    cout<<"Unit No/Floor/House No.: ";
    getline(cin, customers[numOfCus].unitFloorHouseNo);
    
    cout<<"Barangay: ";
    getline(cin, customers[numOfCus].brgy);
    
    cout<<"Municipality: ";
    getline(cin, customers[numOfCus].municipality);

    cout<<"Province: ";
    getline(cin, customers[numOfCus].prov);
    
    cout << "Contact No.: ";
    getline(cin, customers[numOfCus].contactNo);
    
    cout<<"-----------------------------------ORDER---------------------------------"<<endl;
    do
    {
        cout << "How many orders?: ";
        cin >> customers[numOfCus].numOrders;
        if (customers[numOfCus].numOrders < 1 || customers[numOfCus].numOrders > 100)
        {
            cout<<"Enter a number of orders between 1 and 100."<<endl;
        }
    } while (customers[numOfCus].numOrders < 1 || customers[numOfCus].numOrders > 100);
    cin.ignore();

    customers[numOfCus].total = 0;
    customers[numOfCus].idx = numOfCus;  
    
    displayMenu();

    for (int x = 0; x < customers[numOfCus].numOrders; x++) 
    {
        int availableStock = 0;
        bool validMenu = true;      // Stays true when the chosen menu number is valid
        do
        {
            bool hasAvailableStock = false;
            for (int i = 0; i < ITEMS; i++)
            {
                int remainingStock = stock[i];
                for (int previous = 0; previous < x; previous++)
                {
                    if (orders[numOfCus][previous].item == static_cast<Menu>(i))
                    {
                        remainingStock -= orders[numOfCus][previous].quantity;
                    }
                }

                if (remainingStock > 0)
                {
                    hasAvailableStock = true;
                    break;
                }
            }

            if (!hasAvailableStock)
            {
                cout << "No items are currently available. Pre-order registration cancelled." << endl;
                return;
            }

            cout << "Select Item (1 - 5)";
            int choice;
            cin >> choice;
            
            if (choice == 1) 
            { 
                orders[numOfCus][x].item = CROWN; 
                orders[numOfCus][x].price = 650.00; 
                
                validMenu = true;
            }
            
            else if (choice == 2) 
            { 
                orders[numOfCus][x].item = USDA; 
                orders[numOfCus][x].price = 900.00; 

                validMenu = true;
            }

            else if (choice == 3) 
            { 
                orders[numOfCus][x].item = HONEY; 
                orders[numOfCus][x].price = 750.00; 
                
                validMenu = true;
            }

            else if (choice == 4) 
            { 
                orders[numOfCus][x].item = PIT; 
                orders[numOfCus][x].price = 1295.00; 
                
                validMenu = true;
            }

            else if (choice == 5) 
            { 
                orders[numOfCus][x].item = XTRA; 
                orders[numOfCus][x].price = 70.00; 
                
                validMenu = true;
            }
            
            else
            {
                cout<<"Item not available. Pick again"<<endl;
                
                validMenu = false;
            }

            if (validMenu)
            {
                availableStock = stock[orders[numOfCus][x].item];
                for (int previous = 0; previous < x; previous++)
                {
                    if (orders[numOfCus][previous].item == orders[numOfCus][x].item)
                    {
                        availableStock -= orders[numOfCus][previous].quantity;
                    }
                }

                if (availableStock <= 0)
                {
                    cout << "Item sold out. Pick another." << endl;
                    validMenu = false;
                }
            }
        } while (validMenu == false);

        cout << "Quantity (1 - " << availableStock << "): ";
        cin >> orders[numOfCus][x].quantity;
        while (orders[numOfCus][x].quantity < 1 || orders[numOfCus][x].quantity > availableStock)
        {
            if (orders[numOfCus][x].quantity < 1)
            {
                cout<<"Quantity must be greater than 0. Enter again: ";
            }
            else
            {
                cout << "Quantity exceeds available stock (" << availableStock << "). Enter again: ";
            }
            cin >> orders[numOfCus][x].quantity;
        }
        cin.ignore();

        orders[numOfCus][x].subtotal = orders[numOfCus][x].price * orders[numOfCus][x].quantity;
        customers[numOfCus].total += orders[numOfCus][x].subtotal;
    }

    bool validOrderingMethod = true;        // Stays true when the order method is valid  
    do
    {
        cout << "Order Method ([1] Delivery | [2] Pick-up): ";
        int method;
        cin >> method;
        cin.ignore();

        if (method == 1) 
        {
            customers[numOfCus].orderMet = DELIVERY;
            cout << "Delivery Time (HH:MM): ";
            getline(cin, customers[numOfCus].timeSlot);

            bool validDeliveryPeriod = true;  // stays true when the AM/PM input is valid
            do
            {
                cout << "AM or PM: ";
                string periodInput;
                getline(cin, periodInput);

                if (periodInput == "AM" || periodInput == "am" || periodInput == "Am" || periodInput == "aM" ||
                    periodInput == "A.M." || periodInput == "A.M" || periodInput == "A.m." || periodInput == "A.m" ||
                    periodInput == "a.M." || periodInput == "a.M" || periodInput == "a.m." || periodInput == "a.m")
                {
                    customers[numOfCus].period = AM;

                    validDeliveryPeriod = true;
                }

                else if (periodInput == "PM" || periodInput == "pm" || periodInput == "Pm" || periodInput == "pM" ||
                        periodInput == "P.M." || periodInput == "P.M" || periodInput == "P.m." || periodInput == "P.m" ||
                        periodInput == "p.M." || periodInput == "p.M" || periodInput == "p.m." || periodInput == "p.m")
                {
                    customers[numOfCus].period = PM;

                    validDeliveryPeriod = true;
                }

                else
                {
                    cout << "Invalid. Type AM or PM only." << endl;

                    validDeliveryPeriod = false;
                }
            } while (validDeliveryPeriod == false);

            cout << "Shipping Fee: P";
            cin >> customers[numOfCus].shippingFee;
            cin.ignore();

            customers[numOfCus].total += customers[numOfCus].shippingFee;

            validOrderingMethod = true;
        } 
        
        else if (method == 2){
            customers[numOfCus].orderMet = PICKUP;
            cout << "Pick-up Time (HH:MM): ";
            getline(cin, customers[numOfCus].timeSlot);

            bool validPickupPeriod = true;  // stays true when the AM/PM input is valid
            do
            {
                cout << "AM or PM: ";
                string periodInput;
                getline(cin, periodInput);

                if (periodInput == "AM" || periodInput == "am" || periodInput == "Am" || periodInput == "aM" ||
                    periodInput == "A.M." || periodInput == "A.M" || periodInput == "A.m." || periodInput == "A.m" ||
                    periodInput == "a.M." || periodInput == "a.M" || periodInput == "a.m." || periodInput == "a.m")
                {
                    customers[numOfCus].period = AM;

                    validPickupPeriod = true;
                }

                else if (periodInput == "PM" || periodInput == "pm" || periodInput == "Pm" || periodInput == "pM" ||
                        periodInput == "P.M." || periodInput == "P.M" || periodInput == "P.m." || periodInput == "P.m" ||
                        periodInput == "p.M." || periodInput == "p.M" || periodInput == "p.m." || periodInput == "p.m")
                {
                    customers[numOfCus].period = PM;

                    validPickupPeriod = true;
                }

                else
                {
                    cout << "Invalid. Type AM or PM only." << endl;

                    validPickupPeriod = false;
                }
            } while (validPickupPeriod == false);
            
            customers[numOfCus].shippingFee = 0;
            
            validOrderingMethod = true;
        }

        else
        {
            cout<<"Ordering Method Not Available."<<endl;
            
            validOrderingMethod = false;
        }
    } while (validOrderingMethod == false);

    bool validPayMethod = true;     // Stays true when the payment mode is valid
    do
    {
        cout<<"Payment Mode ([1] Cash | [2] Maribank | [3] G-Cash): ";
        int pChoice;
        cin >> pChoice;
        cin.ignore();

        if (pChoice == 1)
        {
            customers[numOfCus].payMet = CASH;

            validPayMethod = true;
        }
        
        else if (pChoice == 2)
        {
            customers[numOfCus].payMet=MARIBANK;

            validPayMethod = true;
        }

        else if (pChoice == 3)
        {
            customers[numOfCus].payMet=GCASH;

            validPayMethod = true;
        }

        else
        {
            cout<<"Payment Method not Available."<<endl;
            
            validPayMethod = false;
        }
    }while(validPayMethod == false);

    bool validBookingType = true;       // Stays true when the booking type is valid
    do
    {
        int bookChoice;
        cout<<"Booking Type ([1] Secured: Pay Now | [2] Shortlisted: Pay Later): ";
        cin >> bookChoice;
        cin.ignore();

        if(bookChoice == 1)
        {
            customers[numOfCus].bookType = SECURED;
            customers[numOfCus].origBookType = SECURED;

            if (takeStock(customers[numOfCus]) == true)
            {
                enqueueSecured(customers[numOfCus]);
                validBookingType = true;
            }
            else
            {
                cout<<"Not enough stock for a Secured order. Pick again or choose Shortlisted."<<endl;
                validBookingType = false;
            }
        }

        else if (bookChoice == 2)
        {
            customers[numOfCus].bookType = SHORTLISTED;
            customers[numOfCus].origBookType = SHORTLISTED;
            enqueueShortlisted(customers[numOfCus]);

            validBookingType = true;
        }

        else
        {
            cout<<"Invalid Booking."<<endl;

            validBookingType = false;
        }
    } while (validBookingType == false);

    cout<<"\n [SUCCESS] Pre-order registered!\n";
    numOfCus++;
}

//OPTION 2
void processNextOrder()
{
    if (isSecuredQueueEmpty() == true)
    {
        while (isShortlistedQueueEmpty() == false)
        {
            CustomerRecord c = shortlistedFront->customer;      // Copy of the front shortlisted customer
            dequeueShortlisted();
 
            if (takeStock(c) == true)
            {
                c.bookType = SECURED;
                enqueueSecured(c);
            }
            else
            {
                cout<<"[NOT FULFILLED] "<<c.fullName<<" (stock ran out)"<<endl;
            }
        }
    }

    if (isSecuredQueueEmpty() == true)
    {
        printEmptySecuredQueue();
    }
    else
    {
        CustomerRecord next = securedFront->customer;
        dequeueSecured();
        insertAtBeginning(next);
    }
}

//OPTION 3
void displayOrders()
{
    displaySecuredQueue();
    displayShortlistedQueue();
    displayList();
}

//OPTION 4
//LINEAR SEARCH (same as the Search in CC102): checks every customer one by one, records can be in any order
void linearSearchOrder(CustomerRecord customers[], int numOfCus)
{
    bool found;             // Stays true when the customer matches the search key
    int result;             // Counts the matching records
    char again;             // Y/y = search again
    string searchKey;

    do
    {
        found = false;      //Reset every search
        result = 0;         //Counts result

        cout<<"\n---LINEAR SEARCH---"<<endl;
        cout<<"Enter Search Key (Name/Address/D/P/C/M/G/CROWN/USDA/HONEY/PIT/XTRA) : ";
        getline(cin, searchKey);

        cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
        cout<<"\n ---------⌕ MATCHING RECORDS---------------------------------------------------------------"<<endl;

        for (int y = 0; y < numOfCus; y++)
        {
            found = false;  // reset overall result

            //Compare search key with: full name; address; order method (D - Delivery/P - Pick-up);
            //payment method (C - Cash/M - Maribank/G - G-Cash); and the menu.
            if (customers[y].fullName.find(searchKey) != string::npos ||
                customers[y].purok.find(searchKey) != string::npos || customers[y].brgy.find(searchKey) != string::npos ||
                customers[y].municipality.find(searchKey) != string::npos || customers[y].prov.find(searchKey) != string::npos ||
                ((searchKey == "D" || searchKey == "d") && customers[y].orderMet == DELIVERY) ||
                ((searchKey == "P" || searchKey == "p") && customers[y].orderMet == PICKUP) ||
                ((searchKey == "C" || searchKey == "c") && customers[y].payMet == CASH) ||
                ((searchKey == "M" || searchKey == "m") && customers[y].payMet == MARIBANK) ||
                ((searchKey == "G" || searchKey == "g") && customers[y].payMet == GCASH))
            {
                found = true;
            }

            // The orders of a customer are in the row customers[y].idx of orders[][]
            for (int x = 0; x < customers[y].numOrders; x++)
            {
                if (((searchKey == "CROWN" || searchKey == "crown") && orders[customers[y].idx][x].item == CROWN) ||
                    ((searchKey == "USDA" || searchKey == "usda") && orders[customers[y].idx][x].item == USDA) ||
                    ((searchKey == "HONEY" || searchKey == "honey") && orders[customers[y].idx][x].item == HONEY) ||
                    ((searchKey == "PIT" || searchKey == "pit") && orders[customers[y].idx][x].item == PIT) ||
                    ((searchKey == "XTRA" || searchKey == "xtra") && orders[customers[y].idx][x].item == XTRA))
                {
                    found = true;
                }
            }

            if (found)
            {
                result++;

                cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
                cout<<"˖⌕ ۫ . . . Result "<<result<<endl;
                cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
                cout<<"👤 Customer "<<y + 1<<"⋆˚࿔"<<endl;
                printOrders(customers[y], orders);
            }
        }

        if (result == 0)    //If not found
        {
            cout<<"No Record Found\n";
        }

        cout<<"\nDo you want to search again?(Y/N): ";
        cin>>again;
        cin.ignore();

    } while (again == 'Y' || again == 'y');     //While the answer is Y/y the user can search again and again
}

//BINARY SEARCH (records must be sorted alphabetically)
void binarySearchOrder(CustomerRecord customers[], int numOfCus)
{
    string key;
    cout<<"\n---BINARY SEARCH (Requires Sorted Records) ---"<< endl;
    cout<< "Enter Customer Full Name to search: ";
    getline(cin, key);

    int low = 0, high = numOfCus - 1;
    bool found = false;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (customers[mid].fullName == key)
        {
            found = true;
            cout<<"[FOUND via Binary Search ] Name: " << customers[mid].fullName
            << "| Contact: " << customers[mid].contactNo
            << "| Total: " << fixed << setprecision(2) << "₱" << customers[mid].total << endl;
            break;
        }
        else if (customers[mid].fullName < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }

    }

    if (!found)
    {
        cout<<"Record not found using Binary Search (ensure records are sorted alphabetically)"<<endl;
    }

}

void searchOrder(CustomerRecord customers[], int numOfCus)
{
    if (numOfCus == 0)
    {
        cout<<"\nNo customer records found to search.\n";
        return;
    }

    string searchChoice;
    cout<<"\n--- SEARCH ORDER ---"<<endl;
    cout<<"[1] Linear Search            (any search key, records can be in any order)"<<endl;
    cout<<"[2] Binary Search by Name    (records must be sorted by name)"<<endl;
    cout<<"Choice: ";
    getline(cin, searchChoice);

    if (searchChoice == "1")
    {
        linearSearchOrder(customers, numOfCus);
    }
    else if (searchChoice == "2")
    {
        binarySearchOrder(customers, numOfCus);
    }
    else
    {
        cout<<"Invalid choice."<<endl;
    }
}

//OPTION 5
void bubbleSortOrder(CustomerRecord customers[], int numOfCus)
{
    if (numOfCus < 2)
    {
        cout<<"\nNot enough records to sort.\n";
        return;
    }

    int sortChoice;
    do
    {
        cout << "\nSort by [1] Time [2] Name: ";
        cin >> sortChoice;
        if (sortChoice != 1 && sortChoice != 2)
        {
            cout << "Invalid choice. Select 1 for Time or 2 for Name." << endl;
        }
    } while (sortChoice != 1 && sortChoice != 2);
    cin.ignore();

    cout << "\nSorting customer records by "
         << (sortChoice == 1 ? "Time" : "Full Name")
         << " using Bubble Sort\n";
    for (int i = 0; i < numOfCus - 1; i++)
    {
        for (int j = 0; j < numOfCus - i - 1; j++)
        {
            bool shouldSwap;
            if (sortChoice == 1)
            {
                shouldSwap = customers[j].period > customers[j + 1].period ||
                    (customers[j].period == customers[j + 1].period &&
                     customers[j].timeSlot > customers[j + 1].timeSlot);
            }
            else
            {
                shouldSwap = customers[j].fullName > customers[j + 1].fullName;
            }

            if (shouldSwap)
            {
                CustomerRecord temp = customers[j];
                customers[j] = customers[j+1];
                customers[j+1] = temp;
            }
            
        }
        
    }
    cout<<"[SUCCESS] Orders sorted successfully via Bubble Sort!\n";

    cout<<"\n---SORTED REPORT---\n";
    for (int i = 0; i < numOfCus; i++)
    {
        cout<<"[" << i + 1 << "] Name: " << customers[i].fullName
        << "| Time: " << customers[i].timeSlot
        << "| Total: "<< fixed << setprecision(2) << customers[i].total << endl;
    }
}

//OPTION 6
void removeConfirmedOrder()
{
    if (isListEmpty())
    {
        printEmptyList();
        return;
    }

    displayList();

    int position;
    cout<<"Enter position of the order to return to the queue (1 - "<<countList()<<"): ";
    cin>>position;
    cin.ignore();

    CustomerRecord removed;     // Holds the customer taken out of the list

    if (removeAtPosition(position, removed) == false)
    {
        cout<<"Invalid position."<<endl;
        return;
    }

    cout<<"\n[REMOVED from Confirmed List]: "<<removed.fullName<<endl;

    // Return the customer to the REAR of the queue he/she originally came from
    if (removed.origBookType == SECURED)
    {
        // Already paid, so the stock stays reserved
        enqueueSecured(removed);
    }
    else
    {
        // Not paid, so the stock taken when he/she was promoted is given back
        returnStock(removed);
        removed.bookType = SHORTLISTED;
        enqueueShortlisted(removed);
    }
}


//OPTION 7 = peekStack()

int main()
{
    int numOfCus = 0, option;

    cout<<"╔═══════════════════════════════════════════════════════════════════════════════════════════╗"<<endl;
    cout<<"                                       ♨️ VICEROY ♨️                                       "<<endl;
    cout<<"                                  LOW AND SLOW SINCE 2019                                  "<<endl;
    cout<<"                     FB: Viceroy Smoked BBQ | Contact No.: 09279855649                     "<<endl;
    cout<<"                                 Quezon City, Philippines                                  "<<endl;
    cout<<"╚═══════════════════════════════════════════════════════════════════════════════════════════╝"<<endl<<endl;

    cout<<"                                         WELCOME!                                          "<<endl;

    do{
        //Program or System Menu
        //1. Register Pre-order (Queue)             5. Sort Orders (Bubble or Merge)
        //2. Process Next Order (Dequeu, Push)      6. Remove Confirmed Order (Remove, Queue)
        //3. Display Orders (Peek Queue)            7. View Last Confirmed Order (Peek Stack)
        //4. Search Order (Linear or Binary)        8. EXIT
        cout<<"─────────────────────────────────────── VICEROY|MENU ──────────────────────────────────────"<<endl;
        cout<<"                 [1] Register Pre-order           [5] Sort Orders                          "<<endl;
        cout<<"                 [2] Process Next Order           [6] Remove Confirmed Order               "<<endl;
        cout<<"                 [3] Display Orders               [7] View Last Confirmed Order            "<<endl;
        cout<<"                 [4] Search Order                 [8] EXIT                                 "<<endl;
        cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;

        cout<<"Pitmaster, select an option (1 - 8): ";
        cin>>option;
        cin.ignore();

        switch (option)
        {
            case 1:
            {
                registerPreOrder(customers, orders, numOfCus);
                break;
            }
            case 2:
            {
                processNextOrder();
                break;
            }
            case 3:
            {
                displayOrders();
                break;
            }
            case 4:
            {
                searchOrder(customers, numOfCus);
                break;
            }
            case 5:
            {
                bubbleSortOrder(customers, numOfCus);
                break;
            }
            case 6:
            {
                removeConfirmedOrder();
                break;
            }
            case 7:
            {
                peekList();
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
                cout<<"Invalid option, pitmaster."<<endl;
                cout<<"Pick again."<<endl;
            }
        }

    }while (option != 8);
    
    return EXIT_SUCCESS;

}