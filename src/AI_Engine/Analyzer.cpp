#include "Analyzer.hpp"


bool do_complete_analysis = true;



// Study behaviors frequences with random input values
void mostCommonBehavior(NeuralNetwork& nn) {
    std::vector<float> departure(5);

    float shareCount = 0, reproCount = 0, fightCount = 0, ignoreCount = 0;

    for (unsigned int i = 0; i < 10000; i++) {
        departure.clear();
        for (unsigned int j = 0; j < 5; j++) {
            departure.push_back(rand3() / 3.0f);
        }

        nn.setInputValues(departure);

        nn.update();


        float shareScore = nn.getfinalValue(0);
        float reproScore = nn.getfinalValue(1);
        float fightScore = nn.getfinalValue(2);
        float ignoreScore = nn.getfinalValue(3);

        float max = shareScore;
        short unsigned int caseValue = 1;
        if (reproScore > max) { max = reproScore; caseValue = 2; }
        if (fightScore > max) { max = fightScore; caseValue = 3; }
        if (ignoreScore > max) { max = ignoreScore; caseValue = 0; }

        switch (caseValue)
        {
        case 1:
            shareCount++;
            break;

        case 2:
            reproCount++;
            break;

        case 3:
            fightCount++;
            break;

        default:
            ignoreCount++;
            break;
        }
    }


    std::cout << "Share Probability: " << shareCount / 100.0f << "%  Reproduce Probability: " << reproCount / 100.0f
        << "%  Fight Probability: " << fightCount / 100.0f << "%  Ignore Probability: " << ignoreCount / 100.0f << "%\n";

}











void behaviorScores(NeuralNetwork& nn, int parameter) {
    std::vector<float> shareVector;
    std::vector<float> reproduceVector;
    std::vector<float> fightVector;
    std::vector<float> ignoreVector;

    std::vector<float> departure(5); // Values that will be input into each network

    std::string xAxis;

    float testedValue, maxValue;

    if (parameter == 4) { // Testing genetic distance
        testedValue = 0.0f;
        maxValue = 400.0f;
        xAxis = "genetic distance";
    }
    else if (parameter == 0) { // Relative to the age of the studied individual
        testedValue = 0.0f;
        maxValue = 100.0f;
        xAxis = "age";
    }
    else if (parameter == 1) { // Relative to the age of the other studied individual
        testedValue = 0.0f;
        maxValue = 100.0f;
        xAxis = "age of other individual";
    }
    else if (parameter == 2) { // Relative to the food reserves of the studied individual
        testedValue = 0.0f;
        maxValue = 100.0f;
        xAxis = "food reserves";
    }
    else if (parameter == 3) { // Relative to the food reserves of the other studied individual
        testedValue = 0.0f;
        maxValue = 100.0f;
        xAxis = "food reserves of other individual";
    }

    if (maxValue == 0) return;

    float precision = 1000; // Number of bars on the final graph (precision)
    float numIterations = 1000; // Number of iterations for each bar, helps mitigate randomness through averaging

    for (float bar = 0; bar < precision; bar++) {
        float totalShare = 0;
        float totalReproduce = 0;
        float totalFight = 0;
        float totalIgnore = 0;

        // Repeat the calculation 100 times with different values to reduce randomness
        for (unsigned int iter = 0; iter < numIterations; iter++) {
            testedValue = bar / precision * maxValue;

            departure.clear();

            if (parameter == 4) { // Genetic distance
                for (unsigned int j = 0; j < 4; j++) {
                    departure.push_back(rand3() / 3.0f);
                }
                departure.push_back(testedValue / 200.0f - 1.0f); // Change the calculation if it's modified in Engine.cpp

            }
            else if (parameter == 0) {
                departure.push_back((testedValue - 50) / 50.0f); // Change the calculation if it's modified in Engine.cpp
                for (unsigned int j = 0; j < 4; j++) {
                    departure.push_back(rand3() / 3.0f);
                }
            }
            else if (parameter == 1) {
                departure.push_back(rand3() / 3.0f);
                departure.push_back((testedValue - 50) / 50.0f); // Change the calculation if it's modified in Engine.cpp
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);
            }
            else if (parameter == 2) {
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);
                departure.push_back((testedValue - 50) / 50.0f); // Change the calculation if it's modified in Engine.cpp
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);
            }
            else if (parameter == 3) {
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);
                departure.push_back((testedValue - 50) / 50.0f); // Change the calculation if it's modified in Engine.cpp
                departure.push_back(rand3() / 3.0f);
            }

            nn.setInputValues(departure);

            nn.update();

            totalShare += nn.getfinalValue(0) + 1;
            totalReproduce += nn.getfinalValue(1) + 1;
            totalFight += nn.getfinalValue(2) + 1;
            totalIgnore += nn.getfinalValue(3) + 1;
        }

        float total;
        if (true) { // If true, represent each score on the graph as a percentage of the total, not absolute value
            total = totalShare + totalReproduce + totalFight + totalIgnore;
        }
        else {
            total = 1;
        }

        shareVector.push_back(totalShare / total);
        reproduceVector.push_back(totalReproduce / total);
        fightVector.push_back(totalFight / total);
        ignoreVector.push_back(totalIgnore / total);
    }

    if (false) { // Divide by the number of iterations
        for (unsigned int j = 0; j < numIterations; j++) {
            shareVector[j] /= numIterations;
            reproduceVector[j] /= numIterations;
            fightVector[j] /= numIterations;
            ignoreVector[j] /= numIterations;
        }
    }

    shareVector = movingAverage(shareVector, 10);
    fightVector = movingAverage(fightVector, 10);
    ignoreVector = movingAverage(ignoreVector, 10);
    reproduceVector = movingAverage(reproduceVector, 10);

    std::string fileName = "scores as a function of " + xAxis;

   saveAsSvgWith4VectorsOneGraph(fileName, shareVector, reproduceVector, fightVector, ignoreVector, xAxis, "frequency", "Blue: Share | Purple: Reproduce | Red: Fight | Green: Ignore", maxValue);
}














