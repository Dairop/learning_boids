#include "Specie.hpp"

std::vector<Specie> topSpecies;
const unsigned int NB_SPECIES_TOP = 15; // number of species saved in the leaderboard (saved even if they disapear)



const int GENETIC_DIVERSITY_IN_ONE_SPECIE = 40.0f; // lower value means less diversity in one specie

unsigned int timeSinceAnalysis = 0;
const unsigned int analyzeEvery = 250; // time between each deep analysis (1 = everytime we run updateTop)

unsigned int prevAnalysisId = 0; //in order to not analyze 2 times the same specie since the result would be the same


void updateTop(std::vector<Specie>& species, const std::vector<Fish*>& fishes) {
    if (species.size() == 0) return;

    //the species vector should be sorted from oldest to youngest
    for (unsigned int i = 0; ((i < NB_SPECIES_TOP) && (i < species.size())); i++){
        topSpecies.push_back(species[i]);
    }

    std::sort(topSpecies.begin(), topSpecies.end());

    //remove duplicates
    for (unsigned int i = 0; i < topSpecies.size(); i++){
        for (int j = i+1; j < topSpecies.size(); j++){
            if (topSpecies[i]._id == topSpecies[j]._id){
                topSpecies[i]._age = topSpecies[j]._age;
                topSpecies.erase(topSpecies.begin()+j);
                j--;
            }
        }
    }

    while (topSpecies.size() > NB_SPECIES_TOP){
        topSpecies.erase(topSpecies.begin(), topSpecies.begin()+1);
    }


    Specie& top1 = topSpecies[topSpecies.size()-1];
    if (timeSinceAnalysis >= analyzeEvery){
        if (prevAnalysisId != top1._id) {
            prevAnalysisId = top1._id;
            timeSinceAnalysis = 0;

            analize(top1._NN);	//analyze the best (in total time lived) specie since the start of the simulation
            //analyser(Species[Species.size()-1]._reseauBase); //we can also analyze the best specie that is still alive
        }
        else {
            //we already analyzed this specie
            std::cout << "\nSpecie already analyzed last time";
            timeSinceAnalysis = 0;
        }
    }



    //update if a specie is alive or not
    for (Specie& spe : topSpecies) {
        spe._isAlive = false;
        for (const Fish* f : fishes) {
            if (f->speciesId == spe._id) {
                spe._isAlive = true;
                break;
            }
        }
    }
}



void updateSpeciesVector(std::vector<Specie>& species, const std::vector<Fish*>& fishes){
    for (Specie& spe: species){
        spe._age++;
    }

    timeSinceAnalysis++;

    if (timeSinceAnalysis%10 == 0){
        updateTop(species, fishes);
        std::cout << "\nTime until global analysis: " << timeSinceAnalysis << "/" << analyzeEvery << "\n";
    }

    //if no Entity is using a Specie, then we delete it from our list
    if (timeSinceAnalysis%10 == 0){
        for (unsigned int i = 0; i < species.size();){
            species[i]._isAlive = false;
            for (unsigned int j = 0; j < fishes.size(); j++){
               species[i]._isAlive = species[i]._isAlive || (species[i]._id == fishes[j]->speciesId);
            }

            if (species[i]._isAlive) {
                i++;
            } else {
                species.erase(species.begin()+i);
            }
        }

        std::sort(topSpecies.begin(), topSpecies.end());
        displaySpecies(topSpecies);
    }


    //create a Specie for every Entity that doesn't have a Specie with a minimal genetic distance of GENETIC_DIVERSITY_IN_ONE_SPECIE
    for (const Fish* f: fishes){
        bool haveASpecie = false;

        for (Specie& spe: species){
            if (f->NN.geneticDistanceWith(spe._NN) < GENETIC_DIVERSITY_IN_ONE_SPECIE){
                haveASpecie = true;
                break;
            }
        }

        if (!haveASpecie){
            species.push_back(Specie(rand()%10000000, f->NN));
        }
    }
}




void updateEntitiesSpecies(const std::vector<Specie>& species, std::vector<Fish*>& fishes){
    // Find the closest Specie for each Fish
    for (Fish* f: fishes){
        float dist_min = 1.7976931348623e+308;

        for (const Specie& sp: species){
            float dist_test = f->NN.geneticDistanceWith(sp._NN);
            if (dist_test < dist_min){
                dist_min = dist_test;
                f->speciesId =  sp._id;
                f->color = sp._color;
            }
        }
    }
}








std::string Specie::toString() const {
    return "\nAge: "+std::to_string(_age)+"  id: "+std::to_string(_id) +"  Color: "+ colorToString(_color) + (_isAlive ? " < " : "");
}

void displaySpecies(const std::vector<Specie>& species) {
    std::cout << "\n\n";

    for (unsigned int i = 0; i < species.size();i++){
        coloredStdCout(species[i].toString(), species[i]._color);
    }
}