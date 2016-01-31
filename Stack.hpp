#ifndef STACK_HPP
#define STACK_HPP

struct NODE{
    int num;
    NODE *next;
};

class Stack {
    private:
        int top;    //keeps track of how many items are in the stack
        NODE *head;
    
    public:
        Stack();
        ~Stack();
        
        void push(int in);  //pushes a new number on the stack, it becomes the head of the linked list
        int pop();  //returns the number on the top of the stack, then delete it and release memory
        int get(int num); //get the value of the (num+1)th position in the stack
        int getSize();  //returns the number of items in the stack
        void emptyList();  //clear the stack
};
    
#endif
