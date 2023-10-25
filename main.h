#ifndef RESTAURANT_H
#define RESTAURANT_H
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;
ofstream solution;
int MAXSIZE;
int COUNTDELETE = 0;
class Restaurant {

public:
    Restaurant() {};
    ~Restaurant() {};

    //! n = MAXSIZE
    //^ Độ phức tạp O(n)
    virtual void RED(string name, int energy) = 0;
    //^ Độ phức tạp O(n)
    //^ Nếu hàng chờ không có người thì O(1)
    //^ Nếu hàng chờ có người thì O(n), number số lượng cần xóa
    virtual void BLUE(int num) = 0;
    //^ Độ phức tạp shell sort O(n*log(n))
    virtual void PURPLE() = 0;
    //^ Độ phức tạp O(n)
    virtual void REVERSAL() = 0;
    //^ Độ phức tạp O(n)
    virtual void UNLIMITED_VOID() = 0;
    //^ Độ phức tạp O(n)
    virtual void DOMAIN_EXPANSION() = 0;
    //^ Độ phức tạp O(n)
    virtual void LIGHT(int num) = 0;


public:
    class customer {
    public:
        string name;
        int energy;
        customer* prev;
        customer* next;
        friend class Restaurant;
    public:
        customer(){}
        customer(string na, int e, customer* p, customer *ne): name(na), energy(e), prev(p), next(ne){}
        ~customer(){
            // delete prev;
            // delete next;
        }
        void print() {
            cout << name << "-" << energy << endl;
        }
    };
};
#endif