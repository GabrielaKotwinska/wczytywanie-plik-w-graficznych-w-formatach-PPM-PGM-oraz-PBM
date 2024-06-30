#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>

struct Color {
    int r, g, b;
    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b;
    }
};

namespace std {
    template <>
    struct hash<Color> {
        std::size_t operator()(const Color& c) const {
            return (std::hash<int>()(c.r) ^ std::hash<int>()(c.g) ^ std::hash<int>()(c.b));
        }
    };
}

void readPPM(const std::string& filename, int& width, int& height, std::vector<Color>& pixels) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file");
    }

    std::string format;
    file >> format;

    if (format != "P6") {
        throw std::runtime_error("Invalid PPM format");
    }

    file >> width >> height;
    int maxVal;
    file >> maxVal;
    file.ignore(256, '\n'); // Skip to the end of the current line

    pixels.resize(width * height);
    for (int i = 0; i < width * height; ++i) {
        unsigned char r, g, b;
        file.read(reinterpret_cast<char*>(&r), 1);
        file.read(reinterpret_cast<char*>(&g), 1);
        file.read(reinterpret_cast<char*>(&b), 1);
        pixels[i] = { r, g, b };
    }
}

void processImage(const std::vector<Color>& pixels, int width, int height) {
    std::unordered_map<Color, int> colorCount;
    for (const auto& pixel : pixels) {
        colorCount[pixel]++;
    }

    auto mostCommonColor = std::max_element(colorCount.begin(), colorCount.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

    std::cout << "Szerokosc obrazu: " << width << std::endl;
    std::cout << "Wysokosc obrazu: " << height << std::endl;
    std::cout << "Najczesciej wystepujacy kolor to (" << mostCommonColor->first.r
        << ", " << mostCommonColor->first.g << ", " << mostCommonColor->first.b
        << ") i wystapil " << mostCommonColor->second << " razy" << std::endl;
    std::cout << "Liczba unikalnych kolorow: " << colorCount.size() << std::endl;
}

int main() {
    while (true) {
        std::string filename;
        std::cout << "Podaj nazwe pliku: ";
        std::cin >> filename;

        int width, height;
        std::vector<Color> pixels;

        try {
            readPPM(filename, width, height, pixels);
            processImage(pixels, width, height);
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        std::string response;
        std::cout << "Czy chcesz wczytac kolejny plik (tak/nie): ";
        std::cin >> response;
        if (response != "tak") {
            break;
        }
    }

    return 0;
}