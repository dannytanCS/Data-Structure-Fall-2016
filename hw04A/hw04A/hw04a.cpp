//
//  main.cpp
//  hw04A
//
//  Created by Danny Tan on 10/6/16.
//  Copyright © 2016 NYU. All rights reserved.
//


#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>

using namespace std;

template <typename Object>
class Vector
{
public:
    explicit Vector(int initSize = 0)
    : theSize{ initSize }, theCapacity{ initSize + SPARE_CAPACITY }
    {
        objects = new Object[theCapacity];
    }
    
    Vector(const Vector & rhs)
    : theSize{ rhs.theSize }, theCapacity{ rhs.theCapacity }, objects{ nullptr }
    {
        objects = new Object[theCapacity];
        for (int k = 0; k < theSize; ++k)
            objects[k] = rhs.objects[k];
    }
    
    Vector & operator= (const Vector & rhs)
    {
        Vector copy = rhs;
        std::swap(*this, copy);
        return *this;
    }
    
    ~Vector()
    {
        delete[] objects;
    }
    
    Vector(Vector && rhs)
    : theSize{ rhs.theSize }, theCapacity{ rhs.theCapacity }, objects{ rhs.objects }
    {
        rhs.objects = nullptr;
        rhs.theSize = 0;
        rhs.theCapacity = 0;
    }
    
    Vector & operator= (Vector && rhs)
    {
        std::swap(theSize, rhs.theSize);
        std::swap(theCapacity, rhs.theCapacity);
        std::swap(objects, rhs.objects);
        
        return *this;
    }
    
    bool empty() const
    {
        return size() == 0;
    }
    int size() const
    {
        return theSize;
    }
    int capacity() const
    {
        return theCapacity;
    }
    
    Object & operator[](int index)
    {
        return objects[index];
    }
    
    const Object & operator[](int index) const
    {
        return objects[index];
    }
    
    void resize(int newSize)
    {
        if (newSize > theCapacity)
            reserve(newSize * 2);
        theSize = newSize;
    }
    
    void reserve(int newCapacity)
    {
        if (newCapacity < theSize)
            return;
        
        Object *newArray = new Object[newCapacity];
        for (int k = 0; k < theSize; ++k)
            newArray[k] = std::move(objects[k]);
        
        theCapacity = newCapacity;
        std::swap(objects, newArray);
        delete[] newArray;
    }
    
    // Stacky stuff
    void push_back(const Object & x)
    {
        if (theSize == theCapacity)
            reserve(2 * theCapacity + 1);
        objects[theSize++] = x;
    }
    // Stacky stuff
    void push_back(Object && x)
    {
        if (theSize == theCapacity)
            reserve(2 * theCapacity + 1);
        objects[theSize++] = std::move(x);
    }
    
    
    // Iterator stuff: not bounds checked
    typedef Object * iterator;
    typedef const Object * const_iterator;
    
    iterator begin()
    {
        return &objects[0];
    }
    const_iterator begin() const
    {
        return &objects[0];
    }
    iterator end()
    {
        return &objects[size()];
    }
    const_iterator end() const
    {
        return &objects[size()];
    }
    
    static const int SPARE_CAPACITY = 2;
    
    iterator erase(iterator vItr) {
        for (iterator iter = begin(); iter != end(); iter ++){
            if (iter == vItr){
                *iter = *(iter + 1);
            }
        }
        resize(theSize - 1);
        return vItr;
    }
    
private:
    int theSize;
    int theCapacity;
    Object * objects;
};


SCENARIO("Testing Question 1") {
    GIVEN("A vector of 1 element") {
        Vector<int> vOne;
        vOne.push_back(1);
        Vector<int>::iterator vOneIterator1 = vOne.end();
        WHEN("The element is erased") {
            Vector<int>::iterator catchItr = vOne.erase(vOne.begin());
            THEN("The size should be zero") {
                REQUIRE(vOne.size() == 0);
            }
            THEN("end() should not return the same iterator") {
                REQUIRE(vOne.end() != vOneIterator1);
            }
        }
    }
    GIVEN("A vector of some elements") {
        Vector<int> vSome; 
        vSome.push_back(1); 
        vSome.push_back(2); 
        vSome.push_back(3); 
        int beforeSize = vSome.size(); 
        Vector<int>::iterator beforeErase = vSome.end(); 
        WHEN("The second element is erased") {
            Vector<int>::iterator catchItr = vSome.erase(vSome.begin() + 1); 
            THEN("Make sure the size is decreased by 1") {
                REQUIRE(vSome.size() == beforeSize - 1); 
            }
            THEN("Make sure the order of element is correct") {
                REQUIRE(vSome[0] == 1);
                REQUIRE(vSome[1] == 3); 
                
            }
            THEN("Make sure the last element has been erased") {
                REQUIRE(beforeErase != vSome.end()); 
            }
        }
    }
}

