int main() {
    // Configuration file path
    string cfg_filename = "config.txt";

    // Define the layers of the MLPClassifier
    ILayer* layers[] = {
        new FCLayer(784, 128, true), // Fully connected layer with 784 inputs and 128 outputs
        new ReLU(),                  // ReLU activation
        new FCLayer(128, 64, true),  // Fully connected layer with 128 inputs and 64 outputs
        new ReLU(),                  // ReLU activation
        new FCLayer(64, 10, true),   // Fully connected layer with 64 inputs and 10 outputs (for 10 classes)
        new Softmax()                // Softmax activation for classification
    };

    // Create the MLPClassifier model
    MLPClassifier model(cfg_filename, "MLPClassifier", layers, 6);

    // Create the optimizer, loss function, and metrics
    Adam optimizer(0.001); // Adam optimizer with learning rate 0.001
    CrossEntropy loss_layer(REDUCE_MEAN); // Cross-entropy loss with mean reduction
    ClassMetrics metrics(10); // Metrics for 10 classes

    // Compile the model with the optimizer, loss function, and metrics
    model.compile(&optimizer, &loss_layer, &metrics);

    // Create a DataLoader for training and validation data
    // Assuming you have a Dataset class that provides the data
    Dataset<double, double> train_dataset("path/to/train/data");
    Dataset<double, double> valid_dataset("path/to/valid/data");
    DataLoader<double, double> train_loader(&train_dataset, 32); // Batch size of 32
    DataLoader<double, double> valid_loader(&valid_dataset, 32); // Batch size of 32

    // Train the model
    model.fit(&train_loader, &valid_loader, 10, 1); // Train for 10 epochs with verbosity level 1

    // Save the trained model
    model.save("path/to/save/model");

    // Load the trained model
    model.load("path/to/save/model");

    // Evaluate the model on validation data
    double_tensor metrics_result = model.evaluate(&valid_loader);
    cout << "Validation Metrics: " << metrics_result << endl;

    return 0;
}