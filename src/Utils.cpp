#include "Utils.hpp"

bool activer_couleurs_terminal;



sf::Vector2f add(const sf::Vector2f a, const sf::Vector2f b)
{
    return sf::Vector2f(a.x+b.x, a.y+b.y);
}

sf::Vector3f add(const sf::Vector3f a, const sf::Vector3f b)
{
    return sf::Vector3f(a.x+b.x, a.y+b.y, a.z+b.z);
}


sf::Vector2f sub(const sf::Vector2f a, const sf::Vector2f b)
{
    return sf::Vector2f(a.x-b.x, a.y-b.y);
}

sf::Vector3f sub(const sf::Vector3f a, const sf::Vector3f b)
{
    return sf::Vector3f(a.x-b.x, a.y-b.y, a.z-b.z);
}


sf::Vector2f mult(const sf::Vector2f a, const sf::Vector2f b)
{
    return sf::Vector2f(a.x*b.x, a.y*b.y);
}

sf::Vector3f mult(const sf::Vector3f a, const sf::Vector3f b)
{
    return sf::Vector3f(a.x*b.x, a.y*b.y, a.z*b.z);
}


sf::Vector2f mult(const sf::Vector2f a, const float le)
{
    return sf::Vector2f(a.x*le, a.y*le);
}

sf::Vector3f mult(const sf::Vector3f a, const float le){
    return sf::Vector3f(a.x*le, a.y*le, a.z*le);
}

sf::Vector2f normalize(sf::Vector2f vect) {
    float d = dist(sf::Vector2f(0, 0), vect) + 0.000001;
    return sf::Vector2f(vect.x / d, vect.y / d);
}



float dist2(const sf::Vector2f p1, const sf::Vector2f p2) {
    return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
}

float dist(const sf::Vector2f p1, const sf::Vector2f p2) {
    return sqrt(dist2(p1, p2));
}


std::string v2toString(sf::Vector2f v) {
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}

std::string v3ToString(sf::Vector3f v) {
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}



sf::Vector3f HSVtoRGB(float H, float S, float V) {   // h:0-360.0, s:0.0-1.0, v:0.0-1.0
	S *= 100; V *= 100;
	if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
		std::cout << "The given HSV values are not in valid range" << H << " " << S << " " << V << "\n";
		return sf::Vector3f(0, 0, 0);
	}
	float s = S / 100;
	float v = V / 100;
	float C = s * v;
	float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;
	if (H >= 0 && H < 60) {
		r = C, g = X, b = 0;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = C, b = 0;
	}
	else if (H >= 120 && H < 180) {
		r = 0, g = C, b = X;
	}
	else if (H >= 180 && H < 240) {
		r = 0, g = X, b = C;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}
	int R = (r + m) * 255;
	int G = (g + m) * 255;
	int B = (b + m) * 255;

	return sf::Vector3f(R, G, B);
}



inline float max3(float a, float b, float c) {
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

inline float min3(float a, float b, float c) {
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}


sf::Vector3f RGBtoHSV(float R, float G, float B){
    //https://en.wikipedia.org/wiki/HSL_and_HSV
    R /= 255.0f; G /= 255.0f; B /= 255.0f;

    float xMax = max3(R, G, B);
    float xMin = min3(R, G, B);

    float v = xMax;
    float c = xMax - xMin;
    float l = v-c/2;

    float h = 0.0f, s = 0.0f;

    if(c > 0) {
        if(v == R) {
            h = 60.0f * (fmod(((G - B) / c), 6));
        } else if(v == G) {
            h = 60.0f * (((B - R) / c) + 2);
        } else if(v == B) {
            h = 60.0f * (((R - G) / c) + 4);
        }
        
        if(v > 0) {
            s = c / v;
        } else {
            s = 0;
        }
        
    } else {
        h = 0;
        s = 0;
    }
    
    if (h < 0.0) {
        h += 360;
    }

    return sf::Vector3f(h, s, v);
}



void coloredStdCout(std::string str, sf::Vector3f color){
    //afficher en couleur (blanc de base)
    //https://en.wikipedia.org/wiki/ANSI_escape_code
    if (!activer_couleurs_terminal) {
        std::cout << str;
        return;
    }

    std::string colorStrDebut = "\033[;97m";
    std::string colorStrFin = "\033[0m";
    
    if (color.x > 127){
        if (color.y > 127) { // jaune
            colorStrDebut = "\033[;93m";
        } else if (color.z > 127) { // magenta
            colorStrDebut = "\033[;95m";
        } else { // rouge
            colorStrDebut = "\033[;91m";
        }
    } else if (color.y > 127){
        if (color.z > 127){ // cyan
            colorStrDebut = "\033[;96m";
        } else { // vert
            colorStrDebut = "\033[;92m";
        }
    } else if (color.z > 127){ // bleu
            colorStrDebut = "\033[;94m";
    }
    


    std::cout << colorStrDebut + str + colorStrFin;
}








float randomNormalDistrib(float precision){
    float s = 0;
    for (unsigned int i = 0; i < precision; i++){
        s += rand()%1000;
    }
    return s /= 1000.0f * precision;
}



std::string rgbToHex(int r, int g, int b) {
    // convertit des couleurs de (0-255, 0-255, 0-255) à de l'hexadecimal #000FFF
    std::string rHex = "", gHex = "", bHex = "";
    int hexDigit;
    
    //on convertit chaque couleur à la fois, il suffit de passer d'une valeur en base 10
    //à deux char en base 16

    hexDigit = (r >> 4) & 0xF;
    rHex += (hexDigit < 10) ? (char)(hexDigit + '0') : (char)(hexDigit - 10 + 'A');
    hexDigit = r & 0xF;
    rHex += (hexDigit < 10) ? (char)(hexDigit + '0') : (char)(hexDigit - 10 + 'A');
    
    hexDigit = (g >> 4) & 0xF;
    gHex += (hexDigit < 10) ? (char)(hexDigit + '0') : (char)(hexDigit - 10 + 'A');
    hexDigit = g & 0xF;
    gHex += (hexDigit < 10) ? (char)(hexDigit + '0') : (char)(hexDigit - 10 + 'A');
    
    hexDigit = (b >> 4) & 0xF;
    bHex += (hexDigit < 10) ? (char)(hexDigit + '0') : (char)(hexDigit - 10 + 'A');
    hexDigit = b & 0xF;
    bHex += (hexDigit < 10) ? (char)(hexDigit + '0') : (char)(hexDigit - 10 + 'A');
    
    //combine les strings de chaque "canal" de couleur rgb
    std::string hexString = "#" + rHex + gHex + bHex;
    return hexString;
}




std::vector<float> moyenneMobile(const std::vector<float>& vals, unsigned int n){
    std::vector<float> calc;
    if (n > vals.size()) return calc;

    float avg;
    for (int i = 0; i < vals.size(); i++){
        avg = 0;
        int nb = 0;
        for (int j = i-n+1; j <= i; j++){
            if (j >= 0) {
                avg += vals[j];
                nb++;
            }
        }
        if (nb > 0) calc.push_back(avg / (float) nb);
        else calc.push_back(0);
    }
    return calc;
}






