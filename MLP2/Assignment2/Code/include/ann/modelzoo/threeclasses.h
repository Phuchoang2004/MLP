/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   theeclasses.h
 * Author: ltsach
 *
 * Created on October 8, 2024, 8:44 PM
 */

#ifndef THEECLASSES_H
#define THEECLASSES_H
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

#include "sformat/fmt_lib.h"
#include "tensor/xtensor_lib.h"
#include "ann/annheader.h"
#include "loader/dataset.h"
#include "loader/dataloader.h"
#include "config/Config.h"
#include "dataset/DSFactory.h"
#include "optim/Adagrad.h"
#include "optim/Adam.h"

void threeclasses_classification(){
    try {
        DSFactory factory("./config.txt");
        xmap<string, TensorDataset<double, double>*>* pMap = factory.get_datasets_3cc();
        TensorDataset<double, double>* train_ds = pMap->get("train_ds");
        TensorDataset<double, double>* valid_ds = pMap->get("valid_ds");
        TensorDataset<double, double>* test_ds = pMap->get("test_ds");
        DataLoader<double, double> train_loader(train_ds, 50, true, false);
        DataLoader<double, double> valid_loader(valid_ds, 50, false, false);
        DataLoader<double, double> test_loader(test_ds, 50, false, false);

        int nClasses = 3;
        ILayer* layers[] = {
            new FCLayer(2, 50, true),
            new ReLU(),
            new FCLayer(50, 20, true),
            new ReLU(),
            new FCLayer(20, nClasses, true),
            new Softmax()
        };

        // Create the MLPClassifier model
        MLPClassifier model("./config.txt", "MLPClassifier", layers, 6);

        // Create the optimizer, loss function, and metrics
        Adam optimizer(0.001); // Adam optimizer with learning rate 0.001
        CrossEntropy loss_layer(REDUCE_MEAN); // Cross-entropy loss with mean reduction
        ClassMetrics metrics(nClasses); // Metrics for 3 classes

        // Compile the model with the optimizer, loss function, and metrics
        model.compile(&optimizer, &loss_layer, &metrics);

        // Train the model
        model.fit(&train_loader, &valid_loader, 10, 1); // Train for 10 epochs with verbosity level 1

        // Evaluate the model on test data
        double_tensor metrics_result = model.evaluate(&test_loader);
        cout << "Test Metrics: " << metrics_result << endl;
    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
}

#endif /* THEECLASSES_H */

