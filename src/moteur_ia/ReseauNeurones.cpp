#include "ReseauNeurones.hpp"



void ReseauNeurones::update(){
    unsigned int nbCouches = couches.size();

    for (unsigned int i = 1; i < nbCouches; i++){
        couches[i].update(&couches[i-1]);
    }

    this->valeursArrivee.clear();
    for (Neurone& n: couches[nbCouches-1].neurones){
        this->valeursArrivee.push_back(n.valeur);
    }

}




void ReseauNeurones::setValeursDepart(std::vector<float>& vals){
    if (couches.size() == 0) return;
    if (vals.size() != couches[0].neurones.size()) return;

    for (unsigned int i = 0; i < vals.size(); i++){
        couches[0].neurones[i].valeur = vals[i];
    }
}



void ReseauNeurones::getCouches(std::vector<Couche>& vecCouches) const {
    vecCouches = couches;
}



void ReseauNeurones::copier(const ReseauNeurones& r2){
    this->couches = r2.couches;
}



//renvoie une valeur aléatoire entre -3 et 3 proba% du temps, sinon renvoie la valeur val
float randProba(float val, float proba /*de 0.0 à 1.0*/){
    if (rand()%10000 < proba*10000){
        return ((float (rand()%1000))-500)/166.7f;
    }
    return val;
}


void ReseauNeurones::modifierAleatoirement(const float PROBABILITE_MUTATION){
    for (Couche& c: this->couches){
        for (Neurone& n: c.neurones){
            n.biais = randProba(n.biais, PROBABILITE_MUTATION);
            if (n.biais > 1.0f || n.biais < -1.0f) n.biais = ((float) (rand()%10000))/5000.0f-1.0f;
        }

        for (Synapse& s: c.synapses){
            s.poids = randProba(s.poids, PROBABILITE_MUTATION);
        }
    }
}





float ReseauNeurones::distanceDe(const ReseauNeurones& RN2) const{
    float s = 0;

    float v1, v2;

    for (unsigned int i = 0; i < this->couches.size(); i++){
        for (unsigned int j = 0; j < this->couches[i].neurones.size(); j++){
            v1 = this->couches[i].neurones[j].biais;
            v2 = RN2.couches[i].neurones[j].biais;

            s += fabs(v2 - v1);
        }

        for (unsigned int k = 0; k < this->couches[i].synapses.size(); k++){
            v1 = this->couches[i].synapses[k].poids;
            v2 = RN2.couches[i].synapses[k].poids;

            s += fabs(v2 - v1);
        }
    }

    
    return s;
}
