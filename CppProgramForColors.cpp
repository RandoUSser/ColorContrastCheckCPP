#include <iostream>
#include <cmath>
#include <string>
#include <vector>

class Color {
    int hexNumber;
    double luminescenceValue;
    struct RGBValues {
        int redChannel;
        int greenChannel;
        int blueChannel;
    };
    
    RGBValues RGB;
    double applyGammaCorrection(double value) {
        return (value <= 0.04045) ? value / 12.92 : pow((value + 0.055) / 1.055, 2.4);
    }
    
    public:
    
    Color (int hex) : hexNumber(hex){
        RGB.redChannel = (hexNumber >> 16) & 0xFF;
        RGB.greenChannel = (hexNumber >> 8) & 0xFF;
        RGB.blueChannel = hexNumber & 0xFF;
        
        double red = applyGammaCorrection(RGB.redChannel / 255.0);
        double green = applyGammaCorrection(RGB.greenChannel / 255.0);
        double blue = applyGammaCorrection(RGB.blueChannel / 255.0);
        
        luminescenceValue = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
    }
    ~Color (){}
    friend std::ostream& operator<<(std::ostream& os, const Color& color);
    
    const double& getLuminescence () const {
        return luminescenceValue;
    }
    
    const int& getHexValue () const {
        return hexNumber;
    }
};

std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << "R: " << color.RGB.redChannel << " "
       << "G: " << color.RGB.greenChannel << " "
       << "B: " << color.RGB.blueChannel;
    return os;
}

void printWithColors (int hexValueOne, int hexValueTwo,  std::string text){
    std::string ANSICode = "\33[38;2;" + std::to_string((hexValueOne >> 16) & 0xFF) + ";" + std::to_string((hexValueOne >> 8) & 0xFF) + ";" + std::to_string(hexValueOne & 0xFF) + ";48;2;" + std::to_string((hexValueTwo >> 16) & 0xFF) + ";" + std::to_string((hexValueTwo >> 8) & 0xFF) + ";" + std::to_string(hexValueTwo & 0xFF) + "m";
    std::cout << ANSICode << text << "\033[0m ";
}

double calculateContrast(double luminescenceOne, double luminescenceTwo) {
    return (luminescenceTwo > luminescenceOne) 
           ? (luminescenceTwo + 0.05) / (luminescenceOne + 0.05)
           : (luminescenceOne + 0.05) / (luminescenceTwo + 0.05);
}

std::string getContrastSymbol(double colorContrast){
    if (colorContrast >= 4.5){
        return "Good";
    }
    else if (colorContrast >= 3.0){
        return "Warning";
    }
    else {
        return "Bad";
    }
}

int main() {
    std::vector<Color> colors;
    int numberOfColors, hexColorValue;
    double colorContrast;
    std::string colorContrastResult;
    std::cin >> std::hex >> numberOfColors;
    
    colors.reserve(numberOfColors);
    
    for (int i = 0; i < numberOfColors; ++i) {
        std::cin >> std::hex >> hexColorValue;
        while (std::cin.fail() || hexColorValue >= 0x1000000) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "entered values are not in range, try again ☹️\n";
            std::cin >> hexColorValue;
        }
        colors.emplace_back(hexColorValue);
    }
    
    for (size_t i = 0; i < colors.size(); ++i) {
        for (size_t j = 0; j < colors.size(); ++j) {
            if (i == j) continue;  // Skip self-comparison
            colorContrast = calculateContrast(colors[i].getLuminescence(), colors[j].getLuminescence());
            colorContrastResult = getContrastSymbol(colorContrast);
            
            std::cout << "Color 1: " << colors[i] << "\n"
                      << "Color 2: " << colors[j] << "\n"
                      << "Contrast: " << colorContrast << " (" << colorContrastResult << ")\n\n";
        }
    }
    return 0;
}