int sumDigits(int x) {
    if (x < 0) {
        x *= -1;
    }
    if (x/10 == 0) return x%10;
    return x%10 + sumDigits( x/10);
}

SCENARIO("Testing Question 2") {
    GIVEN("A one digit positive integer ") {
        WHEN("sumDigits is called") {
            int sum = sumDigits(5);
            THEN("theSum should be ") {
                REQUIRE(sum == 5);
            }
        }
    }
    GIVEN("A one digit negative integer") {
        WHEN("sumDigits is called") {
            int sum = sumDigits(-5);
            THEN("theSum should be ") {
                REQUIRE(sum == 5);
            }
        }
    }
    GIVEN("A positive integer ") {
        WHEN("sumDigits is called") {
            int sum = sumDigits(363);
            THEN("theSum should be ") {
                REQUIRE(sum == 12);
            }
        }
    }
    GIVEN("A negative integer") {
        WHEN("sumDigits is called") {
            int sum = sumDigits(-363);
            THEN("theSum should be ") {
                REQUIRE(sum == 12);
            }
        }
    }
}

//Insert Recursive Function
int vectorSum(vector<int>::const_iterator left, vector<int>::const_iterator right ) {
    
    if( left == right) {
        return *left;
    }

    vector<int>::const_iterator center = left + (right - left) / 2;
    int leftVectorSum = vectorSum(left, center );
    int rightVectorSum = vectorSum(center + 1, right);
    return leftVectorSum + rightVectorSum;
}

//Insert driver function here

int sumVector( const vector<int> & a){
    return a.size( ) > 0 ? vectorSum(a.begin() , a.end() - 1) : 0;
}


SCENARIO("Testing sumVector") {
    GIVEN("A vector with just one element") {
        vector<int> singleElement = { 1 }; //Note that this is the STL vector, so we can initialize like this
        WHEN("The sumVector function is called") {
            int sum = sumVector(singleElement);
            THEN("The correct sume is returned") {
                REQUIRE(sum == 1); //testing base case
            }
        }
    }
    GIVEN("A vector with odd number of integers ") {
        vector<int> vOdd = { 1,2,3 };
        WHEN("sumVector function is called ") {
            int recSum = sumVector(vOdd);
            
            vector<int>::iterator itrStart = vOdd.begin();
            vector<int>::iterator itrEnd = vOdd.end();
            
            int sizeVector = vOdd.size();
            
            THEN("vector stays same after the function call") {
                REQUIRE(itrStart == vOdd.begin());
                REQUIRE(itrEnd == vOdd.end());
            }
            THEN("The size of the vector doesn't change") {
                REQUIRE(sizeVector == vOdd.size());
            }
            THEN("It sums up properly") {
                REQUIRE(recSum ==6);
            }
        }
        GIVEN("A vector with even number of integers ") {
            vector<int> vEven = { 1,2,3,10 };
            
            vector<int>::iterator itrStart = vEven.begin();
            vector<int>::iterator itrEnd = vEven.end();
            
            int sizeVector = vEven.size();
            
            WHEN("sumVector function is called ") {
                
                int recSum = sumVector(vEven);
                
                THEN("vector stays same after the function call") {
                    REQUIRE(itrStart == vEven.begin());
                    REQUIRE(itrEnd == vEven.end());
                }
                THEN("The size of the vector doesn't change") {
                    REQUIRE(sizeVector == vEven.size());
                }
                THEN("It sums up properly") {
                    REQUIRE(recSum == 16);
                }
            }
        }
    }
}

