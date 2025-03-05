/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <iostream>
#include <sstream>
#include <type_traits>

#include "list/IList.h"
using namespace std;

template <class T>
class DLinkedList : public IList<T> {
 public:
  class Node;         // Forward declaration
  class Iterator;     // Forward declaration
  class BWDIterator;  // Forward declaration

 protected:
  Node *head;
  Node *tail;
  int count;
  bool (*itemEqual)(T &lhs, T &rhs);
  void (*deleteUserData)(DLinkedList<T> *);

 public:
  DLinkedList(void (*deleteUserData)(DLinkedList<T> *) = 0,
              bool (*itemEqual)(T &, T &) = 0);
  DLinkedList(const DLinkedList<T> &list);
  DLinkedList<T> &operator=(const DLinkedList<T> &list);
  ~DLinkedList();

  // Inherit from IList: BEGIN
  void add(T e);
  void add(int index, T e);
  T removeAt(int index);
  bool removeItem(T item, void (*removeItemData)(T) = 0);
  bool empty();
  int size();
  void clear();
  T &get(int index);
  int indexOf(T item);
  bool contains(T item);
  string toString(string (*item2str)(T &) = 0);
  // Inherit from IList: END

  void println(string (*item2str)(T &) = 0) {
    cout << toString(item2str) << endl;
  }
  void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0) {
    this->deleteUserData = deleteUserData;
  }

  bool contains(T array[], int size) {
    int idx = 0;
    for (DLinkedList<T>::Iterator it = begin(); it != end(); it++) {
      if (!equals(*it, array[idx++], this->itemEqual)) return false;
    }
    return true;
  }

  Iterator begin() { return Iterator(this, true); }
  Iterator end() { return Iterator(this, false); }

  BWDIterator bbegin() { return BWDIterator(this, true); }
  BWDIterator bend() { return BWDIterator(this, false); }

 protected:
  void copyFrom(const DLinkedList<T> &list);
  void removeInternalData();
  Node *getPreviousNodeOf(int index);

  //! FUNTION STATIC
 public:
  static void free(DLinkedList<T> *list) {
    typename DLinkedList<T>::Iterator it = list->begin();
    while (it != list->end()) {
      delete *it;
      it++;
    }
  }

 protected:
  static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &)) {
    if (itemEqual == 0)
      return lhs == rhs;
    else
      return itemEqual(lhs, rhs);
  }

 public:
  class Node {
   public:
    T data;
    Node *next;
    Node *prev;
    friend class DLinkedList<T>;

   public:
    Node(Node *next = 0, Node *prev = 0) {
      this->next = next;
      this->prev = prev;
    }
    Node(T data, Node *next = 0, Node *prev = 0) {
      this->data = data;
      this->next = next;
      this->prev = prev;
    }
  };

 public:
  class Iterator {
   private:
    DLinkedList<T> *pList;
    Node *pNode;

   public:
    Iterator(DLinkedList<T> *pList = 0, bool begin = true) {
      if (begin) {
        if (pList != 0)
          this->pNode = pList->head->next;
        else
          pNode = 0;
      } else {
        if (pList != 0)
          this->pNode = pList->tail;
        else
          pNode = 0;
      }
      this->pList = pList;
    }

    Iterator &operator=(const Iterator &iterator) {
      this->pNode = iterator.pNode;
      this->pList = iterator.pList;
      return *this;
    }
    void remove(void (*removeItemData)(T) = 0) {
      pNode->prev->next = pNode->next;
      pNode->next->prev = pNode->prev;
      Node *pNext = pNode->prev;  // MUST prev, so iterator++ will go to end
      if (removeItemData != 0) removeItemData(pNode->data);
      delete pNode;
      pNode = pNext;
      pList->count -= 1;
    }

    T &operator*() { return pNode->data; }
    bool operator!=(const Iterator &iterator) {
      return pNode != iterator.pNode;
    }
    // Prefix ++ overload
    Iterator &operator++() {
      pNode = pNode->next;
      return *this;
    }
    // Postfix ++ overload
    Iterator operator++(int) {
      Iterator iterator = *this;
      ++*this;
      return iterator;
    }
  };

  class BWDIterator {
    // TODO implement
    private:
    DLinkedList<T> *pList; 
    Node *currentNode;   

   public:
    // Constructor
    BWDIterator(DLinkedList<T> *pList = 0, bool begin = true) {
      this->pList = pList;
      if (pList != 0) {
        if (begin) {
          this->currentNode = pList->tail->prev; 
        } else {
          this->currentNode = pList->head;  
        }
      } else {
        this->currentNode = 0;  
      }
    }

    BWDIterator &operator=(const BWDIterator &iterator) {
      this->pList = iterator.pList;
      this->currentNode = iterator.currentNode;
      return *this;
    }

    T &operator*() {
      return currentNode->data;
    }
    bool operator!=(const BWDIterator &iterator) {
      return this->currentNode != iterator.currentNode;
    }

    BWDIterator &operator--() {
      if (currentNode != pList->head) { 
        currentNode = currentNode->prev;
      }
      return *this;
    }
    BWDIterator operator--(int) {
      BWDIterator temp = *this;  
      --(*this);                
      return temp;              
    }
    BWDIterator &operator++() {
      if (currentNode != pList->head) { 
        currentNode = currentNode->prev;
      }
      return *this;
    }
    BWDIterator operator++(int) {
      BWDIterator temp = *this;  
      ++(*this);                
      return temp;              
    }
    void remove(void (*removeItemData)(T) = 0) {
      if (currentNode == pList->head || currentNode == pList->tail) {
        return;
      }
      Node *pNext = currentNode->next;
    currentNode->prev->next = currentNode->next;
    currentNode->next->prev = currentNode->prev;
    if (removeItemData != 0) {
        removeItemData(currentNode->data);  
    }
    delete currentNode;  
    currentNode = pNext; 
    pList->count -= 1;
    }
  };
};

