//
//  main.cpp
//  hw07
//
//  Created by Danny Tan on 11/4/16.
//  Copyright © 2016 NYU. All rights reserved.
//


#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;


//question 1

template <typename Object>
class List
{
private:
    
    struct Node
    {
        Object  data;
        Node   *next;
        
        Node( const Object & d = Object{ },  Node * n = nullptr )
        : data{ d },  next{ n } { }
        
        Node( Object && d, Node * n = nullptr )
        : data{ std::move( d ) }, next{ n } { }
    };
    
public:
    class iterator
    {
    public:
        
        iterator( ): current( nullptr )
        { }
        
        Object & operator* ( )
        { return current->data; }
        
        const Object & operator* ( ) const
        { return  current->data; }
        
        iterator & operator++ ( )
        {
            this->current = this->current->next;
            return *this;
        }
        
        iterator operator++ ( int )
        {
            iterator old = *this;
            ++( *this );
            return old;
        }
        
        bool operator== ( const iterator & rhs ) const
        { return current == rhs.current; }
        
        bool operator!= ( const iterator & rhs ) const
        { return !( *this == rhs ); }
        
    private:
        Node * current;
        
        iterator( Node *p ) : current{ p }
        { }
        
        friend class List<Object>;
    };
    
public:
    List( )
    { header = new Node(); }
    
    List( const List & rhs ) {
        header = new Node(rhs.header->data, nullptr);
        Node* temp = header;
        Node* rhsTemp = rhs.header;
        while (rhsTemp -> next) {
            temp -> next = new Node(rhsTemp -> next -> data, nullptr);
            temp = temp ->next;
            rhsTemp = rhsTemp ->next;
        }
    }
    
    ~List( ) {
        Node* p = header;
        while (p != nullptr) {
            Node* temp = p->next;
            delete p;
            p = temp;
        }
        header = nullptr;
    }
    
    List & operator= ( const List & rhs )
    {
        List copy = rhs;
        std::swap( *this, copy );
        return *this;
    }
    
    List( List && rhs ):header{ new Node }
    {
        header->next = rhs.header->next;
        rhs.header->next = nullptr;
    }
    
    List & operator= ( List && rhs )
    {
        std::swap( header, rhs.header );
        return *this;
    }
    
    iterator begin( ) const
    { return iterator( header->next ); }
    
    iterator end( ) const
    { return iterator( nullptr ); }
    
    iterator before_begin( ) const
    { return iterator( header ); }
    
    bool empty( ) const
    { return header->next == nullptr; }
    
    void clear( )
    {
        while( !empty( ) )
            pop_front( );
    }
    
    void push_front(Object data){
        header->data = data;
        Node* oldHeader = header;
        header = new Node;
        header->next = oldHeader;
    }
    
    void pop_front( )
    { erase_after( before_begin( ) ); }
    
    iterator insert_after( iterator itr, const Object & x )
    {
        Node *p = itr.current;
        p->next = new Node{ x, p->next };
        
        return iterator(p->next);
    }
    
    iterator erase_after( iterator itr )
    {
        Node *p = itr.current;
        Node *oldNode = p->next;
        
        p->next = oldNode->next;
        delete oldNode;
        
        return iterator( p->next );
    }
    
    Object & front( ) {
        return header -> next -> data;
    };
    
    const Object & front( ) const {
        return header -> next -> data;
    };
    
    void merge( List & alist ) {
        Node* mergedCurrent = header;
        Node* lhsCurrent = header->next;
        Node* rhsCurrent = alist.header->next;
        while (lhsCurrent != nullptr && rhsCurrent != nullptr) {
            if (lhsCurrent->data < rhsCurrent ->data) {
                lhsCurrent = lhsCurrent->next;
                mergedCurrent = mergedCurrent ->next;
            } else {
                Node* temp = rhsCurrent->next;
                rhsCurrent ->next = lhsCurrent;
                mergedCurrent ->next = rhsCurrent;
                rhsCurrent = temp;
                mergedCurrent = mergedCurrent->next;
            }
        }
        while (rhsCurrent != nullptr) {
            Node* temp = rhsCurrent->next;
            rhsCurrent->next = nullptr;
            mergedCurrent->next = rhsCurrent;
            rhsCurrent = temp;
        }
        alist.header->next = nullptr;
    };
    
    void remove_adjacent_duplicates( ) {
        Node* current = header;
        while (current->next != nullptr) {
            if (current->data == current->next->data) {
                Node* temp = current->next;
                current->next = temp->next;
                delete temp;
            } else {
                current = current->next;
            }
        }

    };
    
    template<class Predicate>
    void remove_if( Predicate pred ) {
        Node* p = header;
        while (p->next != nullptr) {
            if (pred(p->next->data)) {
                erase_after(iterator(p));
            } else {
                p = p->next;
            }
        }
    }
private:
    Node *header;
};

SCENARIO("Testing Copy Constructor"){
    GIVEN("An instance of the list class"){
        List<int> l;
        l.push_front(3);
        l.push_front(2);
        l.push_front(1);
        WHEN("The copy constructor is invoked"){
            List<int> l2(l);
            THEN("A deep copy should have been made"){
                List<int>::iterator itr1 = l.begin();
                List<int>::iterator itr2 = l2.begin();
                while(itr2 != l2.end() && itr1 != l.end()){
                    REQUIRE(*itr2 == *itr1);
                    ++itr2;
                    ++itr1;
                }
                REQUIRE(itr1 == l.end());
                REQUIRE(itr2 == l2.end());
                l.clear();
                REQUIRE(l.empty());
                REQUIRE(!l2.empty());
            }
        }
    }
}


