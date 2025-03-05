/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataset.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 3:59 PM
 */

#ifndef DATASET_H
#define DATASET_H
#include "tensor/xtensor_lib.h"
using namespace std;

template<typename DType, typename LType>
class DataLabel{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    DataLabel(xt::xarray<DType> data,  xt::xarray<LType> label):
    data(data), label(label){
    }
    xt::xarray<DType> getData() const{ return data; }
    xt::xarray<LType> getLabel() const{ return label; }
};

template<typename DType, typename LType>
class Batch {
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    Batch(): data(), label() {}
    
    Batch(xt::xarray<DType> data, xt::xarray<LType> label):
        data(data), label(label) {}
    
    Batch(const Batch& other): data(other.data), label(other.label) {}
    
    Batch& operator=(const Batch& other) {
        if (this != &other) {
            data = other.data;
            label = other.label;
        }
        return *this;
    }
    
    bool operator==(const Batch& other) const {
        if (data.shape() != other.data.shape() || label.shape() != other.label.shape()) {
            return false;
        }
        bool dataEqual = xt::all(xt::equal(data, other.data));
        bool labelEqual = xt::all(xt::equal(label, other.label));
        return dataEqual && labelEqual;
    }
    
    virtual ~Batch() {}
    
    // Add const-qualified methods
    const xt::xarray<DType>& getData() const { return data; }
    const xt::xarray<LType>& getLabel() const { return label; }
    
    // Keep non-const methods for when modification is needed
    xt::xarray<DType>& getData() { return data; }
    xt::xarray<LType>& getLabel() { return label; }
};


template<typename DType, typename LType>
class Dataset{
private:
public:
    Dataset(){};
    virtual ~Dataset(){};
    
    virtual int len()=0;
    virtual DataLabel<DType, LType> getitem(int index)=0;
    virtual xt::svector<unsigned long> get_data_shape()=0;
    virtual xt::svector<unsigned long> get_label_shape()=0;
    
};

//////////////////////////////////////////////////////////////////////
template<typename DType, typename LType>
class TensorDataset: public Dataset<DType, LType>{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;
public:
    /* TensorDataset: 
     * need to initialize:
     * 1. data, label;
     * 2. data_shape, label_shape
    */
    TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label){
        /* TODO: your code is here for the initialization
         */
        this->data = data;
        this->label = label;
        this->data_shape = xt::svector<unsigned long>(data.shape().begin(), data.shape().end());
        this->label_shape = xt::svector<unsigned long>(label.shape().begin(), label.shape().end());
    }
    /* len():
     *  return the size of dimension 0
    */
    int len() override {
        /* TODO: your code is here to return the dataset's length
         */
        return data_shape[0];
  
    }
    
    /* getitem:
     * return the data item (of type: DataLabel) that is specified by index
     */
    DataLabel<DType, LType> getitem(int index) override {
        /* TODO: your code is here
         */
        if(index >= this->len()||index<0){
            throw out_of_range("Index is out of range!");
        }
        xt::xarray<DType> item_data = xt::view(data, index);
        
        // If label is scalar (universal), return it as is
        if (label.dimension() == 0) {
            return DataLabel<DType, LType>(item_data, label);
        }
        // If label has dimensions and matches data shape
        else if (label_shape.size() > 0) {
            return DataLabel<DType, LType>(item_data, xt::view(label, index));
        }
        // Empty label case
        else {
            return DataLabel<DType, LType>(item_data, xt::xarray<LType>());
        }
    }
    
    xt::svector<unsigned long> get_data_shape() override {
        /* TODO: your code is here to return data_shape
         */
        return data_shape;
    }
    xt::svector<unsigned long> get_label_shape() override {
        /* TODO: your code is here to return label_shape
         */
        return label_shape;
    }
};

#endif /* DATASET_H */

