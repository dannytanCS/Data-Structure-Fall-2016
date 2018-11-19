//
//  main.cpp
//  hw10
//
//  Created by Danny Tan on 12/3/16.
//  Copyright © 2016 NYU. All rights reserved.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

template<typename HashedObj>
class HashTable
{
public:
    enum EntryType { ACTIVE, EMPTY, DELETED };
private:
    struct HashEntry {
        HashedObj element;
        EntryType info;
        HashEntry( const HashedObj & e = HashedObj(), EntryType i = EMPTY )
        : element( e ), info( i ) {}
    };
public:
    typedef typename vector<HashEntry>::const_iterator iterator;
    explicit HashTable(int size = 101):currentSize(0) {
        array.resize(size);
    }
    
    iterator find( const HashedObj & );
    bool remove( const HashedObj & );
    void makeEmpty();
    bool insert( const HashedObj & );
    iterator end() const;
    int capacity() const;
    
private:
    std::hash<HashedObj> hf;
    
    vector<HashEntry> array;
    int currentSize;
    void rehash();
};

template< typename HashedObj >
bool HashTable<HashedObj>::insert( const HashedObj & x ) {
    if ( find(x) != array.end() ) {
        return false;
    }
    size_t i = hf(x) % array.size();
    while (array[i].info == ACTIVE) {
        ++i;
    }
    array[i] = HashEntry( x, ACTIVE );
    if ( ++currentSize  >= array.size() * 0.5 ){
        rehash();
    }
    return true;
}

template< class HashedObj >
typename HashTable<HashedObj>::iterator HashTable<HashedObj>::find( const HashedObj & x ) {
    int index = hf(x) % array.size();
    for (iterator itr = array.begin() + index; itr != array.end(); ++itr) {
        if (itr->info == ACTIVE && itr->element == x) {
            return itr;
        }
    }
    for (iterator itr = array.begin(); itr != array.begin() + index; ++itr) {
        if (itr->info == ACTIVE && itr->element == x) {
            return itr;
        }
    }
    return array.end();
}

template< typename HashedObj >
bool HashTable<HashedObj>::remove( const HashedObj & x ) {
    int index =  hf(x) % array.size() ;
    for( int i = index; i < array.size(); ++i ) {
        if ( array[i].info == EMPTY )
            return false;
        if ( array[i].info == ACTIVE && array[i].element == x ) {
            array[i].info = DELETED;
            --currentSize;
            return true;
        }
    }
    for( int i = 0; i < index; ++i ) {
        if ( array[i].info == EMPTY )
            return false;
        if ( array[i].info == ACTIVE && array[i].element == x ) {
            array[i].info = DELETED;
            --currentSize;
            return true;
        }
    }
    /**
     * There's a clever but convoluted way to merge the above two loops into one
     * if you would like to give it a shot.
     */
    return false;
}

template< typename HashedObj >
void HashTable<HashedObj>::rehash() {
    vector< HashEntry> oldArray = std::move( array );
    array.clear();
    currentSize = 0;
    array.resize(2*oldArray.size()+1);
    for( int i = 0; i < array.size( ); i++ ){
            array[ i ].info = EMPTY;
    }
    
    for( int i = 0; i < oldArray.size( ); i++ ) {
        if( oldArray[ i ].info == ACTIVE ) {
            insert( oldArray[ i ].element );
        }
    }
    return;
}

template< typename HashedObj >
void HashTable<HashedObj>::makeEmpty() {
    for( auto i : array )
        i.info = EMPTY;
}

template<typename HashedObj>
typename HashTable<HashedObj>::iterator HashTable<HashedObj>::end() const {
    return array.end();
}

template<typename HashedObj>
int HashTable<HashedObj>::capacity() const {
    return array.size();
}

