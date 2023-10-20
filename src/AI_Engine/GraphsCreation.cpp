#include "GraphsCreation.hpp"
#include <filesystem>





void saveAsSvg(std::string title, std::vector<float>& data, float graphWidth, std::string axisX, std::string axisY, float maxValAxisX, sf::Vector3f color, bool fill) {
    std::string currentDirectory = std::filesystem::current_path().generic_string();
    std::string finalPath = currentDirectory + "/../../../data/" + title + ".svg";

    std::ofstream file;
    file.open(finalPath, std::fstream::trunc);

    if (file.is_open()) {
        int increment = 1;
        if (data.size() > 100) increment = data.size() / 20;

        float svgWidth = graphWidth + 250.0f;
        float svgHeight = 550.0f;
        float graphHeight = 400.0f;

        //center the graph in order to display the axes and texts (title, labels, scales)
        float offsetX = 150.0f;
        float offsetY = 75.0f;

        float maxHeight = data[0];
        for (unsigned int it = 0; it < data.size(); it += increment) {
            if (maxHeight < data[it]) maxHeight = data[it];
        }

        if (maxHeight == 0) maxHeight += 1;
        float factor = graphHeight / maxHeight;

        // header
        file << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
        file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << std::endl;
        file << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
        file << "<svg width=\"" + std::to_string(svgWidth) + "\" height=\"" + std::to_string(svgHeight) + "\"" << std::endl;
        file << "xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

        float barWidth = graphWidth / (data.size() - 1);

        //data
        std::string colorStr = rgbToHex(color.x, color.y, color.z);
        if (fill) {
            for (unsigned int i = 0; i < data.size(); i++) {
                file << "<rect x=\"" << offsetX + barWidth * i << "\" y=\"" << offsetY + graphHeight - data[i] * factor << "\" width=\"" << barWidth << "\" height=\"" << data[i] * factor << "\" style=\"fill:" + colorStr + ";\"/>" << std::endl;
            }
        }
        else {
            std::string pointsSvg = "";
            for (unsigned int i = 0; i < data.size(); i++) {
                pointsSvg += std::to_string((int)(offsetX + barWidth * i)) + "," + std::to_string((int)(offsetY + graphHeight - data[i] * factor)) + " ";
            }

            file << "<polyline points=\"" + pointsSvg + "\" style=\"fill:none;stroke:" + colorStr + ";stroke-width:3;stroke-linejoin:bevel\" />";
        }

        //axes
        file << "<line x1=\"" << offsetX << "\" y1=\"" << offsetY << "\" x2=\"" << offsetX << "\" y2=\"" << offsetY + graphHeight << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;
        file << "<line x1=\"" << offsetX << "\" y1=\"" << offsetY + graphHeight << "\" x2=\"" << offsetX + svgWidth << "\" y2=\"" << offsetY + graphHeight << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;

        //texts
        file << "<text x=\"" + std::to_string(graphWidth / 2 - 50) + "\" y=\"25\">" + title + "</text>" << std::endl;
        file << "<text x=\"10\" y=\"" + std::to_string(graphHeight / 2 + offsetY) + "\">" + axisY + "</text>" << std::endl;
        file << "<text x=\"" + std::to_string(graphWidth / 2 + offsetX - 50) + "\" y=\"" + std::to_string(graphHeight + offsetY + 40) + "\">" + axisX + "</text>" << std::endl;

        //scales
        float precisionY = 7;
        for (float id = 0; id <= precisionY; id++) {
            float value = id / precisionY * maxHeight;
            file << "<text x=\"" + std::to_string(offsetX - 50) + "\" y=\"" + std::to_string(graphHeight + offsetY - (value * factor)) + "\">" + std::to_string(value).substr(0, 5) + "</text>" << std::endl;
        }

        float precisionX = 5;
        unsigned int nbCharPrecisionAxis = 5;
        for (float id = 0; id <= precisionX; id++) {
            float value = id / precisionX * maxValAxisX;
            if (value > 99999) nbCharPrecisionAxis = 10;
            file << "<text x=\"" + std::to_string(offsetX + (id / precisionX) * graphWidth) + "\" y=\"" + std::to_string(graphHeight + offsetY + 15) + "\">" + std::to_string(value).substr(0, nbCharPrecisionAxis) + "</text>" << std::endl;
        }

        file << "</svg>" << std::endl;

        file.close();
    }
    else {
        std::cout << "Unable to open the file " + finalPath << std::endl;
    }
}









