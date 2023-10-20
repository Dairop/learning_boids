#pragma once
#include "Layer.hpp"



class NeuralNetwork {
    std::vector <Layer> Layers;
    std::vector <float> finalValues;


public:
    NeuralNetwork(){
        // the sizes of each Layer are defined manually
        // 
        // For the parameters (x1, y1) (x2, y2), ..., (xn, yn) with n the index of the Layer,
        //
        //     yn = x(n-1)
        //     (x1, y1) = (5, 0) and x(n_max) = 4


        //simple networks, fast learning but not very complex
        /*
        Layers.push_back(Layer(5, 0));
        Layers.push_back(Layer(6, 5));
        Layers.push_back(Layer(4, 6));
        */



        //longer learning but in theory better
        
        Layers.push_back(Layer(5, 0));
        Layers.push_back(Layer(9, 5));        
        Layers.push_back(Layer(7, 9));
        Layers.push_back(Layer(9, 7));
        Layers.push_back(Layer(6, 9));
        Layers.push_back(Layer(4, 6));
        
    }

    void copy_NN(const NeuralNetwork& r2);
    void randomlyMutate(const float MUTATION_PROB); //0.10 randomly modifies 10% of weights and biases in the network

    void update();
    void setInputValues(std::vector<float>& vals);
    
    void getLayers(std::vector <Layer>& vecLayers) const;

    std::vector<float> getfinalValues() const { return this->finalValues; }
    float getfinalValue(int i) const { return this->finalValues[i]; }

    //calculate the difference between 2 networks. The smaller the value, the closer they are “genetically”
    float geneticDistanceWith(const NeuralNetwork& RN2) const;
};