SCENARIO("Testing the hash table with linear probing") {
    HashTable<int> hst;
    int n = 10; //adjust to make it less than 10 when rehash isn't implemented;
    int capacity = hst.capacity();
    
    GIVEN("A hash table has been created but not filled") {
        WHEN("Method insert is called") {
            THEN("All insertions of original elements should succeed") {
                for( int i = 0; i < n; ++i )
                    REQUIRE( hst.insert(i*capacity) == true );
            }
            THEN("All insertions of duplicate elements should fail") {
                for( int i = 0; i < n; ++i) {
                    REQUIRE( hst.insert(i * capacity) == true );
                    REQUIRE( hst.insert(i * capacity) == false );
                }
            }
        }
    }
    
    GIVEN("A hash table has been created and filled ") {
        for( int i = 0; i < n; ++i )
            hst.insert( i * capacity);
        capacity = hst.capacity();
        
        WHEN("Method insert is called") {
            THEN("It should fail on all duplicate elements") {
                for( int i = 0; i < n; ++i) {
                    REQUIRE( hst.insert(i*capacity) == false );
                }
            }
            THEN("It should pass on all original elements") {
                for( int i = 0; i < n; ++i ) {
                    hst.remove(i * capacity);
                    REQUIRE( hst.insert( i*capacity ) == true );
                }
                for( int i = n; i < 2*n; ++i ) {
                    REQUIRE( hst.insert( i*capacity ) == true );
                }
            }
        }
        
        WHEN("Method remove is called") {
            THEN("It should return true for any item in the table.") {
                for( int i = 0; i < n; ++i )
                    REQUIRE( hst.remove(i * capacity) == true );
            }
        }
        
        WHEN("Method remove is called") {
            THEN("It should return false for any item not in the table.") {
                for( int i = n; i < 2*n; ++i )
                    REQUIRE( hst.remove(i * capacity) == false );
            }
        }
        
        WHEN("Method find is used") {
            for( int i = 0; i < n; ++i )
                hst.insert(i * capacity);
            THEN("All inserted elements should be found") {
                for( int i = 0; i < n; ++i )
                    REQUIRE( hst.find(i * capacity)->element ==  i * capacity );
            }
            THEN("Any non-inserted element should not be found") {
                for( int i = n; i < 2*n; ++i )
                    REQUIRE( hst.find(i*capacity) == hst.end() );
            }
            THEN("Any element that is deleted should not be found.") {
                for( int i = 0; i < n; ++i ) {
                    hst.remove(i * capacity);
                    REQUIRE( hst.find(i * capacity) == hst.end() );
                }
            }
        }
        
        WHEN("The load factor is greater than 0.5") {
            const int * temp = &(hst.find(5 * capacity)->element);
            for( int i = 1; i < 100; ++i )
                hst.insert( i );
            THEN("rehash() should be called.") {
                REQUIRE( hst.capacity() != capacity  );
            }
        }
    }
}



SCENARIO("Checking for adjacency") {
    GIVEN("Some subways with adjacency are tested") {
        unordered_map < string, list<string>> adjList;
        WHEN("The container has been populated") {
            ifstream trainStream ("MTA_train_stop_data.txt");
            if (!trainStream){
                cout << "Could not find file." << endl;
                exit (1);
            }
            string line, id;
            string previous = "";
            getline(trainStream,line); //skip first line
            while (getline(trainStream, line)) {
                stringstream lineStream(line);
                getline(lineStream, id, ',');
                if (id.length() == 3) {
                    if (previous != ""){
                        adjList[id].push_back(previous);
                        adjList[previous].push_back(id);
                    }
                    previous = id;
                }
            }
            trainStream.close();
            
            ifstream transferStream ("transfers.txt");
            if (!transferStream){
                cout << "Could not find file." << endl;
                exit (1);
            }
            string transfer;
            getline(transferStream,line); //skip first line
            while (getline(transferStream, line)) {
                stringstream lineStream(line);
                getline(lineStream, id, ',');
                getline(lineStream, transfer, ',');

                if (id != transfer) {
                    adjList[id].push_back(transfer);
                }
            }
            transferStream.close();

                

            //checking if stop 125 is there
            unordered_map < string, list<string>> ::iterator adjListItr = adjList.find("125");
            THEN("125 is a stop in the container ") {
                //make sure find returns a true
                REQUIRE(adjListItr != adjList.end());
            }
            THEN("The adjacency list of stop 125 is correctly stored") {
                //go through and check all of adjacency list of station 125
                vector<string> adjVec;
                //putting all the adjacent stops in a vector so that the order in which they were inserted into the map doesn't matter
                for (list<string>::iterator litr = adjListItr->second.begin(); litr != adjListItr->second.end(); litr++) {
                    adjVec.push_back(*litr);
                }
                
                //Checking to see if stops 124, A24, and 126 are there
                bool stop124 = false;
                bool stopA24 = false;
                bool stop126 = false;
                for (int i = 0; i < adjVec.size(); i++) {
                    if (adjVec[i] == "124") stop124 = true;
                    else if (adjVec[i] == "A24") stopA24 = true;
                    else if (adjVec[i] == "126") stop126 = true;
                }
                REQUIRE(stop124);
                REQUIRE(stopA24);
                REQUIRE(stop126);
                
            }
            
            //checking if stop B16 is there
            unordered_map < string, list<string>> ::iterator adjListItr2 = adjList.find("B16");
            THEN("B16 is a stop in the container ") {
                //make sure find returns a true
                REQUIRE(adjListItr2 != adjList.end());
            }
            THEN("The adjacency list of stop B16 is correctly stored") {
                //go through and check all of adjacency list of station 125
                vector<string> adjVec;
                //putting all the adjacent stops in a vector so that the order in which they were inserted into the map doesn't matter
                for (list<string>::iterator litr = adjListItr2->second.begin(); litr != adjListItr2->second.end(); litr++) {
                    adjVec.push_back(*litr);
                }
                
                //Checking to see if stops B15, N04 and B17 are there 
                bool stopB15 = false;
                bool stopN04 = false;
                bool stopB17 = false;
                for (int i = 0; i < adjVec.size(); i++) {
                    if (adjVec[i] == "B15") stopB15 = true;
                    else if (adjVec[i] == "N04") stopN04 = true;
                    else if (adjVec[i] == "B17")stopB17 = true;
                }
                REQUIRE(stopB15);
                REQUIRE(stopN04);
                REQUIRE(stopB17);
                
            }
        }
    }
}


