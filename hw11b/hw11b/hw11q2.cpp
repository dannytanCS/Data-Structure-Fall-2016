//
//  main.cpp
//  hw11b
//
//  Created by Danny Tan on 12/13/16.
//  Copyright © 2016 NYU. All rights reserved.
//


// Code from Lisa Hellerstein.
// This is a bare-bones implementation of the single-source unweighted
// shortest paths algorithm
// It does not have nice classes.

// Assumption: The vertices of the graph are labeled from 0 to n - 1,
// where n is the number of vertices.
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

const int DEFAULT_VAL =  -1;      // must be less than 0

typedef unordered_map<string, list<string>> Graph;        // The graph is given in an adjacency list.
// Vertices are indexed from 0 to V-1
// The indices of the vertices adjacent to vertex i
// are in the list Graph[i].
// Graph can be directed or undirected.

struct vertexInf                    // Stores information for a vertex
{
    int dist;  // distance to vertex from the source
    string prev;  // previous node in BFS tree
};

void printPath(string origin, string j, const unordered_map<string, vertexInf> & vertexInfo)
{
    stack<string> t;
    
    string current = j;
    while (current != "")
    {
        t.push(current);
        auto iter = vertexInfo.find(current);
        if (iter != vertexInfo.end()) {
            current = iter->second.prev;
        }
    }
    cout << "From " << origin << " to " << j << endl;
    cout << "Distance: " << t.size() << endl;
    cout << "Shortest Path: ";
    if (!t.empty()) {
        cout << origin << " ";
    }
    while (!t.empty())
    {
        cout << t.top() << " ";
        t.pop();
    }
    cout << endl;
}



// Breadth First Search
// The unweighted shortest path algorithm on the graph g, with vertex i as the source
// Prints the length (number of edges) of the shortest path from the source to every vertex
// in the graph

void shortestpaths(const Graph & g, string origin_stop_id, string destination_stop_id)
{
    queue<string> q;             // q is the queue of vertex numbers
    
    if(g.find(origin_stop_id) == g.end() || g.find(destination_stop_id) == g.end()) {
        cout << "Wrong Stop ID" << endl;
        return;
    }
    
    // stores BFS info for the vertices
    unordered_map<string, vertexInf> vertices(g.size());
    vertices[origin_stop_id].dist = 0;
    
    // Initialize distances and prev values
    for (unordered_map<string, list<string>>::const_iterator it = g.begin(); it != g.end(); it++) {
        vertices[it->first].dist = DEFAULT_VAL;
        vertices[it->first].prev = "";
    }
    
    q.push(origin_stop_id);
    while  (!q.empty() )
    {
        string v = q.front();
        q.pop();
        
        auto iter = g.find(v);
        if (iter != g.end()) {
            list<string> adjList = iter->second;
            for (list<string>::iterator w = adjList.begin(); w != adjList.end(); w++) {
                if (vertices[*w].dist == DEFAULT_VAL) {
                    vertices[*w].dist = vertices[v].dist+1;
                    if (iter->first != origin_stop_id) {
                        vertices[*w].prev = v;
                    }
                    q.push(*w);
                }
            }
        }
    }
    
    printPath(origin_stop_id, destination_stop_id, vertices);
}

void dataFromMTA(unordered_map<string, list<string>>& adjList) {
    ifstream trainStream ("MTA_train_stop_data.txt");
    if (!trainStream){
        cout << "Could not find MTA_train_stop_data.txt." << endl;
        exit (1);
    }
    string line, id;
    string previous = "";
    getline(trainStream,line); //skip first line
    while (getline(trainStream, line)) {
        stringstream lineStream(line);
        getline(lineStream, id, ',');
        if (id.length() == 3) {
            if (previous != "" && id[0] == previous[0]){
                adjList[id].push_back(previous);
                adjList[previous].push_back(id);
            }
            previous = id;
        }
    }
    trainStream.close();

}

void dataFromTransfers(unordered_map<string, list<string>>& adjList) {
    ifstream transferStream ("transfers.txt");
    if (!transferStream){
        cout << "Could not find file." << endl;
        exit (1);
    }
    string transfer, line, id;
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
}

int main() {
    unordered_map<string, list<string>> adjList;
    dataFromMTA(adjList);
    dataFromTransfers(adjList);
    shortestpaths(adjList, "M01", "M08");
    return 0;

}