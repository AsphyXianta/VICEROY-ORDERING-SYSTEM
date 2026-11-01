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
    cout<<"No\t\t\tProduct\t\t\t\t\tPortion\tPrice\t Stock"<<endl;
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

bool takeStock(CustomerRecord c)
{
    for (int k = 0; k < c.numOrders; k++)
    {
        if (orders[c.idx][k].quantity > stock[orders[c.idx][k].item])
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

void printOrders(CustomerRecord c, Order orders[100][100])
{
    double subTb4Sf;
    double itemTot;

    cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
    cout<<"🏷️ Name : "<<c.fullName<<endl;
    cout<<"📍 Address: Purok "<<c.purok<<", #"<<c.unitFloorHouseNo<<", Barangay "<<c.brgy<<", "<<c.municipality<<", "<<c.prov<<endl;
    cout<<"📞 Contact: "<<c.contactNo<<endl;

    cout<<"────────────────────────────────────── ∘₊✧ ORDER ✧₊ ───────────────────────────────────────\n";
    cout<<"\tNo\t\t\t\t\tItem\t\tQuantity\tPrice\tPer Item Total\n";
    cout<<"────────────────────────────────────────────────────────────────────────────────────────────"<<endl;

    // Loop through each order of a customer
    for(int x = 0; x < c.numOrders; x++)
    {
        cout<<"\t"<<x + 1<<"\t\t\t";

        // Display item name based on enum value
        if (orders[c.idx][x].item == CROWN)
        {
            cout<<"THE CROWN JEWELS Smoked Pork Belly\t\t";
        }

        else if (orders[c.idx][x].item == USDA)
        {
            cout<<"USDA Smoked Beef Shortplate       \t\t";
        }

        else if (orders[c.idx][x].item == HONEY)
        {
            cout<<"Artisan Honey Cured Smoked Bacon  \t\t";
        }

        else if (orders[c.idx][x].item == PIT)
        {
            cout<<"The Pitmaster's Sampler           \t\t";
        }

        else
        {
            cout<<"Extra Signature BBQ Sauce         \t\t";
        }

        cout<<orders[c.idx][x].quantity<<"\t";
        cout<<"₱"<<fixed<<setprecision(2)<<orders[c.idx][x].price<<" \t";

        //Used displaying each item total by multiplying quantity and price
        itemTot = orders[c.idx][x].quantity * orders[c.idx][x].price;
        cout << fixed << setprecision(2) << " ₱" << itemTot << endl;
       
    }
    cout<<"──────────────────────────────────────── ˗ˏˋ⛁$ˎˊ˗ ─────────────────────────────────────────"<<endl;
    //Subtotal after adding the total per item and before shipping fee
    subTb4Sf = c.total - c.shippingFee;
    cout<<"💲 Subtotal :                                                                    ₱"<<fixed<<setprecision(2)<<subTb4Sf<<endl;
    cout<<"🚚 Shipping Fee:                                                                 ₱"<<fixed<<setprecision(2)<<c.shippingFee<<endl;
    cout<<"💸 Total:                                                                        ₱"<<fixed<<setprecision(2)<<c.total<<endl;
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

void printEmptyStack()
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

//STACK
struct StackNode {
    CustomerRecord customer;    // The customer data stored in this node
    StackNode* next;            // Points to the node below this one in the stack (nullptr if last)
};

StackNode* top = nullptr;       // Top of the Confirmed-orders stack (nullptr = empty)

bool isStackEmpty() 
{
 return top == nullptr;
}

void push(CustomerRecord c) 
{
    StackNode* newStackNode = new StackNode;    // New node that will hold the customer
    newStackNode->customer = c;
    newStackNode->next = top; 
    top = newStackNode;

    cout<<"\nConfirmed Order: "<<newStackNode->customer.fullName<<endl;
    cout<<"                                      ---Order Details---"<<endl;
    printOrders(newStackNode->customer, orders);
}

void pop()
{
    if(isStackEmpty() == true)
    {
        cout<<"                            ---No Confirmed Order to Undo---"<<endl;
        printEmptyStack();
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
        printEmptyStack();
    }
    else
    {
        cout<<"Most Recently Confirmed Order: "<<top->customer.fullName<<endl;
        cout<<"                                      ---Order Details---"<<endl;
        printOrders(top->customer, orders);
    }
}

void displayStack()
{
    cout<<"Confirmed Orders: "<<endl;
    if(isStackEmpty() == true)
    {
        printEmptyStack();
    }
    else
    {
        StackNode* current = top;           // Walker pointer that moves through the stack
        int confirmedCustomerCount = 0;     // Counts customers in the stack

        while(current != nullptr)
        {
            confirmedCustomerCount++;
            current = current->next;
        }

        cout<<"═══════════════════════════════════════════════════════════════════════════════════════════"<<endl;
        cout<<"                              STACK STATUS: "<<confirmedCustomerCount<<" Confirmed Customer"<<endl;
        cout<<"-------------------------------------------------------------------------------------------"<<endl;

        current = top;
        int countingStack = 1;          // Numbering shown beside each confirmed customer

        while(current != nullptr)
        {
            cout<<"\n["<<countingStack<<"] "<<current->customer.fullName<<endl;
            printOrders(current->customer, orders);

            current = current->next;
            countingStack++;
        }
        cout<<endl;
    }
}
//-------------------------------------------------

//OPTION 1
void registerPreOrder(CustomerRecord customers[], Order orders[][100], int &numOfCus) 
{ 
    cout << "\n--- REGISTER PRE-ORDER ---\n";
    
    cin.ignore(); 
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
    

    cout << "How Many orders?: ";
    cin >> customers[numOfCus].numOrders;
    cin.ignore();

    customers[numOfCus].total = 0;
    customers[numOfCus].idx = numOfCus;  
    
    displayMenu();

    for (int x = 0; x < customers[numOfCus].numOrders; x++) 
    {
        bool validMenu = true;      // Stays true when the chosen menu number is valid
        do
        {
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
        } while (validMenu == false);

        cout << "Quantity: ";
        cin >> orders[numOfCus][x].quantity;
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

            customers[numOfCus].total += customers[numOfCus].shippingFee;

            validOrderingMethod = true;
        } 
        
        else if (method == 2){
            customers[numOfCus].orderMet = PICKUP;
            cout << "Pick-up Time (HH:MM): ";
            getline(cin, customers[numOfCus].timeSlot);
            
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
        push(next);
    }
}

//OPTION 3
void displayOrders()
{
    displaySecuredQueue();
    displayShortlistedQueue();
    displayStack();
}

//OPTION 4
void binarySearchOrder(CustomerRecord customers[], int numOfCus)
{
    if (numOfCus == 0)
    {
        cout<<"\nNo customer records found to search.\n";
        return;
    }

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
            << "| Total: ₱" << fixed << setprecision(2) << customers[mid].total << endl;
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

//OPTION 5
void bubbleSortOrder(CustomerRecord customers[], int numOfCus)
{
    if (numOfCus < 2)
    {
        cout<<"\nNot enough records to sort.\n";
        return;
    }
    
    cout<<"\nSorting customer records alphabetically by Full Name using Bubble Sort\n";
    for (int i = 0; i < numOfCus - 1; i++)
    {
        for (int j = 0; j < numOfCus - i - 1; j++)
        {
            if (customers[j].fullName > customers[j+1].fullName)
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
void undoLastOrder()
{
    pop();
    // TODO: ibalik ang customer sa queue (pag napagdesisyunan na)
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
        //2. Process Next Order (Dequeu, Push)      6. Undo Last Confirmed Order (Pop, Queue)
        //3. Display Orders (Peek Queue)            7. View Last Confirmed Order (Peek Stack)
        //4. Search Order (Linear or Binary)        8. EXIT
        cout<<"─────────────────────────────────────── VICEROY|MENU ──────────────────────────────────────"<<endl;
        cout<<"                 [1] Register Pre-order           [5] Sort Orders                          "<<endl;
        cout<<"                 [2] Process Next Order           [6] Undo Last Confirmed Order            "<<endl;
        cout<<"                 [3] Display Orders               [7] View Last Confirmed Order            "<<endl;
        cout<<"                 [4] Search Order                 [8] EXIT                                 "<<endl;
        cout<<"───────────────────────────────────────────────────────────────────────────────────────────"<<endl;

        cout<<"Pitmaster, select an option (1 - 8): ";
        cin>>option;

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
                cin.ignore();
                binarySearchOrder(customers, numOfCus);
                break;
            }
            case 5:
            {
                bubbleSortOrder(customers, numOfCus);
                break;
            }
            case 6:
            {
                undoLastOrder();
                break;
            }
            case 7:
            {
                peekStack();
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