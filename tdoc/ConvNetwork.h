#pragma once
class ConvNetwork
{
private:
    int numLayers;
    double learningSpeed;
    std::vector <double> DesiredOutputMap;
    std::vector <std::vector <double>> InputMap;
public:
    // Initialization of Network;
    ConvNetwork(int numLayers);
    // Reading num of Layers from File;
    ConvNetwork();
    // Processing an output vector;
    void ProcessOutput(std::vector <double> OutputMap);
    // Getting InputMap from FILE;
    void getInput();
    // Getting Learning Parameters for BP from FILE;
    void getBPLearningParameters();
    // BP learning;
    void BackPropLearning(std::vector <double> DesiredOutputMap, double learningSpeed);
    double GradientNeuron();
    // Delete Network;
    ~ConvNetwork();


}