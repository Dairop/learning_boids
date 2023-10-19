#include "creationGraphes.hpp"
#include <filesystem>





void enregisterEnSvg(std::string titre, std::vector<float>& data, float largeurGraphe, std::string axeX, std::string axeY, float maxValAxeX, sf::Vector3f couleur, bool remplir){
    std::string current_directory = std::filesystem::current_path().generic_string();
    std::string final_path = current_directory + "/../../../data/" + titre + ".svg";
    
    std::ofstream file;
    file.open(final_path, std::fstream::trunc);

    if (file.is_open()) {
        float largeurSvg = largeurGraphe + 250.0f;

        float hauteurSvg = 550.0f;
        float hauteurGraphe = 400.0f;

        //centrer le graphe pour pouvoir afficher les axes et les textes (titre, noms des axes, echelles)
        float decalageX = 150.0f;
        float decalageY = 75.0f;

        float hauteurMax = data[0];
        for (unsigned int it = 0; it < data.size(); it++){
            if (hauteurMax < data[it]) hauteurMax = data[it];
        }

        if (hauteurMax==0) hauteurMax += 1;
        float facteur = hauteurGraphe/hauteurMax; //facteur multiplicateur pour passer d'une valeur à une hauteur dans le graphe


        //en-tete
        file << "<?xml version=\"1.0\" standalone=\"no\"?>"<< std::endl;
        file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << std::endl;
        file << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
        file << "<svg width=\""+std::to_string(largeurSvg)+"\" height=\""+std::to_string(hauteurSvg)+"\"" << std::endl; 
        file << "xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

        //donnees
        float largeurBarre = largeurGraphe/(data.size()-1);

        std::string couleurStr = rgbToHex(couleur.x, couleur.y, couleur.z);
        if (remplir){
            for (unsigned int i = 0; i < data.size(); i++){
                file << "<rect x=\"" << decalageX+largeurBarre*i <<"\" y=\"" << decalageY+hauteurGraphe-data[i]*facteur << "\" width=\"" << largeurBarre << "\" height=\"" << data[i]*facteur << "\" style=\"fill:"+couleurStr+";\"/>" << std::endl;
             }
        } else {
            std::string pointsSvg = "";
            for (unsigned int i = 0; i < data.size(); i++){
                pointsSvg += std::to_string((int) (decalageX+largeurBarre*i)) + "," + std::to_string((int) (decalageY+hauteurGraphe-data[i]*facteur)) + " ";
            }

            file << "<polyline points=\"" + pointsSvg + "\" style=\"fill:none;stroke:"+couleurStr+";stroke-width:3;stroke-linejoin:bevel\" />";

        }
        

        //axes
        file << "<line x1=\"" << decalageX << "\" y1=\"" << decalageY << "\" x2=\"" << decalageX << "\" y2=\"" << decalageY+hauteurGraphe << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;
        file << "<line x1=\"" << decalageX << "\" y1=\"" << decalageY+hauteurGraphe << "\" x2=\"" << decalageX+largeurSvg << "\" y2=\"" << decalageY+hauteurGraphe << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;


        //textes
        file << "<text x=\""+std::to_string(largeurGraphe/2-50)+"\" y=\"25\">"+titre+"</text>" << std::endl;

        file << "<text x=\"10\" y=\""+std::to_string(hauteurGraphe/2+decalageY)+"\">"+axeY+"</text>" << std::endl;
        file << "<text x=\""+std::to_string(largeurGraphe/2+decalageX-50)+"\" y=\""+std::to_string(hauteurGraphe+decalageY+40)+"\">"+axeX+"</text>" << std::endl;

        //echelle axe Y
        float precisionY = 7;
        for (float id = 0; id <= precisionY; id++){
            float valeur = id/precisionY * hauteurMax;
            file << "<text x=\""+std::to_string(decalageX-50)+"\" y=\""+std::to_string(hauteurGraphe+decalageY-(valeur*facteur))+"\">"+std::to_string(valeur).substr(0, 5)+"</text>" << std::endl;
        }

        //echelle axe x
        float precisionX = 5;
        unsigned int nbCharPrecisionAxe = 5;
        for (float id = 0; id <= precisionX; id++){
            float valeur = id/precisionX * maxValAxeX;
            if (valeur > 99999) nbCharPrecisionAxe = 10; // permet d'afficher des grands nombres sur l'axe des X si besoin
            file << "<text x=\""+std::to_string(decalageX+(id/precisionX)*largeurGraphe)+"\" y=\""+std::to_string(hauteurGraphe+decalageY+15)+"\">"+std::to_string(valeur).substr(0, nbCharPrecisionAxe)+"</text>" << std::endl;
        }

        file << "</svg>" << std::endl;



        // close the file
        file.close();
    } else {
        std::cout << "Impossible d'ouvrir le fichier "+ final_path << std::endl;
    }
}








