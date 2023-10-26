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
        //! ADD TO QUEUE
        if (sizeCustomerInDesk == MAXSIZE) { push_queue(cus);   return; }
        //! ADD TO RESTAURANT
        else {
            add_circle(cus);
            tailOrder->in_table = true;
        }
    }
    //! UPDATE & DELETE NODE DL_LIST
    void update_delete_ordered_cus(OrderListCustomer *temp) {
        //! UPDATE POINTER
        if (sizeCustomerInDesk + sizeCustomerQueue == 1) { //! THERE IS JUST ONE NODE
            headOrder = headOrderQueue = tailOrder  = nullptr;
        }
        else if (temp == headOrder) { //! temp is head of list
            //! UPDATE headOrderQueue until ordered customer are not in table
            if (temp == headOrderQueue) {
                headOrderQueue = headOrderQueue->next;
                while (headOrderQueue->in_table && headOrderQueue) headOrderQueue = headOrderQueue->next;
            }
            headOrder = headOrder->next;
            if (headOrder) headOrder->prev = nullptr;
        }
        else if (temp == tailOrder) { //! temp is tail of list
            if (temp == headOrderQueue) headOrderQueue = nullptr;
            tailOrder = tailOrder->prev;
            if (tailOrder) tailOrder->next = nullptr;
        }
        else { //! temp is somewhere else
            if (temp == headOrderQueue) { //! UPDATE headOrderQueue until ordered customer are not in table
                headOrderQueue = headOrderQueue->next;
                while (headOrderQueue->in_table && headOrderQueue) headOrderQueue = headOrderQueue->next;
            }
            OrderListCustomer *temp_prev = temp->prev;
            temp_prev->next = temp->next;
            temp->next->prev = temp_prev;
        }
        //! DELETE NODE
            //! if temp is node in restaurant
        if (temp->in_table) temp->deleteOrderCustomer(temp, sizeCustomerInDesk--);
        else { //! if temp is node in queue
            customer *temp_cus = temp->addressCus;
            if (sizeCustomerQueue < 2) customerQueueHead = customerQueueTail = nullptr;
            else if (temp_cus == customerQueueHead) customerQueueHead = customerQueueHead->next;
            else if (temp_cus == customerQueueTail) customerQueueTail = customerQueueTail->prev;
            temp->deleteOrderCustomer(temp, sizeCustomerQueue--);
        }
    }
    //! UPDATE headOrderQueue IN LIST
    void update_h_queue(customer* head){
        OrderListCustomer *temp = headOrderQueue;
        while (temp != nullptr && temp->addressCus != head) temp = temp->next;
        temp->in_table = true;
        if (temp == headOrderQueue) while (headOrderQueue && headOrderQueue->in_table) headOrderQueue = headOrderQueue->next;
    }
    void BLUE(int num) {
        if (num <= 0 || !sizeCustomerInDesk) return;
        for (int i = 0; i < num && sizeCustomerInDesk; i++) {
            OrderListCustomer *temp = headOrder;
            while (temp) {
                if (temp->in_table) {
                    OrderListCustomer *del = temp;
                    temp = temp->next;
                    //! UPDATE CUSTOMER X IN LIST
                    if (del->addressCus->energy > 0) customerX = del->addressCus->next;
                    else customerX = del->addressCus->prev;
                    update_delete_ordered_cus(del);
                    break;
                }
                else temp = temp->next;
            }
        }
        if (!sizeCustomerInDesk) customerX = nullptr;
        //! ADD FROM QUEUE
        while (customerQueueHead && sizeCustomerInDesk < MAXSIZE) {
            customer *temp = customerQueueHead;
            customerQueueHead = customerQueueHead->next;
            if (customerQueueHead) customerQueueHead->prev = nullptr;
            add_circle(temp);
            //! UPDATE headOrderQueue IN LIST
            update_h_queue(temp);
            sizeCustomerQueue--;
        }
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