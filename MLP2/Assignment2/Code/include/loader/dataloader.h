/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template<typename DType, typename LType>
std::ostream& operator<<(std::ostream& os, const Batch<DType, LType>& batch) {
    os  <<xt::adapt(batch.getData().shape())<<xt::adapt(batch.getLabel().shape());
    return os;
}

template <typename DType, typename LType>
class DataLoader {
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    XArrayList<Batch<DType, LType>> batches;
    int total_batches;
    unsigned int seed;
public:
    class Iterator;
    //New method: from V2: begin
    int get_batch_size(){ return batch_size; }
    int get_sample_count(){ return ptr_dataset->len(); }
    int get_total_batch(){return int(ptr_dataset->len()/batch_size); }
    DataLoader(Dataset<DType, LType>* ptr_dataset, int batch_size,
               bool shuffle = true, bool drop_last = false, int seed=-1) 
        : ptr_dataset(ptr_dataset), batch_size(batch_size), 
          shuffle(shuffle), drop_last(drop_last), seed(seed) {
        
        int dataset_size = ptr_dataset->len();
        xt::xarray<int> indices = xt::arange<int>(dataset_size);
        if(seed >=0){
            xt::random::seed(seed);
        }
        if (shuffle) {
            xt::random::shuffle(indices);
        }
        
        int num_complete_batches = dataset_size / batch_size;
        total_batches = num_complete_batches;

        // Get shapes from the dataset
        auto data_shape = ptr_dataset->get_data_shape();
        auto label_shape = ptr_dataset->get_label_shape();
        bool has_labels = !label_shape.empty();
        
        for (int b = 0; b < total_batches; b++) {
            int start_idx = b * batch_size;
            int end_idx = std::min(start_idx + batch_size, dataset_size);
            if(!drop_last&& b == total_batches-1){
                end_idx=dataset_size;
            }
            int current_batch_size = end_idx - start_idx;
            
             // Prepare shapes for the current batch
            xt::svector<unsigned long> batch_data_shape = data_shape;
            batch_data_shape[0] = current_batch_size;
            
            // Create batch data container
            xt::xarray<DType> batch_data = xt::empty<DType>(batch_data_shape);
            
            // Create batch label container only if we have labels
            xt::xarray<LType> batch_labels;
            if (has_labels) {
                xt::svector<unsigned long> batch_label_shape = label_shape;
                batch_label_shape[0] = current_batch_size;
                batch_labels = xt::empty<LType>(batch_label_shape);
            }
            
            // Fill batch
            for (int i = 0; i < current_batch_size; i++) {
                int idx = indices[start_idx + i];
                auto item = ptr_dataset->getitem(idx);
                
                // Copy data
                xt::view(batch_data, i) = item.getData();
                
                // Copy label only if we have labels
                if (has_labels) {
                    xt::view(batch_labels, i) = item.getLabel();
                }
            }
            
            batches.add(Batch<DType, LType>(batch_data, batch_labels));
        }
    }
    
    virtual ~DataLoader(){}

    Iterator begin() {
        return Iterator(this, 0);
    }
    
    Iterator end() {
        return Iterator(this, batches.size());
    }
    
    class Iterator {
    private:
        DataLoader* loader;
        int current;
        
    public:
        Iterator(DataLoader* loader, int current) 
            : loader(loader), current(current) {}
        
        Iterator& operator=(const Iterator& iterator) {
            loader = iterator.loader;
            current = iterator.current;
            return *this;
        }
        
        Iterator& operator++() {
            ++current;
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            ++current;
            return temp;
        }
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
        
        Batch<DType, LType>& operator*() {
            return loader->batches.get(current);
        }
    };
};

#endif /* DATALOADER_H */
