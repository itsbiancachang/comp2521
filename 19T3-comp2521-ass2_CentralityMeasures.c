#include <stdio.h>
#include <stdlib.h>


#include "CentralityMeasures.h"
#include "Dijkstra.h"


static int countAppearence(Vertex u, Vertex dest, ShortestPaths SP, int found);
static int countShortestPaths(Vertex dest, ShortestPaths SP);


NodeValues closenessCentrality(Graph g) {

    int numV = GraphNumVertices(g); 
    
    //declare a nodeValues struct 
    NodeValues CC;
    CC.numNodes = GraphNumVertices(g);
    CC.values = malloc(sizeof(double) * numV);
    
    //initialize an array to store the number of reachable node 
    int countReachable[numV];

    //initialize the values & countReachable array
    for (int i = 0; i < numV; i++) {
       
        CC.values[i] = 0;
        countReachable[i] = 0;
        
    }

    //calculate sigma d(u,v) 
    for (int u = 0; u < numV; u++) {
        
        ShortestPaths SP = dijkstra(g, u);   

        for (int v = 0; v < numV; v++) {
            
            //if u to v is reachable
            if (SP.dist[v] != 0 && u != v) { 
            
                CC.values[u] += SP.dist[v];
                countReachable[u]++;
                
            }
        }

        if (countReachable[u] != 0 && CC.values[u] != 0) {

            CC.values[u] = ((double)countReachable[u] / CC.values[u])* ((double)countReachable[u] / (numV - 1));  
        } 
    }

    return CC;
}


NodeValues betweennessCentrality(Graph g) {
    
    int numV = GraphNumVertices(g);
    
    //declare a nodeValues struct 
    NodeValues BC;
    BC.numNodes = GraphNumVertices(g);
    BC.values = malloc(sizeof(double) * numV);
    
    //initialize the value array
    for (int i = 0; i < numV; i++) {
    
        BC.values[i] = 0;
        
    }


    //for each vertex in this graph
    for (int u = 0; u < numV; u++) {
    //count the num of appearence of u 
        for (int src = 0; src < numV; src++) {
            //get the shortest path from src to all other verteices 
            ShortestPaths SP = dijkstra(g, src);
            
            for (int dest = 0; dest < numV; dest++) {
                //count the betweennessCentrality for u
                if (u != src && src != dest && u != dest) {
                    
                    double centrality = 0; 
                    
                    if (u == SP.src || u == dest) centrality = 0; 
                    
                    int numOfAppearence = countAppearence(u, dest, SP, 0);
                    int numOfShortestPath = countShortestPaths(dest, SP);
                    
                    if (numOfAppearence == 0 || numOfShortestPath == 0) {
                    
                        centrality = 0;
                        
                    } else {
                    
                        centrality = ((double)numOfAppearence/(double)numOfShortestPath);
                        
                    }
                    
                    
                    BC.values[u] += centrality; 
                }
            }
            
            freeShortestPaths(SP);
            
        } 

        
    }
    
    return BC;
}


NodeValues betweennessCentralityNormalised(Graph g) {
    
    NodeValues BC = betweennessCentrality(g);
    int numV = GraphNumVertices(g);
    
    //declare a nodeValues struct 
    NodeValues BCN;
    BCN.numNodes = numV; 
    BCN.values = malloc(numV * sizeof(double));
    
    //initialze the value array 
    for (int i = 0; i < numV; i++) {
       
        BCN.values[i] = 0;
        
    }

    //avoid zero as denominator (must n>2).
    if (numV < 2) abort();
    else {
        
        for (int i = 0; i < numV; i++) {
        
            BCN.values[i] = BC.values[i] / (((double)numV - 1)*((double)numV-2));
            
        }
       
    }

    return BCN;  
}


//this function counts the num of shortest path to dest 
static int countShortestPaths(Vertex dest, ShortestPaths SP) {
    
    if (dest == SP.src) return 1;

    PredNode *temp = SP.pred[dest];

    
    int sum = 0;

    while (temp != NULL)  {
          
        sum += countShortestPaths(temp->v, SP);
        temp = temp->next; 
        
    }

    return sum;
}


//count the num of apperence of u 
static int countAppearence(Vertex u, Vertex dest, ShortestPaths SP, int found) {

    //no need to count anymore 
    if (u == dest) found = 1;
    if (dest == SP.src) return found;
    
    int count = 0;

    PredNode *temp = SP.pred[dest];
    
    //there might be some shortest paths - let's find 
    while (temp != NULL)  {

        count += countAppearence(u, temp->v, SP, found);
        temp = temp->next;
    }

    return count;
    
}



void showNodeValues(NodeValues nvs) {
  
    for (int i = 0; i < nvs.numNodes; i++) {
       
        printf("%d: %lf\n", i, nvs.values[i]); 
        
    }
}



void freeNodeValues(NodeValues nvs) {

    free(nvs.values);
    
}