void enregistrerEnSvg4Vecteurs1Graphe( std::string titre, 
    std::vector<float>& d1, std::vector<float>& d2, 
    std::vector<float>& d3, std::vector<float>& d4, 
    std::string axeX, std::string axeY, std::string legende, float maxValAxeX){


    std::string current_directory = std::filesystem::current_path().generic_string();
    std::string final_path = current_directory + "/../../../data/" + titre + ".svg";

    std::ofstream file;
    file.open(final_path, std::fstream::trunc);

    if (file.is_open()) {
        float largeurSvg = 700.0f;
        float largeurGraphe = 400.0f;

        float hauteurSvg = 575.0f;
        float hauteurGraphe = 400.0f;

        //centrer le graphe pour pouvoir afficher les axes et les textes (titre, noms des axes, echelles)
        float decalageX = 150.0f;
        float decalageY = 100.0f;

        float hauteurMax = d1[0]+d2[0]+d3[0]+d4[0];
        for (unsigned int it = 0; it < d1.size(); it++){
            float _v = d1[it]+d2[it]+d3[it]+d4[it];
            if (hauteurMax < _v) hauteurMax = _v;
        }

        if (hauteurMax==0) hauteurMax += 1;
        float facteur = hauteurGraphe/hauteurMax; //facteur multiplicateur pour passer d'une valeur à une hauteur dans le graphe


        //en-tete
        file << "<?xml version=\"1.0\" standalone=\"no\"?>"<< std::endl;
        file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << std::endl;
        file << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
        file << "<svg width=\""+std::to_string(largeurSvg)+"\" height=\""+std::to_string(hauteurSvg)+"\"" << std::endl; 
        file << "xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

        //donnees
        float largeurBarre = largeurGraphe/(d1.size()-1);
        for (unsigned int i = 0; i < d1.size(); i++){
            float y1 = decalageY+hauteurGraphe-d1[i]*facteur;
            file << "<rect x=\"" << decalageX+largeurBarre*i <<"\" y=\"" << y1 << "\" width=\"" << largeurBarre+0.5f << "\" height=\"" << d1[i]*facteur << "\" style=\"fill:"+color1+";\"/>" << std::endl;
            
            float y2 = y1-d2[i]*facteur;
            file << "<rect x=\"" << decalageX+largeurBarre*i <<"\" y=\"" << y2 << "\" width=\"" << largeurBarre+0.5f << "\" height=\"" << d2[i]*facteur << "\" style=\"fill:"+color2+";\"/>" << std::endl;

            float y3 = y2-d3[i]*facteur;
            file << "<rect x=\"" << decalageX+largeurBarre*i <<"\" y=\"" << y3 << "\" width=\"" << largeurBarre+0.5f << "\" height=\"" << d3[i]*facteur << "\" style=\"fill:"+color3+";\"/>" << std::endl;

            float y4 = y3-d4[i]*facteur;
            file << "<rect x=\"" << decalageX+largeurBarre*i <<"\" y=\"" << y4 << "\" width=\"" << largeurBarre+0.5f << "\" height=\"" << d4[i]*facteur << "\" style=\"fill:"+color4+";\"/>" << std::endl;
        }

        //axes
        file << "<line x1=\"" << decalageX << "\" y1=\"" << decalageY << "\" x2=\"" << decalageX << "\" y2=\"" << decalageY+hauteurGraphe << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;
        file << "<line x1=\"" << decalageX << "\" y1=\"" << decalageY+hauteurGraphe << "\" x2=\"" << decalageX+largeurSvg << "\" y2=\"" << decalageY+hauteurGraphe << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;


        //textes
        file << "<text x=\""+std::to_string(largeurGraphe/2-50)+"\" y=\"25\">"+titre+"</text>" << std::endl;
        file << "<text x=\""+std::to_string(largeurGraphe/2-100)+"\" y=\"50\">"+legende+"</text>" << std::endl;

        file << "<text x=\"10\" y=\""+std::to_string(hauteurGraphe/2+decalageY)+"\">"+axeY+"</text>" << std::endl;
        file << "<text x=\""+std::to_string(largeurGraphe/2+decalageX-50)+"\" y=\""+std::to_string(hauteurGraphe+decalageY+40)+"\">"+axeX+"</text>" << std::endl;

        //echelle axe Y
        float precisionY = 7;
        for (float id = 0; id <= precisionY; id++){
            float valeur = id/precisionY * hauteurMax;
            file << "<text x=\""+std::to_string(decalageX-50)+"\" y=\""+std::to_string(hauteurGraphe+decalageY-(valeur*facteur))+"\">"+std::to_string(valeur).substr(0, 5)+"</text>" << std::endl;
        }

        //echelle axe x
        float precisionX = 5;
        for (float id = 0; id <= precisionX; id++){
            float valeur = id/precisionX * maxValAxeX;
            file << "<text x=\""+std::to_string(decalageX+(id/precisionX)*largeurGraphe)+"\" y=\""+std::to_string(hauteurGraphe+decalageY+15)+"\">"+std::to_string(valeur).substr(0, 5)+"</text>" << std::endl;
        }

        file << "</svg>" << std::endl;



        // close the file
        file.close();
    } else {
        std::cout << "Impossible d'ouvrir le fichier "+ final_path << std::endl;
    }

}













