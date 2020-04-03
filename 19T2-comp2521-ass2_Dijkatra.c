#include <stdio.h> 
#include <stdlib.h>


#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

#define INFIN 99999

static PredNode *createNewPNode(Vertex vt);
static ItemPQ createNewItem(int k, int v);
static PredNode *addToPred(PredNode *n, Vertex pred);
 
ShortestPaths dijkstra(Graph g, Vertex src) {
    
    int numV = GraphNumVertices(g);
    //declare ShortestPath struct 
    ShortestPaths myPath;
    myPath.numNodes = numV;
    myPath.src = src; 
    
    //allocate memory for dist and pred array
    myPath.dist = malloc(numV*sizeof(int));
    myPath.pred = malloc(numV*sizeof(PredNode*));
    
    
    //initialize dist to infinity and pred to NULL 
    for (Vertex i = 0; i < numV; i++) {   
        
        myPath.pred[i]= NULL; 
        if (i == src) myPath.dist[i] = 0;
        else myPath.dist[i] = INFIN;
    }
    
    
    //initialise a PQ
    PQ myPQ = PQNew();
   
    //put the src node into PQ
    ItemPQ newItemPQ;
    newItemPQ.key = src;
    newItemPQ.value = 0;
    
    //add newItemOQ to PQ 
    PQAdd(myPQ, newItemPQ);
    
    //dist : to itself  = 0 
    myPath.dist[src] = 0; 
    
    // dikstra distances - myPath.dist[] is filled - use a visited Array inside here
    while (!PQIsEmpty(myPQ)) {
        //extract mini distance vertex from myPQ 
        ItemPQ extractedItem = PQDequeue(myPQ);
        // printf("extractedItem: key %d value %d \n", extractedItem.key, extractedItem.value);
        //extracted vertex
        Vertex u = extractedItem.key;  
        
        //get the head of the list 
        AdjList temp = GraphOutIncident(g, u);
        
        
        while (temp != NULL) {
            //get the adjacent vertex
            Vertex v = temp->v; 
            int w = temp->weight; 
            
           // printf("v : %d\n", v);
           // printf("path dist v: %d\n", myPath.dist[v]);
           // printf("path dist u + w : %d\n", myPath.dist[u] + w);
            
            // found a better path: new weight is smaller than the existing weight
            if (myPath.dist[u] + w < myPath.dist[v]) {   
                //update the distance of v
                myPath.dist[v] = myPath.dist[u] + w;
               
                //let u become the pred of v
                myPath.pred[v] = createNewPNode(u); 
                
                PQAdd(myPQ, createNewItem(v, w));
                //printf("myItem: key %d value %d \n", myItem.key, myItem.value);
            } else if (myPath.dist[u] + w == myPath.dist[v]) {
                //update preds; similiar to adding a node to the end of the linked list 
                myPath.pred[v] = addToPred(myPath.pred[v], (Vertex)u);
              
                PQAdd(myPQ, createNewItem(v, myPath.dist[v]));
            }
            
            temp = temp->next; 
        }
    }
   
    PQFree(myPQ);
   
   //change INFIN to 0: means vertex is not connected to another vertex
   for (int i = 0; i < numV; i++) {   
       
       if (myPath.dist[i] == INFIN) myPath.dist[i] = 0; 
   }
    
   return myPath; 
    
}

//this function creates a new predNode
static PredNode *createNewPNode(Vertex vt) {
   
    PredNode *newPNode = malloc(sizeof(PredNode));
    newPNode->v = vt;
    newPNode->next = NULL;
    return newPNode;
    
}

//this function creates a new Item
static ItemPQ createNewItem(int k, int v) {
    
    ItemPQ newItem; 
    newItem.key = k;
    newItem.value = v;
    return newItem; 
    
}

//this function links Prednode to the Pred array
static PredNode *addToPred(PredNode *n, Vertex pred) {
    //insert at the beginning of the linked list
    if (n == NULL || pred < n->v) {
        
        PredNode *newPNode = createNewPNode(pred);
        newPNode->next = n;
        return newPNode;
        
    } else if (pred > n->v) {
    
        //insert at last 
        n->next = addToPred(n->next, pred);
        return n;
        
    } else {
    
        return n;
    }
    
    return n; 
}

void showShortestPaths(ShortestPaths sps) {
    
    printf("--- TESTING showShortestPaths --- \n");
    
    printf("Distance\n");
    for (int i = 0; i < sps.numNodes; i++) {
    
        if (sps.dist[i] == INFIN) printf("%d: X\n", i);
        else printf("%d: %d\n", i, sps.dist[i]);
        
    }
    
    printf("Preds\n");
    for (int j = 0; j < sps.numNodes; j++) {
        
        if (sps.pred[j] == NULL) printf("%d : ", j);
        else {
            
            printf("%d : ", j);
            
            while (sps.pred[j] != NULL) {
                
                printf("[%d]->", sps.pred[j]->v);
                
                sps.pred[j] = sps.pred[j]->next; 
            }
            
        }
        
        printf("NULL \n");
    }
    
    
    
};


void freeShortestPaths(ShortestPaths sps)
{

    
    //free each dist node
    free(sps.dist);
    
    //free each pred node
    for (int j = 0; j < sps.numNodes; j++) {
        
        free(sps.pred[j]);
        
    }
};