void favoriteBehavior(NeuralNetwork& nn, int parameter) {
    std::vector<float> shareVector;
    std::vector<float> reproduceVector;
    std::vector<float> fightVector;
    std::vector<float> ignoreVector;

    std::vector<float> departure(5); // Values that will be input into each network

    std::string xAxis;

    float testedValue, maxValue;

    if (parameter == 4) { // Testing genetic distance
        testedValue = 0.0f;
        maxValue = 400.0f;
        xAxis = "genetic distance";
    }
    else if (parameter == 0) { // Relative to the age of the studied individual
        testedValue = 0.0f;
        maxValue = 100.0f;
        xAxis = "age";
    }
    else if (parameter == 1) { // Relative to the age of the other studied individual
        testedValue = 0.0f;
        maxValue = 100.0f;
        xAxis = "age of other individual";
    }
    else if (parameter == 2) { // Relative to the food reserves of the studied individual
        testedValue = 0.0f;
        maxValue = 100.0f;
        xAxis = "food reserves";
    }
    else if (parameter == 3) { // Relative to the food reserves of the other studied individual
        testedValue = 0.0f;
        maxValue = 100.0f;
        xAxis = "food reserves of other individual";
    }

    if (maxValue == 0) return;

    float precision = 400; // Number of bars on the final graph (precision)
    float numIterations = 500; // Number of iterations for each bar, helps mitigate the effect of randomness through averaging

    for (float bar = 0; bar < precision; bar++) {
        float totalShare = 0;
        float totalReproduce = 0;
        float totalFight = 0;
        float totalIgnore = 0;

        // Repeat the calculation 100 times with different values to reduce randomness
        for (unsigned int iter = 0; iter < numIterations; iter++) {
            testedValue = bar / precision * maxValue;

            departure.clear();

            if (parameter == 0) {
                departure.push_back((testedValue - 50) / 50.0f); // Change the calculation if it's modified in Engine.cpp
                for (unsigned int j = 0; j < 4; j++) {
                    departure.push_back(rand3() / 3.0f);
                }

            }
            else if (parameter == 1) {
                departure.push_back(rand3() / 3.0f);
                departure.push_back((testedValue - 50) / 50.0f); // Change the calculation if it's modified in Engine.cpp
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);

            }
            else if (parameter == 2) {
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);
                departure.push_back((testedValue - 50) / 50.0f); // Change the calculation if it's modified in Engine.cpp
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);

            }
            else if (parameter == 3) {
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);
                departure.push_back(rand3() / 3.0f);
                departure.push_back((testedValue - 50) / 50.0f); // Change the calculation if it's modified in Engine.cpp
                departure.push_back(rand3() / 3.0f);

            }
            else if (parameter == 4) { // Genetic distance
                for (unsigned int j = 0; j < 4; j++) {
                    departure.push_back(rand3() / 3.0f);
                }
                departure.push_back(testedValue / 200.0f - 1.0f); // Change the calculation if it's modified in Engine.cpp

            }

            nn.setInputValues(departure);

            nn.update();

            float max = nn.getfinalValue(0); // Score of the current choice
            unsigned int caseValue = 1; // Choice of the network
            if (nn.getfinalValue(1) > max) { max = nn.getfinalValue(1); caseValue = 2; }
            if (nn.getfinalValue(2) > max) { max = nn.getfinalValue(2); caseValue = 3; }
            if (nn.getfinalValue(3) > max) { max = nn.getfinalValue(3); caseValue = 0; }

            switch (caseValue) { // Add 1 to the total number of times this choice was made
            case 1:
                totalShare++;
                break;

            case 2:
                totalReproduce++;
                break;

            case 3:
                totalFight++;
                break;

            default:
                totalIgnore++;
                break;
            }
        }

        float total = totalShare + totalReproduce + totalFight + totalIgnore;

        shareVector.push_back(totalShare / total);
        reproduceVector.push_back(totalReproduce / total);
        fightVector.push_back(totalFight / total);
        ignoreVector.push_back(totalIgnore / total);

    }

    
    shareVector = movingAverage(shareVector, precision/400.0f*10);
    fightVector = movingAverage(fightVector, precision/400.0f*10);
    ignoreVector = movingAverage(ignoreVector, precision/400.0f*10);
    reproduceVector = movingAverage(reproduceVector, precision/400.0f*10);
    

    std::string fileName = "Choice depending on " + xAxis;

    saveAsSvgWith4VectorsOneGraph(fileName, shareVector, reproduceVector, fightVector, ignoreVector, xAxis, "frequence", "Blue: Share | Purple: Reproduction | Red: Fight | Green: Ignore", maxValue);
}







