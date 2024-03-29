//Test program

#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>

int main(int argc, char* argv[]) {
    // Check if a file path has been provided as an argument
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path to config.json>\n";
        return 1; // Exit with an error code
    }

    // The first argument after the program name is the path to the config file
    std::string configFilePath = argv[1];

    while (true) {
        std::cout << "Modifying config.canRestart...\n";

        // Attempt to read the JSON file using the provided path
        nlohmann::json config;
        std::ifstream configFile(configFilePath);
        if (configFile.is_open()) {
            configFile >> config;
            configFile.close();
        } else {
            std::cerr << "Failed to open " << configFilePath << " for reading.\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue; // Try again after a short wait
        }

        // Toggle the canRestart value
        bool newRestartValue = !config.value("canRestart", false);
        config["canRestart"] = newRestartValue;

        // Write the updated JSON object back to the file
        std::ofstream outputFile(configFilePath);
        if (outputFile.is_open()) {
            outputFile << config.dump(4); // Pretty printing with a 4-space indent
            outputFile.close();
            std::cout << "config.canRestart is now " << (newRestartValue ? "true" : "false") << "\n";
        } else {
            std::cerr << "Failed to open " << configFilePath << " for writing.\n";
        }

        // Wait for 30 seconds before the next iteration
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }

    return 0; // This line will never be reached
}

