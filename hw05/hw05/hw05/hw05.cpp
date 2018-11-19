//
//  main.cpp
//  hw05
//
//  Created by Danny Tan on 10/20/16.
//  Copyright © 2016 NYU. All rights reserved.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"		
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>
#include <math.h>
#include <functional>


using namespace std;

//number 1

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
    string line, id, name, garbage, lati,longi;
    getline(trainStream,line); //skip first line
    while (getline(trainStream, line)) {
        stringstream lineStream(line);
        getline(lineStream, id, ',');
        if (id.length() == 3) {
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
    }
    trainStream.close();
}

//number 2
template<typename iter, typename predi, typename opr>
int perform_if(iter itrStart, iter itrEnd, predi pred, opr op){
    int how_many = 0;
    for (iter itr = itrStart; itr != itrEnd; itr++ ) {
        if ( pred( *itr ) )
        {
            op(*itr);
            how_many++;
        }
    }
    return how_many;
}


class isStopOnRoute{
public:
    isStopOnRoute(char someRoute): route(someRoute) {}
    bool operator()(const trainStopData& stopData) {
        return stopData.get_id()[0] == route;
    }
private:
    char route;
};

class isSubwayStop {
public:
    isSubwayStop(string id): stopId(id) {}
    bool operator()(const trainStopData& stopData) {
        return stopData.get_id() == stopId;
    }
private:
    string stopId;
};


double degrad(double d) {
    return d * M_PI / 180;
}


double haverdist(double lat1, double longit1, double lat2, double longit2)
{
    double r = 6371;
    double dlat = degrad(lat2 - lat1);
    double dlongit = degrad(longit2 - longit1);
    double a = sin(dlat/2)*sin(dlat/2)+
    cos(degrad(lat1))*cos(degrad(lat2))*sin(dlongit/2)*sin(dlongit/2);
    double c = 2*atan2(sqrt(a),sqrt(1-a));
    return r*c;
}

class isSubwayStopNearX {
public:
    isSubwayStopNearX(double lon, double lat, double d): longitude(lon), latitude(lat), d(d) {}
    bool operator()(const trainStopData& stopData) const {
        return haverdist(stopData.get_latitude(),stopData.get_longitude(),latitude,longitude) <= d;
    }
private:
    double longitude;
    double latitude;
    double d;
};


class printTrainStopInfo{
public:
    void operator()(const trainStopData& stopData) const {
        cout << "ID: " << stopData.get_id() << " Name: " << stopData.get_stop_name() << " Lat: " << stopData.get_latitude() << " Lon: " << stopData.get_longitude() << endl;
    }
};




void menu(const vector<trainStopData>& trainVec) {
    char input;
    cout << "Please select a following choice:" << endl;
    cout << "'A' Print out the information about all the train stops on a specific route" << endl;
    cout << "'B' Print out the information about a specific train stop" << endl;
    cout << "'C' Print out all the train stops within a certain distance" << endl;
    cout << "'D' Quit" << endl;
    cout << "Please enter a choice: ";
    cin >> input;
    if (input == 'A') {
        char route;
        cout << "Please enter a route: ";
        cin >> route;
        int num = perform_if(trainVec.begin(), trainVec.end(), isStopOnRoute(route), printTrainStopInfo());
        if (num == 0) {
            cout << "No train stop in that route" << endl;
        }
    }
    if (input == 'B') {
        string id;
        cout << "Please enter an ID: ";
        cin >> id;
        int num = perform_if(trainVec.begin(), trainVec.end(), isSubwayStop(id), printTrainStopInfo());
        if (num == 0) {
            cout << "No train stop with that ID" << endl;
        }
    }
    if (input == 'C') {
        double lat, lon, d;
        cout << "Please enter a latitude: ";
        cin >> lat;
        cout << "Please enter a longtitude: ";
        cin >> lon;
        cout << "Please enter the maximum distance: ";
        cin >> d;
        int num = perform_if(trainVec.begin(), trainVec.end(), isSubwayStopNearX(lon,lat,d), printTrainStopInfo());
        if (num == 0) {
            cout << "No train stop within that distance" << endl;
        }
    }
    
    if (input != 'D') {
        menu(trainVec);
    }
}


template<class RandItr>
void mergeSort(RandItr tmp, RandItr start, RandItr end) {
    //base case: one element
    if (end - start == 1) return;
    
    RandItr center = start + (end - start) / 2;
   
    mergeSort(tmp, start, center);
    mergeSort(tmp + (end - start) / 2, center, end);
    merge(start, center, center, end, tmp);
    move(tmp, tmp + int(end - start), start);
    
}

template<class RandItr>
void mergeSort(RandItr start, RandItr end){
    int  sz = end - start;  // or use auto sz = end-start;
    typedef typename iterator_traits< RandItr >::value_type Object; //Xcode
    vector<Object> tmp(sz);
    mergeSort(tmp.begin(), start, end);
}



SCENARIO("TESTING MERGESORT"){
    GIVEN("An unsorted vector of integers"){
        vector<int> randomVec = {9,0,2,1,0,1,2,3,4,5,6,7,8,9};
        WHEN("Mergsort is used to sort the vector from least to greatest"){
            size_t numElements = randomVec.size();
            mergeSort(randomVec.begin(), randomVec.end());
            THEN("The number of elements should remain the same"){
                REQUIRE(numElements == randomVec.size());
            }
            THEN("The vector is sorted"){
                for(int x = 1; x < randomVec.size(); ++x){
                    REQUIRE(randomVec[x-1] <= randomVec[x]);
                }
            }
        }
    }
}

class student{
private:
    string name;
    double gpa;
public:
    student(const string& name, double gpa):name(name), gpa(gpa){}
    string get_name() const {return name;}
    double get_gpa() const {return gpa;}
};


class meFirst {
public:
    meFirst(string me): me(me) {}
    bool operator()(const student& student1, const student& student2){
        if (student1.get_name() == me) {
            return true;
        }
        else if (student2.get_name() == me) {
            return false;
        }
        else {
            return student1.get_name() < student2.get_name();
        }
    }
private:
    string me;
};



SCENARIO("TESTING MEFIRST"){
    GIVEN("A functor meFirst"){
        string priority = "Saitama";
        meFirst m(priority);
        WHEN("students are created"){
            student Saitama_Sensei(priority, 2.0);
            student Genos("Genos", 4.0);
            student Bang("Bang", 3.8);
            THEN("When the me matches, priority student goes first"){
                REQUIRE(m(Saitama_Sensei, Genos));
            }
            THEN("When two students who do not match, alphabetically first goes first"){
                REQUIRE(m(Genos,Bang) == false);
                REQUIRE(m(Bang, Genos) == true);
            }
        }
    }
}


void quickSort(vector<bool>& vecBool) {
    bool pivot = vecBool[vecBool.size()-1];
    int wall = 0;
    for (int i = 0; i < vecBool.size(); i++) {
        if (vecBool[i] == pivot) {
            swap(vecBool[wall], vecBool[i]);
            wall ++;
        }
    }
    swap(vecBool[wall],pivot);

}


SCENARIO("Order the Trues and Falses"){
    GIVEN("A vector of boolean values (true/false)"){
        vector<bool> vecBool = {false, true, false, true, true, true, false, false};
        WHEN("Sorted"){
            quickSort(vecBool);
            
            THEN("All falses should come before true"){
                bool lastSeen = false;
                for(int x = 0; x < vecBool.size(); ++x){
                    if(lastSeen == true){
                        REQUIRE(vecBool[x] == true);
                    }
                    lastSeen = vecBool[x];
                }
            }
        }
    }
}