template <class T>
using List = DLinkedList<T>;

//! ////////////////////////////////////////////////////////////////////
//! //////////////////////     METHOD DEFNITION      ///////////////////
//! ////////////////////////////////////////////////////////////////////
template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    // TODO
    this->head = new Node();
    this->tail = new Node();
    this->head->next=this->tail;
    this->tail->prev=this->head;
    this->head->prev = this->tail;
    this->tail->next = this->head;
    this->count=0;
    this->deleteUserData=deleteUserData;
    this->itemEqual=itemEqual;  
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // TODO
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->head->prev = this->tail;
    this->tail->next = this->head;
    this->count = 0;
    copyFrom(list);
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    // TODO
    if (this != &list)
    {
        copyFrom(list);
    }
    return *this;

}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    // TODO
    clear();
    delete this->head;
    delete this->tail;
}

template <class T>
void DLinkedList<T>::add(T e)
{
    // TODO
    Node* newNode=new Node(e);
    newNode->prev=tail->prev;
    newNode->next=tail;
    tail->prev->next=newNode;
    tail->prev=newNode;
    count++;
}
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    // TODO
    if(index<0||index>count){
        throw out_of_range("Index is out of range!");
    }
    else{
        if(index==0 && count==0){
            add(e);
        }
        else if(index==0){
            Node* newNode=new Node(e);
            newNode->next=head->next;
            newNode->next->prev=newNode;
            head->next=newNode;
            newNode->prev=head;
            count++;
        }
        else if(index==count){
            add(e);
        }
        else {
            Node* newNode=new Node(e);
            Node* prevNode=getPreviousNodeOf(index);
            newNode->prev=prevNode;
            newNode->next=prevNode->next;
            prevNode->next->prev=newNode;
            prevNode->next=newNode;
            count++;
        }
    }
    
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    // TODO
    if(index<0||index>=count){
        throw out_of_range("Index is out of range!");
    }
    if(count==0){
        throw out_of_range("Index is out of range!");
    }
    Node* prevNode=getPreviousNodeOf(index);
    Node* CutRaNgoai=prevNode->next;

    prevNode->next=CutRaNgoai->next;
    CutRaNgoai->next->prev=prevNode;

    CutRaNgoai->next=nullptr;
    CutRaNgoai->prev=nullptr;
    T QuocHonQuocTuy =CutRaNgoai->data;
    delete CutRaNgoai;
    count--;
    return QuocHonQuocTuy;

}

