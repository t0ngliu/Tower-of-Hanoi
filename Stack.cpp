#ifndef STACK_CPP
#define STACK_CPP

#include "Stack.hpp"

Stack::Stack() {
    head = NULL;
    top = -1;
}

Stack::~Stack() {
    emptyList();
}

//pushes a new number on the stack, it becomes the head of the linked list
void Stack::push(int in) {
    top++;
    NODE *node = new NODE;
    node->num = in;
    node->next = head;
    head = node;
}

//returns the number on the top of the stack, then delete it and release memory
int Stack::pop() {
    if (head) {
        top--;
        NODE *popped = head;
        int value = head->num;
        head = head->next;        //the next number becomes the new head
        delete popped;
        return value;
    }
    return 0;
}

//get the value of the (num+1)th position in the stack
int Stack::get(int num) {
    NODE *node = head;
    //if not enough numbers 
    if (num > top) return 0;
    for (int i = 0; i < num; i++) {   
        node = node->next;
    }
    return node->num;
}

//returns the number of items in the stack
int Stack::getSize() {
    return top + 1;
}

//clear the stack
void Stack::emptyList() {
    while (head) {
        pop();
    }
    top = -1;
}

#endif