void saveAsSvgWith4VectorsOneGraph(std::string title,
    std::vector<float>& data1, std::vector<float>& data2,
    std::vector<float>& data3, std::vector<float>& data4,
    std::string axisX, std::string axisY, std::string legend, float maxValAxisX) {

    std::string currentDirectory = std::filesystem::current_path().generic_string();
    std::string finalPath = currentDirectory + "/../../../data/" + title + ".svg";

    std::ofstream file;
    file.open(finalPath, std::fstream::trunc);

    if (file.is_open()) {
        float svgWidth = 700.0f;
        float graphWidth = 400.0f;
        float svgHeight = 575.0f;
        float graphHeight = 400.0f;

        //center the graph in order to display the axes and texts (title, labels, scales)
        float offsetX = 150.0f;
        float offsetY = 100.0f;

        float maxHeight = data1[0] + data2[0] + data3[0] + data4[0];
        for (unsigned int it = 0; it < data1.size(); it++) {
            float totalValue = data1[it] + data2[it] + data3[it] + data4[it];
            if (maxHeight < totalValue) maxHeight = totalValue;
        }

        if (maxHeight == 0) maxHeight += 1;
        float factor = graphHeight / maxHeight;

        // header
        file << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
        file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << std::endl;
        file << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
        file << "<svg width=\"" + std::to_string(svgWidth) + "\" height=\"" + std::to_string(svgHeight) + "\"" << std::endl;
        file << "xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

        //data
        float barWidth = graphWidth / (data1.size() - 1);
        for (unsigned int i = 0; i < data1.size(); i++) {
            float y1 = offsetY + graphHeight - data1[i] * factor;
            file << "<rect x=\"" << offsetX + barWidth * i << "\" y=\"" << y1 << "\" width=\"" << barWidth + 0.5f << "\" height=\"" << data1[i] * factor << "\" style=\"fill:" + color1 + ";\"/>" << std::endl;

            float y2 = y1 - data2[i] * factor;
            file << "<rect x=\"" << offsetX + barWidth * i << "\" y=\"" << y2 << "\" width=\"" << barWidth + 0.5f << "\" height=\"" << data2[i] * factor << "\" style=\"fill:" + color2 + ";\"/>" << std::endl;

            float y3 = y2 - data3[i] * factor;
            file << "<rect x=\"" << offsetX + barWidth * i << "\" y=\"" << y3 << "\" width=\"" << barWidth + 0.5f << "\" height=\"" << data3[i] * factor << "\" style=\"fill:" + color3 + ";\"/>" << std::endl;

            float y4 = y3 - data4[i] * factor;
            file << "<rect x=\"" << offsetX + barWidth * i << "\" y=\"" << y4 << "\" width=\"" << barWidth + 0.5f << "\" height=\"" << data4[i] * factor << "\" style=\"fill:" + color4 + ";\"/>" << std::endl;
        }

        //axes
        file << "<line x1=\"" << offsetX << "\" y1=\"" << offsetY << "\" x2=\"" << offsetX << "\" y2=\"" << offsetY + graphHeight << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;
        file << "<line x1=\"" << offsetX << "\" y1=\"" << offsetY + graphHeight << "\" x2=\"" << offsetX + svgWidth << "\" y2=\"" << offsetY + graphHeight << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;

        //texts
        file << "<text x=\"" + std::to_string(graphWidth / 2 - 50) + "\" y=\"25\">" + title + "</text>" << std::endl;
        file << "<text x=\"" + std::to_string(graphWidth / 2 - 100) + "\" y=\"50\">" + legend + "</text>" << std::endl;

        file << "<text x=\"10\" y=\"" + std::to_string(graphHeight / 2 + offsetY) + "\">" + axisY + "</text>" << std::endl;
        file << "<text x=\"" + std::to_string(graphWidth / 2 + offsetX - 50) + "\" y=\"" + std::to_string(graphHeight + offsetY + 40) + "\">" + axisX + "</text>" << std::endl;

        //scales
        float precisionY = 7;
        for (float id = 0; id <= precisionY; id++) {
            float value = id / precisionY * maxHeight;
            file << "<text x=\"" + std::to_string(offsetX - 50) + "\" y=\"" + std::to_string(graphHeight + offsetY - (value * factor)) + "\">" + std::to_string(value).substr(0, 5) + "</text>" << std::endl;
        }

        float precisionX = 5;
        for (float id = 0; id <= precisionX; id++) {
            float value = id / precisionX * maxValAxisX;
            file << "<text x=\"" + std::to_string(offsetX + (id / precisionX) * graphWidth) + "\" y=\"" + std::to_string(graphHeight + offsetY + 15) + "\">" + std::to_string(value).substr(0, 5) + "</text>" << std::endl;
        }

        file << "</svg>" << std::endl;

        file.close();
    }
    else {
        std::cout << "Unable to open the file " + finalPath << std::endl;
    }

}









