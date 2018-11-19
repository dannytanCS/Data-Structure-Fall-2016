//  Danny Tan
//  main.cpp
//  hw01
//
//  Created by Danny Tan on 9/11/16.
//  Copyright © 2016 Danny Tan. All rights reserved.
//

#include <iostream>
#include <vector>
using namespace std;


//question 1

class timer {
public:
    timer() : start(clock()) {}
    double elapsed() { return ( clock() - start ) / CLOCKS_PER_SEC; }
    void reset() { start = clock(); }
private:
    double start;
};



/**
 * Cubic maximum contiguous subsequence sum algorithm.
 */
int maxSubSum1( const vector<int> & a )
{
    int maxSum = 0;
    
    for( int i = 0; i < a.size( ); ++i )
        for( int j = i; j < a.size( ); ++j )
        {
            int thisSum = 0;
            
            for( int k = i; k <= j; ++k )
                thisSum += a[ k ];
            
            if( thisSum > maxSum )
                maxSum   = thisSum;
        }
    
    return maxSum;
}


/**
 * Quadratic maximum contiguous subsequence sum algorithm.
 */
int maxSubSum2( const vector<int> & a )
{
    int maxSum = 0;
    
    for( int i = 0; i < a.size( ); ++i )
    {
        int thisSum = 0;
        for( int j = i; j < a.size( ); ++j )
        {
            thisSum += a[ j ];
            
            if( thisSum > maxSum )
                maxSum = thisSum;
        }
    }
    
    return maxSum;
}

/**
 * Linear-time maximum contiguous subsequence sum algorithm.
 */
int maxSubSum4( const vector<int> & a )
{
    int maxSum = 0, thisSum = 0;
    
    for( int j = 0; j < a.size( ); ++j )
    {
        thisSum += a[ j ];
        
        if( thisSum > maxSum )
            maxSum = thisSum;
        else if( thisSum < 0 )
            thisSum = 0;
    }
    
    return maxSum;
}

//finds time for first algorithm
void findsTime1(const vector<int>& items) {
    timer t;
    double nuClicks;
    t.reset();
    maxSubSum1(items);
    nuClicks = t.elapsed();
    cout << nuClicks << " for first algorithm" << endl;
}

//finds time for second algorithm
void findsTime2(const vector<int>& items) {
    timer t;
    double nuClicks;
    t.reset();
    maxSubSum2(items);
    nuClicks = t.elapsed();
    cout << nuClicks << " for second algorithm" << endl;
}



//finds time for fourth algorithm
void findsTime4(const vector<int>& items) {
    timer t;
    double nuClicks;
    t.reset();
    maxSubSum4(items);
    nuClicks = t.elapsed();
    cout << nuClicks << " for fourth algorithm" << endl;
}



//test when n = 128
void firstTest(){
    vector<int> vector;
    for (int i = 0; i < 128; i++){
        int x = (rand() % 2001) - 1000;
        vector.push_back(x);
    }
    cout << "n = 128" << endl;
    findsTime1(vector);
    findsTime2(vector);
    findsTime4(vector);
    
}

//test when n = 256
void secondTest(){
    vector<int> vector;
    for (int i = 0; i < 256; i++){
        int x = (rand() % 2001) - 1000;
        vector.push_back(x);
    }
    cout << "n = 256" << endl;
    findsTime1(vector);
    findsTime2(vector);
    findsTime4(vector);
}

//test when n = 512
void thirdTest(){
    vector<int> vector;
    for (int i = 0; i < 512; i++){
        int x = (rand() % 2001) - 1000;
        vector.push_back(x);
    }
    cout << "n = 512" << endl;
    findsTime1(vector);
    findsTime2(vector);
    findsTime4(vector);
}

//test when n = 1024
void fourthTest(){
    vector<int> vector;
    for (int i = 0; i < 1024; i++){
        int x = (rand() % 2001) - 1000;
        vector.push_back(x);
    }
    cout << "n = 1024" << endl;
    findsTime1(vector);
    findsTime2(vector);
    findsTime4(vector);
}

//test when n = 2048
void fifthTest(){
    vector<int> vector;
    for (int i = 0; i < 2048; i++){
        int x = (rand() % 2001) - 1000;
        vector.push_back(x);
    }
    cout << "n = 2048" << endl;
    findsTime1(vector);
    findsTime2(vector);
    findsTime4(vector);
}

//test when n = 4096
void sixthTest(){
    vector<int> vector;
    for (int i = 0; i < 4096; i++){
        int x = (rand() % 2001) - 1000;
        vector.push_back(x);
    }
    cout << "n = 4096" << endl;
    findsTime1(vector);
    findsTime2(vector);
    findsTime4(vector);
}


//question 2

int algorithm2b(int n){
    int sum = 0;
    for (int i = 0; i < n; i++) {
        ++sum;
    }
    return sum;
}

int algorithm2c(int n){
    int sum = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; ++j) {
            ++sum;
        }
    }
    return sum;
}

int algorithm2d(int n){
    int sum = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < i; ++j) {
            ++sum;
        }
    }
    return sum;
}

int algorithm2e(int n){
    int sum = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; ++j) {
            for(int k = 0; k < n; ++k) {
                ++sum;
            }
        }
    }
    return sum;
}



//finds time for 2b
void findsTime2b(int x) {
    timer t;
    double nuClicks;
    t.reset();
    algorithm2b(x);
    nuClicks = t.elapsed();
    cout << nuClicks << " for 2b" << endl;
}

//finds time for 2c
void findsTime2c(int x) {
    timer t;
    double nuClicks;
    t.reset();
    algorithm2c(x);
    nuClicks = t.elapsed();
    cout << nuClicks << " for 2c" << endl;
}



//finds time for 2d
void findsTime2d(int x) {
    timer t;
    double nuClicks;
    t.reset();
    algorithm2d(x);
    nuClicks = t.elapsed();
    cout << nuClicks << " for 2d" << endl;
}

//finds time for 2e
void findsTime2e(int x) {
    timer t;
    double nuClicks;
    t.reset();
    algorithm2e(x);
    nuClicks = t.elapsed();
    cout << nuClicks << " for 2e" << endl;
}


void findsTimeForQ2(){
    vector<int> vector {256,512,1024,2048,4096};
    for (int i = 0; i < vector.size(); i ++) {
        cout << "For n =" << vector[i] << endl;
        findsTime2b(vector[i]);
        findsTime2c(vector[i]);
        findsTime2d(vector[i]);
        findsTime2e(vector[i]);
    }
}








int main() {
    cout.precision(numeric_limits<double>::digits10 + 1);
    
    //number 1
    cout << "NUMBER 1" << endl;
    firstTest();
    secondTest();
    thirdTest();
    fourthTest();
    fifthTest();
    sixthTest();
    
    //number 2
    cout << endl << "NUMBER 2" << endl;
    findsTimeForQ2();
    
}
