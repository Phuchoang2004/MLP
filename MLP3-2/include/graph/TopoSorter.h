/*
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"

template<class T>
class TopoSorter {
public:
    static int DFS;
    static int BFS; 
    
protected:
    DGraphModel<T>* graph;
    int (*hash_code)(T&, int);
    
public:
    TopoSorter(DGraphModel<T>* graph, int (*hash_code)(T&, int)=0){
        this->graph = graph;
        this->hash_code = hash_code;
    }   
    DLinkedList<T> sort(int mode=0, bool sorted=true){
        if(mode == DFS) return dfsSort(sorted);
        else return bfsSort(sorted);
    }
    DLinkedList<T> bfsSort(bool sorted=true){ 
        DLinkedListSE<T> zeroInDegreeList = listOfZeroInDegrees();
        if(sorted) zeroInDegreeList.sort();
        
        Queue<T> queue;
        for(auto it = zeroInDegreeList.begin(); it != zeroInDegreeList.end(); it++){
            queue.push(*it);
        }
        
        DLinkedList<T> topoOrder;
        xMap<T, int> inDegreeMap = vertex2inDegree(hash_code);
        
        while(!queue.empty()){
            T vertex = queue.pop();
            topoOrder.add(topoOrder.size(), vertex);
            
            DLinkedList<T> outwardEdges = graph->getOutwardEdges(vertex);
            for(auto it = outwardEdges.begin(); it != outwardEdges.end(); it++){
                T adjVertex = *it;
                int inDegree = inDegreeMap.get(adjVertex);
                inDegreeMap.put(adjVertex, inDegree - 1);
                if(inDegree - 1 == 0){
                    queue.push(adjVertex);
                }
            }
        }
        
        return topoOrder;
    }
    DLinkedList<T> dfsSort(bool sorted=true){
        DLinkedListSE<T> vertices = graph->vertices();
        if(sorted) vertices.sort();
        
        DLinkedList<T> topoOrder;
        xMap<T, bool> visited(hash_code);
        
        for(auto it = vertices.begin(); it != vertices.end(); it++){
            if(!visited.containsKey(*it)){
                dfsVisit(*it, visited, topoOrder);
            }
        }
        
        return topoOrder;
    }
protected:
    void dfsVisit(T vertex, xMap<T, bool>& visited, DLinkedList<T>& topoOrder){
        visited.put(vertex, true);
        
        DLinkedList<T> outwardEdges = graph->getOutwardEdges(vertex);
        for(auto it = outwardEdges.begin(); it != outwardEdges.end(); it++){
            T adjVertex = *it;
            if(!visited.containsKey(adjVertex)){
                dfsVisit(adjVertex, visited, topoOrder);
            }
        }
        topoOrder.add(0, vertex);
    }
    xMap<T, int> vertex2inDegree(int (*hash)(T&, int)){
        xMap<T, int> inDegreeMap(hash);
        DLinkedList<T> vertices = graph->vertices();
        for(auto it = vertices.begin(); it != vertices.end(); it++){
            inDegreeMap.put(*it, 0);
        }
        
        for(auto it = vertices.begin(); it != vertices.end(); it++){
            DLinkedList<T> outwardEdges = graph->getOutwardEdges(*it);
            for(auto edgeIt = outwardEdges.begin(); edgeIt != outwardEdges.end(); edgeIt++){
                T adjVertex = *edgeIt;
                int inDegree = inDegreeMap.get(adjVertex);
                inDegreeMap.put(adjVertex, inDegree + 1);
            }
        }
        
        return inDegreeMap;
    }
    DLinkedList<T> listOfZeroInDegrees(){
        DLinkedList<T> zeroInDegreeList;
        xMap<T, int> inDegreeMap = vertex2inDegree(hash_code);
        
        DLinkedList<T> vertices = graph->vertices();
        for(auto it = vertices.begin(); it != vertices.end(); it++){
            if(inDegreeMap.get(*it) == 0){
                zeroInDegreeList.add(zeroInDegreeList.size(), *it);
            }
        }
        
        return zeroInDegreeList;
    }
};

template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

#endif /* TOPOSORTER_H */