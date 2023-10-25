#include "main.h"

class imp_res : public Restaurant
{
public:
    class OrderListCustomer {
    public:
        OrderListCustomer* next;
        OrderListCustomer* prev;
        customer* addressCus;
        bool in_table;
    public:
        OrderListCustomer() : next(nullptr), prev(nullptr), addressCus(nullptr), in_table(false) {};
        explicit OrderListCustomer(customer *cus) : next(nullptr), prev(nullptr), addressCus(cus), in_table(false) {};
        OrderListCustomer(customer *cus, OrderListCustomer *prev_cus) : next(nullptr), prev(prev_cus), addressCus(cus), in_table(false){};
        void deleteOrderCustomer(OrderListCustomer*& ord_cus, int size) {
            customer *cus = ord_cus->addressCus;
            customer *prev_cus = cus->prev;
            customer *next_cus = cus->next;
            if (size < 2) {
                delete ord_cus; ord_cus->addressCus = nullptr; ord_cus = nullptr;
                return;
            }
            //! ISOLATE CUSTOMER FROM THE CIRCLE
            if (prev_cus) prev_cus->next = next_cus;
            if (next_cus) next_cus->prev = prev_cus;
            //! DELETE AND UPDATE NULL VALUE
            delete ord_cus; ord_cus->addressCus = nullptr; ord_cus = nullptr;

        }
        ~OrderListCustomer() { delete addressCus; addressCus = nullptr; }

    };
private:
    //! IDEA:
        //! use a doubly linked list to manage the list customer come to restaurant => always update list when a new customer come
        //! this dl_list is managed by 4 pointers (1 head, 1 tail, 1 head of queue, 1 tail of queue)
    //! D_LIST: [headOrder]<->[]<->[]<->[]<->[]<->[tailOrder]
    //! ORDERED CUSTOMER COME TO RESTAURANT (@note IN BOTH DESK AND QUEUE)
    OrderListCustomer *headOrder;
    OrderListCustomer *tailOrder;
    //! ORDERED CUSTOMER IN QUEUE
    OrderListCustomer *headOrderQueue;
    OrderListCustomer *tailOrderQueue;
    //! CUSTOMER IN DESK (CIRCULAR LINKED LIST)
    customer * customerX;
    int sizeCustomerInDesk;
    //! CUSTOMER IN QUEUE
    customer * customerQueueHead;
    customer * customerQueueTail;
    int sizeCustomerQueue;
public:
    imp_res() { //! initial empty value
        customerX = customerQueueHead = customerQueueTail = nullptr;
        headOrder = tailOrder = headOrderQueue = tailOrderQueue = nullptr;
        sizeCustomerInDesk = sizeCustomerQueue = 0;
    };
    ~imp_res() {
        while(headOrder) {
            OrderListCustomer *temp = headOrder; headOrder = headOrder->next;
            temp->deleteOrderCustomer(temp, sizeCustomerInDesk);
        }
    }
    void traverse(void(*op)) {} ;
    void push_queue(customer *cus) {
        if (!customerQueueHead) {
            customerQueueHead = customerQueueTail = cus;
            headOrderQueue = tailOrder;
        }
        else {
            customerQueueTail->next = cus;
            cus->prev = customerQueueTail;
            customerQueueTail = customerQueueTail->next;
        }
        sizeCustomerQueue++;
    }
    bool is_namesake(const string &name) {
        if (!customerX) return false; //! Restaurant is empty
        customer *temp = customerX;
        int i = sizeCustomerInDesk;
        while (i--) { //! Check namesake in desk
            if (temp->name == name) return true;    temp = temp->next;
        }
        temp = customerQueueHead;
        while (temp) { //! Check namesake in queue
            if (temp->name == name) return true;    temp = temp->next;
        }
        return false;
    }
    void add_clockwise(customer *base, customer* cus) {
        //! NOTE: this function should be used when there are more than 2 customer in desk
        cus->prev = base; cus->next = base->next;
        base->next->prev = cus; base->next = cus; customerX = cus;
    }
    void add_anticlockwise(customer *base, customer *cus) {
        //! NOTE: this function should be used when there are more than 2 customer in desk
        cus->prev = base->prev; cus->next = base;
        base->prev->next = cus; base->prev = cus; customerX = cus;
    }
    void add_circle(customer *cus) {
            //! CASE 1: RESTAURANT IS EMPTY
        if (!sizeCustomerInDesk) customerX = cus;
            //! CASE 2: JUST ONE CUSTOMER
        else if (sizeCustomerInDesk == 1) {
            cus->next = cus->prev = customerX;
            customerX->next = customerX->prev = cus; customerX = cus;
        }
            //! CASE 3: NEED TO ORGANIZE SEATS
        else if (sizeCustomerInDesk >= MAXSIZE/2) { //! ORGANIZE SEATS
            int RES = -1; customer *RES_CUSTOMER = customerX;
            customer *temp_cus = customerX; //! reset loop_variable for another loop
            for (int i = 0; i < sizeCustomerInDesk; i++) { //! FIND RES_CUSTOMER
                int val = abs(cus->energy - temp_cus->energy);
                RES_CUSTOMER = val > RES ? temp_cus : RES_CUSTOMER; RES = val > RES ? val : RES;
                temp_cus = temp_cus->next;
            }
            if (cus->energy - RES_CUSTOMER->energy < 0) add_anticlockwise(RES_CUSTOMER, cus);
            else add_clockwise(RES_CUSTOMER, cus);
        }
            //! CASE 4: JUST ADD TO RESTAURANT
        else { //! COMPARE BEFORE ADDING
            if (cus->energy >= customerX->energy) add_clockwise(customerX, cus);
            else add_anticlockwise(customerX, cus);
        }
        sizeCustomerInDesk++;
    }
    void RED(string name, int energy)
    {
        //! NO ENERGY || FULL IN QUEUE || NAMESAKE CASE => CHASE GUEST BACK
        if (!energy || sizeCustomerQueue == MAXSIZE || is_namesake(name)) return;
        //! UPDATE ORDER_LIST_CUSTOMER COME
        customer *cus = new customer(name, energy, nullptr, nullptr);
        if (!headOrder) {
            headOrder = new OrderListCustomer(cus);
            tailOrder = headOrder;
        }
        else {
            tailOrder->next = new OrderListCustomer(cus, tailOrder);
            tailOrder = tailOrder->next;
        }
        tailOrder->in_table = true;
        //! ADD TO QUEUE
        if (sizeCustomerInDesk == MAXSIZE) { push_queue(cus);   return; }
        //! ADD TO RESTAURANT
        else add_circle(cus);
    }
    void BLUE(int num)
    {
        cout << "blue "<< num << endl;
    }
    void PURPLE()
    {
        cout << "purple"<< endl;
    }
    void REVERSAL()
    {
        cout << "reversal" << endl;
    }
    void UNLIMITED_VOID()
    {
        cout << "unlimited_void" << endl;
    }
    void DOMAIN_EXPANSION()
    {
        cout << "domain_expansion" << endl;
    }
    void LIGHT(int num) {
        if (!num) {
            if(!sizeCustomerQueue) return;
            customer *temp = customerQueueHead;
            for (int i = 0; i < sizeCustomerQueue; i++) { temp->print(); temp = temp->next;}
        }
        else if (num > 0) {
            if (!sizeCustomerInDesk) return;
            customer *temp = customerX;
            for (int i = 0; i < sizeCustomerInDesk; i++) { temp->print(); temp = temp->next; }
        }
        else {
            if (!sizeCustomerInDesk) return;
            customer *temp = customerX;
            for (int i = 0; i < sizeCustomerInDesk; i++) { temp->print(); temp = temp->prev; }
        }
    }
};