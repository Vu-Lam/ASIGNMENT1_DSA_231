#include "main.h"

class imp_res : public Restaurant
{
private:
    customer * customerX; //! lưu trữ danh sách khách hàng đang ở trong bàn
    int sizeCustomerInDesk;
    customer * customerQueue; //! mô tả danh sách khác hàng đang trong hàng đợi chỉ sử dung next
    int sizeCustomerQueue;
public:
    imp_res() { customerX = customerQueue = nullptr; sizeCustomerInDesk = sizeCustomerQueue = 0;};
    void traverse(void(*op)) {} ;
    void push_queue(const string& name, int energy) {
        if (sizeCustomerQueue == 0) customerQueue = new customer(name, energy, nullptr, nullptr);
        else if (sizeCustomerQueue == 1) { customer *cus = new customer(name, energy, customerQueue, customerQueue);
            customerQueue->next = customerQueue->prev = cus; }
        else {
            customer *previous = customerQueue->prev;
            customer *cus = new customer(name, energy, previous, customerQueue);
            customerQueue->prev = cus; previous->next = cus;
        }
        sizeCustomerQueue++;
    }
    void add_clockwise(customer *base, const string &name, int energy) { //! Create a customer_node and add it next to the base_node in clockwise
        //! NOTE: this function should be used when there are more than 2 customer in desk
        customer *cus = new customer(name, energy, base, base->next);
        base->next->prev = cus; base->next = cus; this->customerX = cus;
    }
    void add_anticlockwise(customer *base, const string &name, int energy) { //! Create a customer_node and add it next to the base_node in anticlockwise
        //! NOTE: this function should be used when there are more than 2 customer in desk
        customer *cus = new customer(name, energy, base->prev, base);
        base->prev->next = cus; base->prev = cus; this->customerX = cus;
    }
    void RED(string name, int energy)
    {
        //! CHASE GUEST BACK
        if (!energy || sizeCustomerQueue == MAXSIZE) return;
        //! CHECK NAMESAKE CASE
        customer *temp_cus = customerX;
        for (int i = 0; i < sizeCustomerInDesk; i++) { if (temp_cus->name == name) return; temp_cus = temp_cus->next; }
        temp_cus = customerQueue;
        for (int i = 0; i < sizeCustomerQueue; i++) { if (temp_cus->name == name) return; temp_cus = temp_cus->next; }
        //! ADD TO QUEUE
        if (sizeCustomerInDesk == MAXSIZE) { push_queue(name, energy); return; }
        //! ADD TO RESTAURANT
            //! CASE 1: RESTAURANT IS EMPTY
        if (!sizeCustomerInDesk) customerX = new customer(name, energy, nullptr, nullptr);
            //! CASE 2: JUST ONE CUSTOMER
        else if (sizeCustomerInDesk == 1) { customer *cus = new customer(name, energy, customerX, customerX);
                                            customerX->next = customerX->prev = cus; customerX = cus;}
            //! CASE 3: NEED TO ORGANIZE SEATS
        else if (sizeCustomerInDesk >= MAXSIZE/2) { //! ORGANIZE SEATS
            int MAX_INDEX = 0, RES = -1; customer *RES_CUSTOMER = customerX;
            temp_cus = customerX; //! reset loop_variable for another loop
            for (int i = 0; i < sizeCustomerInDesk; i++) {
                int val = abs(energy - temp_cus->energy);
                RES_CUSTOMER = val > RES ? temp_cus : RES_CUSTOMER; RES = val > RES ? val : RES;
                temp_cus = temp_cus->next;
            }
            if (energy - RES_CUSTOMER->energy < 0) add_anticlockwise(RES_CUSTOMER, name, energy);
            else add_clockwise(RES_CUSTOMER, name, energy);
        }
            //! CASE 4: JUST ADD TO RESTAURANT
        else { //! COMPARE BEFORE ADDING
            if (energy >= customerX->energy) add_clockwise(customerX, name, energy);
            else add_anticlockwise(customerX, name, energy);
        }
        sizeCustomerInDesk++;
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
            customer *temp = customerQueue;
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
    ~imp_res() {
        while(sizeCustomerInDesk) { customer *del = customerX; customerX = customerX->next; del->next = del->prev = nullptr; delete del; sizeCustomerInDesk--;}
        while(sizeCustomerQueue) { customer *del = customerQueue; customerQueue = customerQueue->next; del->next = del->prev = nullptr; delete del; sizeCustomerQueue--;}
    }
};