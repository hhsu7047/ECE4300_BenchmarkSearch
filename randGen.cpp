#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>

std::string getDownloadsFolder() { //Generates in user downloads folder
    const char* homeDir;
#ifdef _WIN32
    homeDir = std::getenv("USERPROFILE");
    return (homeDir) ? std::string(homeDir) + "\\Downloads\\" : "";
#else
    homeDir = std::getenv("HOME");
    return (homeDir) ? std::string(homeDir) + "/Downloads/" : "";
#endif
}

int main() {
    std::string folder = getDownloadsFolder();
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Range is 1 billion—duplicates will be extremely rare
    std::uniform_int_distribution<long long> dis(1, 1000000000);

    for (int i = 1; i <= 7; ++i) {
        int N = static_cast<int>(std::pow(10, i));
        std::cout << "Generating " << N << " numbers..." << std::flush;

        // We generate 1.01x the numbers needed to account for rare duplicates
        size_t bufferSize = N + (N / 100); 
        std::vector<long long> numbers;
        numbers.reserve(bufferSize);

        for (size_t j = 0; j < bufferSize; ++j) {
            numbers.push_back(dis(gen));
        }

        // Sort and remove duplicates in one pass
        std::sort(numbers.begin(), numbers.end());
        auto last = std::unique(numbers.begin(), numbers.end());
        numbers.erase(last, numbers.end());

        // Ensure we have exactly N numbers
        if (numbers.size() > (size_t)N) {
            numbers.resize(N);
        }

        std::string fname = folder + std::to_string(N) + "_numbers.csv";
        std::ofstream writeFile(fname);
        
        if (writeFile.is_open()) {
            for (int j = 0; j < N; ++j) {
                writeFile << numbers[j] << "\n";
            }
            writeFile.close();
            std::cout << " Done. Saved to " << fname << std::endl;
        }
    }

    return 0;
}