void createGraphWithFrequencyTwoParameters(std::string title, std::string axisX, std::string axisY, std::vector<sf::Vector3f>& data, unsigned int divisionsInX, unsigned int divisionsInY, sf::Vector3f color, std::string legend) {
    std::string current_directory = std::filesystem::current_path().generic_string();
    std::string final_path = current_directory + "/../../../data/" + title + ".svg";

    std::cout << "\nWriting " + legend;
    std::ofstream file;
    file.open(final_path, std::fstream::trunc);

    if (file.is_open()) {
        float svgWidth = 700.0f;
        float graphWidth = 400.0f;
        float svgHeight = 575.0f;
        float graphHeight = 400.0f;

        //center the graph in order to display the axes and texts (title, labels, scales)
        float offsetX = 150.0f;
        float offsetY = 100.0f;

        float maxHeight = data[0].y;
        float maxWidth = data[0].x;

        for (unsigned int it = 0; it < data.size(); it++) {
            float _v = data[it].x;
            if (maxWidth < _v) maxWidth = _v;

            _v = data[it].y;
            if (maxHeight < _v) maxHeight = _v;
        }

        if (maxHeight == 0) maxHeight += 1;
        if (maxWidth == 0) maxWidth += 1;


        // header
        file << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
        file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << std::endl;
        file << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
        file << "<svg width=\"" + std::to_string(svgWidth) + "\" height=\"" + std::to_string(svgHeight) + "\"" << std::endl;
        file << "xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

        float barWidth = graphWidth / ((float)divisionsInX);
        float barHeight = graphHeight / ((float)divisionsInY);

        //data
        for (unsigned int i = 0; i < data.size(); i++) {
            float y1 = offsetY + graphHeight - (data[i].y / (maxHeight / (float)divisionsInY)) * barHeight - barHeight;
            float x1 = offsetX + barWidth * (data[i].x / (maxWidth / (float)divisionsInX));

            std::string colorStr = rgbToHex(color.x * data[i].z, color.y * data[i].z, color.z * data[i].z);
            file << "<rect x=\"" << x1 << "\" y=\"" << y1 << "\" width=\"" << barWidth + 0.5f << "\" height=\"" << barHeight + 0.5f << "\" style=\"fill:" + colorStr + ";\"/>" << std::endl;
        }

        //axes
        file << "<line x1=\"" << offsetX << "\" y1=\"" << offsetY - barHeight << "\" x2=\"" << offsetX << "\" y2=\"" << offsetY + graphHeight << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;
        file << "<line x1=\"" << offsetX << "\" y1=\"" << offsetY + graphHeight << "\" x2=\"" << offsetX + svgWidth << "\" y2=\"" << offsetY + graphHeight << "\" style=\"stroke:black;stroke-width:2\"/>" << std::endl;

        //texts
        file << "<text x=\"" + std::to_string(graphWidth / 2 - title.length()) + "\" y=\"25\">" + title + "</text>" << std::endl;
        file << "<text x=\"" + std::to_string(graphWidth / 2 - legend.length() * 1.5) + "\" y=\"50\">" + legend + "</text>" << std::endl;

        file << "<text x=\"10\" y=\"" + std::to_string(graphHeight / 2 + offsetY) + "\">" + axisY + "</text>" << std::endl;
        file << "<text x=\"" + std::to_string(graphWidth / 2 + offsetX - 50) + "\" y=\"" + std::to_string(graphHeight + offsetY + 40) + "\">" + axisX + "</text>" << std::endl;

        //scales
        float precisionY = 7;
        for (float id = 0; id <= precisionY; id++) {
            float value = id / precisionY * maxHeight;
            float y1 = offsetY + graphHeight - (value / (maxHeight / (float)divisionsInY)) * barHeight - barHeight;
            file << "<text x=\"" + std::to_string(offsetX - 50) + "\" y=\"" + std::to_string(y1) + "\">" + std::to_string(value).substr(0, 5) + "</text>" << std::endl;
        }

        float precisionX = 5;
        for (float id = 0; id <= precisionX; id++) {
            float value = id / precisionX * maxWidth;
            float x1 = offsetX + barWidth * (value / (maxWidth / (float)divisionsInX));
            file << "<text x=\"" + std::to_string(x1) + "\" y=\"" + std::to_string(graphHeight + offsetY + 15) + "\">" + std::to_string(value).substr(0, 5) + "</text>" << std::endl;
        }

        file << "</svg>" << std::endl;

        file.close();
    }
    else {
        std::cout << "Unable to open the file " + final_path << std::endl;
    }
}




