/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"
#include "sorting/DLinkedListSE.h"

//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
class DGraphModel: public AbstractGraph<T> {
public:
    DGraphModel(
            bool (*vertexEQ)(T&, T&), 
            string (*vertex2str)(T&) ): 
        AbstractGraph<T>(vertexEQ, vertex2str){
    }
    
    void connect(T from, T to, float weight=0) {
        typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(to);
        if (!fromNode) throw VertexNotFoundException(this->vertex2str(from));
        if (!toNode) throw VertexNotFoundException(this->vertex2str(to));
        fromNode->connect(toNode, weight);
    }
    
    void disconnect(T from, T to) {
        typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(to);
        if (!fromNode) throw VertexNotFoundException(this->vertex2str(from));
        if (!toNode) throw VertexNotFoundException(this->vertex2str(to));
        
        typename AbstractGraph<T>::Edge* edge = fromNode->getEdge(toNode);
        if (!edge) throw EdgeNotFoundException(this->edge2Str(*edge));
        
        fromNode->removeTo(toNode); // This handles edge deletion
    }
    
    void remove(T vertex) {
        typename AbstractGraph<T>::VertexNode* node = this->getVertexNode(vertex);
        if (!node) throw VertexNotFoundException(this->vertex2str(vertex));
        
        // First remove all edges to/from this vertex
        typename DLinkedList<typename AbstractGraph<T>::VertexNode*>::Iterator it = this->nodeList.begin();
        while(it != this->nodeList.end()) {
            typename AbstractGraph<T>::VertexNode* currentNode = *it;
            if(currentNode != node) {
                currentNode->removeTo(node);
                node->removeTo(currentNode);
            }
            it++;
        }
        
        // Then remove the vertex itself
        this->nodeList.removeItem(node);
        delete node;
    }
    
    static DGraphModel<T>* create(
            T* vertices, int nvertices, Edge<T>* edges, int nedges,
            bool (*vertexEQ)(T&, T&),
            string (*vertex2str)(T&)) {
        DGraphModel<T>* graph = new DGraphModel<T>(vertexEQ, vertex2str);

        for(int idx = 0; idx < nvertices; idx++)
            graph->add(vertices[idx]);
        
        for(int idx = 0; idx < nedges; idx++){
            Edge<T>& edge = edges[idx];
            graph->connect(edge.from, edge.to, edge.weight);
        }
        return graph;
    }
};

#endif /* DGRAPHMODEL_H */