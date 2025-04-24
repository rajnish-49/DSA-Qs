/*Use one array to store elements for both stacks.
One stack grows from left to right (let's call it Stack 1).
The other stack grows from right to left (let's call it Stack 2).
*/

#include<iostream>

using namespace std;

class TwoStack {
    //properties
    public:
        int *arr;
        int top1;
        int top2;
        int size;

    // behaviour
    TwoStack(int size) {
        this -> size = size;
        top1 = -1;
        top2 = size; // top2 is at last index of array
        arr = new int[size];
    }

    //push in stack 1 
    void push( int num) {
        // AT LEAST ONE EMPTY SPACE
        if(top2- top1 > 1) {
            top1++;
            arr[top1] = num;
        }
        else{
            cout << "Stack OverFlow" << endl;
        }
    }

    //push in stack 2
    void push2(int num) {
        if(top2 - top1 > 1) {
            top2--;
            arr[top2] = num;
        }
        else{
            cout << "Stack OverFlow" << endl;
        }
    }

    //pop from stack 1 and return popped element
    int pop1() {
        if(top1 >= 0) {
            int ans = arr[top1];
            top1--;
            return ans;
        }
        else 
        {
            return -1; //stack underflow
        }
    }

    //pop from stack 2  and return popped element
    int pop2() {
        if(top2 < size) {
            int ans = arr[top1];
            top2++;
            return ans;
        }
        else 
        {
            return -1; //stack underflow
        }
    }
};

int main() {

    return 0;
}