SCENARIO("Testing the method front()"){
    GIVEN("An instance of the list class with elements inserted"){
        List<int> l;
        l.push_front(3);
        l.push_front(2);
        l.push_front(1);
        WHEN("front() is called"){
            REQUIRE(l.front() == 1);
        }
    }
}



SCENARIO("Testing the method merge()"){
    GIVEN("Two Instances of the list class with elements in relative sorted order for each"){
        List<int> l1;
        l1.push_front(6);
        l1.push_front(4);
        l1.push_front(2);
        List<int> l2;
        l2.push_front(5);
        l2.push_front(3);
        l2.push_front(1);
        WHEN("Merge is called"){
            l1.merge(l2);
            THEN("The elements should be in sorted order"){
                List<int>::iterator lead = l1.begin();
                List<int>::iterator tail = lead;
                ++lead;
                for(; lead!= l1.end(); ++lead){
                    REQUIRE(*tail < *lead);
                }
            }
            THEN("l2 should be empty"){
                REQUIRE(l2.empty());
            }
        }
    }
}

SCENARIO("Testing remove_adjacent_duplicates()"){
    GIVEN("A list with a ton of adjacent duplicates"){
        List<char> l;
        for(int x = 0; x < 5; ++x){
            l.push_front('c');
        }
        for(int x = 0; x < 3; ++x){
            l.push_front('b');
        }
        for(int x = 0; x < 5; ++x){
            l.push_front('c');
        }
        for(int x = 0; x < 2; ++x){
            l.push_front('a');
        }
        WHEN("remove_adjacent_duplicates() is called"){
            l.remove_adjacent_duplicates();
            THEN("The resulting list should be a->c->b->c"){
                List<char>::iterator iter = l.begin();
                REQUIRE(*iter == 'a');
                ++iter;
                REQUIRE(*iter == 'c');
                ++iter;
                REQUIRE(*iter == 'b');
                ++iter;
                REQUIRE(*iter == 'c');
            }
        }
    }
}





// Ignore this pred used for testing
template<class Foo>
class aPred{
public:
    aPred(Foo bar):myFoo(bar){}
    bool operator()(Foo otherBar){
        return myFoo != otherBar;
    }
private:
    Foo myFoo;
};

SCENARIO("testing remove_if"){
    GIVEN("A list with things to remove and a pred"){
        List<string> l;
        l.push_front("Scott");
        l.push_front("Maliat");
        l.push_front("Nico");
        l.push_front("Anam");
        l.push_front("Xinran");
        l.push_front("Lolaly");
        aPred<string> myPred("Nico");
        WHEN("remove_if is used"){
            l.remove_if(myPred);
            THEN("Only Nico should be left."){
                REQUIRE(l.front() == "Nico");
            }
        }
    }
}

//question 2


template<class Object>
class Node{
public:
    Object data;
    Node* next;
    Node( const Object & d = Object{}, Node* n = nullptr):data{d},next{n}{}
    Node(Object&& d, Node* n = nullptr): data{std::move(d)}, next{n}{}
};

template<class Object>
class Queue{
public:
    // vv IMPLEMENT THESE vv
    Object & front() {
        return head -> data;
    }
    const Object & front() const {
        return head -> data;
    }
    Object & back() {
        return tail -> data;
    }
    const Object & back() const {
        return tail -> data;
    }
    void enqueue(Object obj) {
        Node<Object>* objectPtr = new Node<Object>(obj, nullptr);
        if (head == nullptr) {
            head = tail = objectPtr;
            return;
        }
        tail->next = objectPtr;
        tail = objectPtr;
    }
    void dequeue() {
        if (head == nullptr) {
            return;
        } else if (head == tail) {
            head = tail = nullptr;
        } else {
            Node<Object>* temp = head;
            head = head->next;
            delete temp;
        }

        
    }
    // ^^ IMPLEMENT THESE ^^
    
    //Note this is NOT in the implementation for queue, but I am putting it here for easier testing.
    Node<Object>*& getHead() {
        return head;
    }
    
    //Note this is NOT in the implementation for queue, but I am putting it here for easier testing.
    Node<Object>*& getTail() {
        return tail;
    }
private:
    Node<Object>* head;
    Node<Object>* tail;
};


SCENARIO("Testing Queue"){
    GIVEN("A Queue with elements in it"){
        Queue<int> q;
        Node<int>* start = q.getHead();
        start = new Node<int>(1);
        start->next = new Node<int>(2);
        start->next->next = new Node<int>(3);
        q.getHead() = start;
        q.getTail() = start->next->next;
        WHEN("front() is called"){
            THEN("1 should be at the front"){
                REQUIRE(q.front() == 1);
            }
        }
        WHEN("back() is called"){
            THEN("3 should be at the back"){
                REQUIRE(q.back() == 3);
            }
        }
        WHEN("enqueue is called"){
            int value = 4;
            q.enqueue(value);
            THEN("There should be a new node in the back with the value"){
                Node<int>* curr = q.getHead();
                while(curr->next != nullptr){
                    curr = curr->next;
                }
                REQUIRE(curr->data == value);
            }
        }
        WHEN("dequeue is called"){
            q.dequeue();
            THEN("Then the first element should be changed"){
                REQUIRE(q.getHead()->data != 1) ;
                REQUIRE(q.getHead()->data == 2);
            }
        }
    }
}