template <class T>
bool DLinkedList<T>::empty()
{
    // TODO
    return count==0;
}

template <class T>
int DLinkedList<T>::size()
{
    // TODO
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    // TODO
  Node* current = head->next;
  Node* nextNode;
   
   while (current != tail) {
       nextNode = current->next;
       delete current;
       current = nextNode;
   }

   head->next = tail;
   tail->prev = head;
   count = 0;
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    // TODO
    if(index<0||index>count){
        throw out_of_range("Index is out of range!");
    }
    if(count==0){
       throw out_of_range("Index is out of range!");
    }
    Node* prevNode=getPreviousNodeOf(index);
    return prevNode->next->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    // TODO
    int index=0;
    for(Node* current=head->next;current!=tail;current=current->next){
        if(equals(current->data,item,itemEqual)){
            return index;
        }
        index++;
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    // TODO
    int index=indexOf(item);
    if(index !=-1){
        T ToRemove = removeAt(index);
        if(removeItemData){
          removeItemData(ToRemove);
        }
        return true;
    }
    return false;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    // TODO
    if(indexOf(item)!=-1){
        return true;
    }
    return false;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    // TODO
    stringstream ss;
    ss << "[";
    Node *current = head->next;
    while (current != tail) {
        if (item2str) {
            ss << item2str(current->data);
        } else {
            ss << current->data;
        }
        if (current->next != tail) {
            ss << ", ";
        }
        current = current->next;
    }
    ss << "]";
    return ss.str();
}

//! ////////////////////////////////////////////////////////////////////
//! ////////////////////// (private) METHOD DEFNITION //////////////////
//! ////////////////////////////////////////////////////////////////////
template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list) {
  /**
   * Copies the contents of another doubly linked list into this list.
   * Initializes the current list to an empty state and then duplicates all data
   * and pointers from the source list. Iterates through the source list and
   * adds each element, preserving the order of the nodes.
   */
  // TODO implement
  

  delete this->head;
  delete this->tail;

    this->head = new Node();
    this->tail = new Node();
    this->head->next=this->tail;
    this->tail->prev=this->head;
    this->head->prev = this->tail;
    this->tail->next = this->head;
    this->count=0;
    this->deleteUserData=list.deleteUserData;
    this->itemEqual=list.itemEqual;
    for (Node *current = list.head->next; current != list.tail; current = current->next)
    {
        add(current->data);
    }
}

template <class T>
void DLinkedList<T>::removeInternalData() {
  /**
   * Clears the internal data of the list by deleting all nodes and user-defined
   * data. If a custom deletion function is provided, it is used to free the
   * user's data stored in the nodes. Traverses and deletes each node between
   * the head and tail to release memory.
   */
  // TODO implement
  if (deleteUserData){
        deleteUserData(this);
    }
   this->clear();
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index) {
  /**
   * Returns the node preceding the specified index in the doubly linked list.
   * If the index is in the first half of the list, it traverses from the head;
   * otherwise, it traverses from the tail. Efficiently navigates to the node by
   * choosing the shorter path based on the index's position.
   */
  // TODO implement
  if(index<0||index>count){
        cout<<"count: "<<count<<endl;
        throw out_of_range("Index is out of range!");
    }

    Node* current=head;
    for(int i=0;i<index;i++){
        current=current->next;
    }
    return current;
}

#endif /* DLINKEDLIST_H */