void testLinkedVariables(NeuralNetwork& nn, short int v1, short int v2, short int output) {
    //name of the axes
    std::string nameX, nameY, nameOutput;

    // age1, age2, food1, food2, genetic distance
    std::string namesList[] = { "age1", "age2", "food1", "food2", "genetic_distance" };
    nameX = namesList[v1];
    nameY = namesList[v2];

    std::string outputsList[] = { "share", "reproduce", "fight", "ignore" };
    nameOutput = outputsList[output];

    std::vector<sf::Vector3f> data;
    std::vector<float> departure(5); // Values to be input into each network

    unsigned int numDivisions = 100;     // Number of cells per row/column
    const unsigned int PRECISION = 500; // Number of iterations per cell, used for averaging

    for (float x = 0; x < numDivisions + 1; x++) {
        for (float y = 0; y < numDivisions + 1; y++) {
            float totalValue = 0;

            for (unsigned int it = 0; it < PRECISION; it++) {
                departure.clear();

                for (unsigned int j = 0; j < 5; j++) {
                    departure.push_back(rand3() / 3.0f);
                }

                departure[v1] = x / 50.0f - 1.0f;
                departure[v2] = y / 50.0f - 1.0f;

                nn.setInputValues(departure);
                nn.update();

                float arrivalValue = nn.getfinalValue(output) / 2 + 0.5f;

                totalValue += arrivalValue;
            }

            totalValue /= PRECISION;

            if (v1 == 4 && v2 == 4) { data.push_back(sf::Vector3f(x * 4.0f, y * 4.0f, totalValue)); }
            else if (v1 == 4) { data.push_back(sf::Vector3f(x * 4.0f, y, totalValue)); }
            else if (v2 == 4) { data.push_back(sf::Vector3f(x, y * 4.0f, totalValue)); }
            else { data.push_back(sf::Vector3f(x, y, totalValue)); }
        }
    }

    sf::Vector3f graphColor = sf::Vector3f(255, 0, 0);
    if (nameOutput == "share") {
        graphColor = sf::Vector3f(0, 0, 255);
    }
    else if (nameOutput == "reproduce") {
        graphColor = sf::Vector3f(255, 0, 125);
    }
    else if (nameOutput == "ignore") {
        graphColor = sf::Vector3f(0, 255, 0);
    }

    std::string fileName = nameX.substr(0, 4) + nameX.back() + " " + nameY.substr(0, 4) + nameY.back() + " effect on " + nameOutput.substr(0, 5);

    createGraphWithFrequencyTwoParameters(fileName, nameX, nameY, data, numDivisions, numDivisions, graphColor, "Link between two variables: " + nameX + " and " + nameY + " on behavior: " + nameOutput);
}









void analize(NeuralNetwork& nn){
    if (!do_complete_analysis) return;


    // Graphs of favorite behaviors depending on inputs
    favoriteBehavior(nn, 0);
    favoriteBehavior(nn, 1);
    favoriteBehavior(nn, 2);
    favoriteBehavior(nn, 3);
    favoriteBehavior(nn, 4);

    // Heatmaps
    for (short int var1 = 0; var1 < 5; var1++){ // input 1
        for (short int var2 = var1 +1 ; var2 < 5; var2++){ // input 2
            for (short int var3 = 0; var3 < 4; var3++){ // effect on output n° var3
                if (var1 != var2) testLinkedVariables(nn, var1, var2, var3);
            }
        }   
    }

}