/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template<class T>
class DLinkedListSE: public DLinkedList<T> {
public:
    DLinkedListSE(
            void (*removeData)(DLinkedList<T>*)=0, 
            bool (*itemEQ)(T&, T&)=0 ) : 
            DLinkedList<T>(removeData, itemEQ){
        
    };
    
    DLinkedListSE(const DLinkedList<T>& list){
        this->copyFrom(list);
    }
    
    void sort(int (*comparator)(T&,T&)=0){
        if(this->count <= 1) return;
        mergeSort(this->head->next, this->tail->prev, comparator);
    };
    
protected:
    void mergeSort(typename DLinkedList<T>::Node* left, typename DLinkedList<T>::Node* right, int (*comparator)(T&,T&)){
        if(left == right) return;
        typename DLinkedList<T>::Node* middle = getMiddle(left, right);
        mergeSort(left, middle, comparator);
        mergeSort(middle->next, right, comparator);
        merge(left, middle, right, comparator);
    }
    
    typename DLinkedList<T>::Node* getMiddle(typename DLinkedList<T>::Node* left, typename DLinkedList<T>::Node* right){
        typename DLinkedList<T>::Node* slow = left;
        typename DLinkedList<T>::Node* fast = left->next;
        while(fast != right->next && fast != right){
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }
    
    void merge(typename DLinkedList<T>::Node* left, typename DLinkedList<T>::Node* middle, typename DLinkedList<T>::Node* right, int (*comparator)(T&,T&)){
        DLinkedList<T> tempList;
        typename DLinkedList<T>::Node* leftPtr = left;
        typename DLinkedList<T>::Node* rightPtr = middle->next;
        
        while(leftPtr != middle->next && rightPtr != right->next){
            if(compare(leftPtr->data, rightPtr->data, comparator) <= 0){
                tempList.add(tempList.size(), leftPtr->data);
                leftPtr = leftPtr->next;
            } else {
                tempList.add(tempList.size(), rightPtr->data);
                rightPtr = rightPtr->next;
            }
        }
        
        while(leftPtr != middle->next){
            tempList.add(tempList.size(), leftPtr->data);
            leftPtr = leftPtr->next;
        }
        
        while(rightPtr != right->next){
            tempList.add(tempList.size(), rightPtr->data);
            rightPtr = rightPtr->next;
        }
        
        typename DLinkedList<T>::Iterator tempIt = tempList.begin();
        typename DLinkedList<T>::Node* currentPtr = left;
        while(tempIt != tempList.end()){
            currentPtr->data = *tempIt;
            currentPtr = currentPtr->next;
            ++tempIt;
        }
    }
    
    int compare(T& lhs, T& rhs, int (*comparator)(T&,T&)){
        if(comparator != 0) return comparator(lhs, rhs);
        else{
            if(lhs < rhs) return -1;
            else if(lhs > rhs) return 1;
            else return 0;
        }
    }
};

#endif /* DLINKEDLISTSE_H */