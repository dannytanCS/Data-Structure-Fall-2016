//
//  main.cpp
//  hw08
//
//  Created by Danny Tan on 11/11/16.
//  Copyright © 2016 NYU. All rights reserved.
//

#define CATCH_CONFIG_MAIN
#include<istream>
#include "Tokenizer.cpp"
#include "catch.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// Code from Weiss
// Symbol is the class that will be placed on the Stack.
struct Symbol
{
    char token;
    int  theLine;
    
};


// NOTE. THE HW ONLY REQUIRED YOU TO CHECK FOR PARENTHESIS ONLY. I WROTE THE CODE THAT CHECKS FOR EVERY BRACKET
//
class Balance
{
public:
    Balance( istream & is ):tok( is ),errors( 0 ){}
    int checkBalance(); // returns the number of mismatched parenthesis
private:
    Tokenizer tok;
    int errors;
    void checkMatch( const Symbol & oParen, const Symbol & cParen );
};

int Balance::checkBalance(){
    Symbol newParen;
    stack<Symbol> stackOfParen;
    
    newParen.token = tok.getNextOpenClose();
    newParen.theLine = tok.getLineNumber( );
    
    while(newParen.token != '\0')
    {
        
        if (newParen.token == '(') {
            stackOfParen.push( newParen );
        }
        else if (newParen.token == ')') {
            
            if( stackOfParen.empty( ) )
            {
                cout << "Extra close parenthesis " << " at line " << tok.getLineNumber( )  <<  endl;
                errors++;
            }
            else
            {

                checkMatch( stackOfParen.top() , newParen );
                stackOfParen.pop( );
            }
        }
        newParen.token = tok.getNextOpenClose();
        newParen.theLine = tok.getLineNumber( );

    }
    while ( !stackOfParen.empty( ) )
    {
        cout << "Extra close parenthesis " << " at line " << stackOfParen.top().theLine <<  endl;
        stackOfParen.pop();
        errors++;
    }
    
    return errors;
}

void Balance::checkMatch(const Symbol & oParen, const Symbol & cParen ){
    if ( oParen.token == '(' && cParen.token != ')')
    {
        cout << cParen.token << " on line " << tok.getLineNumber( ) << " does not match " << oParen.token << " at line " << oParen.theLine << endl;
        errors++;
    }
}


SCENARIO("Checking your Balancer"){
    GIVEN("a test file test.cc with 2 known issues taken in as a ifstream"){
        ifstream ifs("test_cases/test.cc");
        if (!ifs) cerr << "Failure to open file" << endl;
        cerr << "test.cc" << endl;
        WHEN("Given a balancer"){
            Balance b(ifs);
            THEN("CheckBalance should return 1"){
                REQUIRE(b.checkBalance() == 1);
            }
        }
        ifs.close();
    }
    GIVEN("a test file test2.cc with 3 known issues taken in as a ifstream"){
        ifstream ifs("test_cases/test2.cc");
        if(!ifs) cerr << "Failure to open file" << endl;
        cerr << "test2.cc" << endl;
        WHEN("Given a balancer"){
            Balance b(ifs);
            THEN("Check balance should return 2"){
                REQUIRE(b.checkBalance() == 2);
            }
        }
    }
    GIVEN("a test file test3.cc with 0 known issues taken in as a ifstream"){
        ifstream ifs("test_cases/test3.cc");
        if(!ifs) cerr << "Failure to open file" << endl;
        cerr << "test3.cc" << endl;
        WHEN("Given a balancer"){
            Balance b(ifs);
            THEN("Check balance should return 0"){
                REQUIRE(b.checkBalance() == 0);
            }
        }
    }
}


// Get point values from the file and store them in the vector
void initValues (vector<int>& pointValues) {
    ifstream ifs("Letter_point_value.txt");
    if(!ifs) cerr << "Failure to open file" << endl;
    int point;
    char letter;
    while (ifs >> point >> letter) {
        pointValues[letter - 65] = point;
    }
    ifs.close();
}

// Compute point value of a word
int wordValue (const vector<int>& pointValues, const string& word) {
    int point = 0;
    for (int i = 0; i < word.size(); i++){
        point += pointValues[toupper(word[i]) - 65];
    }
    return point;
}

// Store all words and their point values in the map
void initWords (const vector<int>& pointValues, map<string, int>& wordValues) {
    ifstream ifs("ENABLE.txt");
    if(!ifs) cerr << "Failure to open file" << endl;
    string word;
    while (ifs >> word) {
        wordValues[word] = wordValue(pointValues, word);
    }
    ifs.close();
}

SCENARIO("Testing words with friends"){
    GIVEN("Given a map and vector"){
        vector<int> pointValues(26,0);
        map<string, int> wordValues;
        WHEN("The map is initialized with this vector with word values"){
            initValues(pointValues);
            initWords(pointValues, wordValues);
            THEN("abacterial should be valued 17"){
                string test = "abacterial";
                map<string, int>::iterator itr = wordValues.find(test);
                REQUIRE(itr->second == 17);
            }
            THEN("zyzzyva should be valued 42"){
                string test = "zyzzyva";
                map<string, int>::iterator itr = wordValues.find(test);
                REQUIRE(itr->second == 42);
            }
            THEN("fuzees should be valued 19"){
                string test = "fuzees";
                map<string, int>::iterator itr = wordValues.find(test);
                REQUIRE(itr->second == 19);
            }
            THEN("poly should be valued 10"){
                string test = "poly";
                map<string, int>::iterator itr = wordValues.find(test);
                REQUIRE(itr->second == 10);
            }
        }
    }
}