void creerGrapheFrequence2Parametres(std::string titre, std::string axeX, std::string axeY, std::vector<sf::Vector3f>& data, unsigned int nbDivisionsEnX, unsigned int nbDivisionsEnY, sf::Vector3f color, std::string legende){
    std::string current_directory = std::filesystem::current_path().generic_string();
    std::string final_path = current_directory + "/../../../data/" + titre + ".svg";

    std::ofstream file;
    file.open(final_path, std::fstream::trunc);

    if (file.is_open()) {
        float largeurSvg = 700.0f;
        float largeurGraphe = 400.0f;

        float hauteurSvg = 575.0f;
        float hauteurGraphe = 400.0f;

        //centrer le graphe pour pouvoir afficher les axes et les textes (titre, noms des axes, echelles)
        float decalageX = 150.0f;
        float decalageY = 100.0f;

        float hauteurMax = data[0].y;
        float largeurMax = data[0].x;

        for (unsigned int it = 0; it < data.size(); it++){
            float _v = data[it].x;
            if (largeurMax < _v) largeurMax = _v;

            _v = data[it].y;
            if (hauteurMax < _v) hauteurMax = _v;
        }

        if (hauteurMax==0) hauteurMax += 1;
        if (largeurMax==0) largeurMax += 1;

        //en-tete
        file << "<?xml version=\"1.0\" standalone=\"no\"?>"<< std::endl;
        file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << std::endl;
        file << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
        file << "<svg width=\""+std::to_string(largeurSvg)+"\" height=\""+std::to_string(hauteurSvg)+"\"" << std::endl; 
        file << "xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

        //donnees
        float largeurBarre = largeurGraphe/((float) nbDivisionsEnX);
        float hauteurBarre = hauteurGraphe/((float) nbDivisionsEnY);
        for (unsigned int i = 0; i < data.size(); i++){
            float y1 = decalageY + hauteurGraphe - (data[i].y / (hauteurMax / (float) nbDivisionsEnY)) * hauteurBarre - hauteurBarre;
            float x1 = decalageX + largeurBarre * (data[i].x / (largeurMax / (float) nbDivisionsEnX));


            std::string colorStr = rgbToHex(color.x * data[i].z, color.y * data[i].z, color.z * data[i].z);
            file << "<rect x=\"" << x1 <<"\" y=\"" << y1 << "\" width=\"" << largeurBarre+0.5f << "\" height=\"" << hauteurBarre+0.5f << "\" style=\"fill:"+colorStr+";\"/>" << std::endl;
        }

        //axes
        file << "<line x1=\"" << decalageX << "\" y1=\"" << decalageY-hauteurBarre << "\" x2=\"" << decalageX << "\" y2=\"" << decalageY+hauteurGraphe << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;
        file << "<line x1=\"" << decalageX << "\" y1=\"" << decalageY+hauteurGraphe << "\" x2=\"" << decalageX+largeurSvg << "\" y2=\"" << decalageY+hauteurGraphe << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;


        //textes
        file << "<text x=\""+std::to_string(largeurGraphe/2-titre.length())+"\" y=\"25\">"+titre+"</text>" << std::endl;
        file << "<text x=\""+std::to_string(largeurGraphe/2-legende.length()*1.5)+"\" y=\"50\">"+legende+"</text>" << std::endl;

        file << "<text x=\"10\" y=\""+std::to_string(hauteurGraphe/2+decalageY)+"\">"+axeY+"</text>" << std::endl;
        file << "<text x=\""+std::to_string(largeurGraphe/2+decalageX-50)+"\" y=\""+std::to_string(hauteurGraphe+decalageY+40)+"\">"+axeX+"</text>" << std::endl;

        //echelle axe Y
        float precisionY = 7;
        for (float id = 0; id <= precisionY; id++){
            float valeur = id/precisionY * hauteurMax;
            float y1 = decalageY + hauteurGraphe - (valeur / (hauteurMax / (float) nbDivisionsEnY)) * hauteurBarre - hauteurBarre;

            file << "<text x=\""+std::to_string(decalageX-50)+"\" y=\""+std::to_string(y1)+"\">"+std::to_string(valeur).substr(0, 5)+"</text>" << std::endl;
        }

        //echelle axe x
        float precisionX = 5;
        for (float id = 0; id <= precisionX; id++){
            float valeur = id/precisionX * largeurMax;
            float x1 = decalageX + largeurBarre * (valeur / (largeurMax / (float) nbDivisionsEnX));
            file << "<text x=\""+std::to_string(x1)+"\" y=\""+std::to_string(hauteurGraphe+decalageY+15)+"\">"+std::to_string(valeur).substr(0, 5)+"</text>" << std::endl;
        }

        file << "</svg>" << std::endl;



        // close the file
        file.close();
    } else {
        std::cout << "Impossible d'ouvrir le fichier "+final_path << std::endl;
    }



}



