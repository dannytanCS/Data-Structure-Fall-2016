//
//  main.cpp
//  hw04b
//
//  Created by Danny Tan on 10/7/16.
//  Copyright © 2016 NYU. All rights reserved.
//


#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <list>
#include <set>
#include <unordered_set>

using namespace std;

class trainStopData {
public:
    trainStopData(string id, string name, double lat, double lon): stop_id(id), stop_name(name), stop_lat(lat), stop_lon(lon) {}
    string get_id( ) const {
        return stop_id;
    }
    string get_stop_name( ) const {
        return stop_name;
    }
    double get_latitude( ) const {
        return stop_lat;
    }
    double get_longitude( ) const {
        return stop_lon;
    }
private:
    string stop_id; // id of train stop (1st token)
    string stop_name; // name of station (4th token)
    double stop_lat; // latitude of train stop location
    double stop_lon; // longitude of train stop location
};
void parseTrainData(vector<trainStopData>& vecData) {
    ifstream trainStream ("MTA_train_stop_data.txt");
    if (!trainStream){
        cout << "Could not find file." << endl;
        exit (1);
    }
    string line;
    string id, name, garbage;
    string lati,longi;
    getline(trainStream,line); //skip first line
    while (getline(trainStream, line)) {
        stringstream lineStream(line);
        getline(lineStream, id, ',');
        getline(lineStream, garbage, ',');
        getline(lineStream, name, ',');
        getline(lineStream, garbage, ',');
        getline(lineStream, lati, ',');
        double lat = atof( lati.c_str() );
        getline(lineStream, longi, ',');
        double lon = atof( longi.c_str() );
        trainStopData aTrainStop(id, name, lat, lon);
        vecData.push_back(aTrainStop);
        
    }
    trainStream.close();
}


SCENARIO("Testing vector<trainStopData>") {
    GIVEN("A vector with trainStopData") {
        vector<trainStopData> test;
        parseTrainData(test);
        WHEN("Checking the size") {
            int size = test.size();
            THEN("The size should be 1482") {
                REQUIRE(size == 1482); //testing if the vector contains all the data
            }
        }
        WHEN("Getting stop_id") {
            string id = test[100].get_id(); //test a random data
            THEN("A valid id should be not nothing") {
                REQUIRE(id != "");
            }
        }
        WHEN("Getting stop_name") {
            string name = test[1000].get_stop_name(); //test a random data
            THEN("A valid name should be not nothing") {
                REQUIRE(name != "");
            }
        }
        WHEN("Getting latitude") {
            double lat = test[10].get_latitude(); //test a random data
            THEN("A valid latitude should be not nothing") {
                REQUIRE(lat != 0);
            }
        }
        WHEN("Getting longitude") {
            double lon = test[10].get_longitude(); //test a random data
            THEN("A valid longitude should be not nothing") {
                REQUIRE(lon != 0);
            }
        }
    }
}


template <class Object>
bool dupTestList(const vector<Object>& vec) {
    list<Object> objList(vec.begin(), vec.end());
    for (auto itr = objList.begin() ; itr != objList.end() ; ++itr) {
        auto temp = itr;
        for (auto itr2 = ++ temp; itr2 != objList.end(); ++itr2) {
            if (*itr == *itr2) {
                return true;
            }
        }
    }
    return false;
}

template <class Object>
bool dupTestSort(const vector<Object>& vec) {
    vector<Object> temp(vec.begin(), vec.end());
    sort(temp.begin(), temp.end());
    for (auto itr = temp.begin(); itr != temp.end(); ++itr) {
        if (*itr == *(itr+1)) {
            return true;
        }
    }
    return false;
}

template <class Object>
bool dupTestSet(const vector<Object>& vec) {
    set<Object> objSet(vec.begin(), vec.end());
    if (objSet.size() == vec.size()){
        return false;
    }
    return true;
}

template <class Object>
bool dupTestUnorderedSet(const vector<Object>& vec) {
    unordered_set<Object> unorderObj(vec.begin(), vec.end());
    if (unorderObj.size() == vec.size()){
        return false;
    }
    return true;
}

SCENARIO("Testing check if there is duplicates") {
    GIVEN("A vector of int with no duplicates") {
        vector<int> intvec;
        intvec.push_back(1);
        intvec.push_back(2);
        intvec.push_back(3);
        WHEN("Checking duplicates with list") {
            bool isDup = dupTestList(intvec);
            THEN("It should return false") {
                REQUIRE(isDup == false);
            }
        }
        WHEN("Checking duplicates with sort+vec") {
            bool isDup = dupTestSort(intvec);
            THEN("It should return false") {
                REQUIRE(isDup == false);
            }
        }
        WHEN("Checking duplicates with set") {
            bool isDup = dupTestSet(intvec);
            THEN("It should return false") {
                REQUIRE(isDup == false);
            }
        }
        WHEN("Checking duplicates with unordered set") {
            bool isDup = dupTestUnorderedSet(intvec);
            THEN("It should return false") {
                REQUIRE(isDup == false);
            }
        }
    }
    GIVEN("A vector of char with duplicates") {
        vector<char> charvec;
        charvec.push_back('a');
        charvec.push_back('c');
        charvec.push_back('a');
        WHEN("Checking duplicates with list") {
            bool isDup = dupTestList(charvec);
            THEN("It should return true") {
                REQUIRE(isDup == true);
            }
        }
        WHEN("Checking duplicates with sort+vec") {
            bool isDup = dupTestSort(charvec);
            THEN("It should return true") {
                REQUIRE(isDup == true);
            }
        }
        WHEN("Checking duplicates with set") {
            bool isDup = dupTestSet(charvec);
            THEN("It should return true") {
                REQUIRE(isDup == true);
            }
        }
        WHEN("Checking duplicates with unordered set") {
            bool isDup = dupTestUnorderedSet(charvec);
            THEN("It should return true") {
                REQUIRE(isDup == true);
            }
        }
    }
    GIVEN("An empty vector of string") {
        vector<string> svec;
        WHEN("Checking duplicates with list") {
            bool isDup = dupTestList(svec);
            THEN("It should return false") {
                REQUIRE(isDup == false);
            }
        }
        WHEN("Checking duplicates with sort+vec") {
            bool isDup = dupTestSort(svec);
            THEN("It should return false") {
                REQUIRE(isDup == false);
            }
        }
        WHEN("Checking duplicates with set") {
            bool isDup = dupTestSet(svec);
            THEN("It should return false") {
                REQUIRE(isDup == false);
            }
        }
        WHEN("Checking duplicates with unordered set") {
            bool isDup = dupTestUnorderedSet(svec);
            THEN("It should return false") {
                REQUIRE(isDup == false);
            }
        }
    }
}


