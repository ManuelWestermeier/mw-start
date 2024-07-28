#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <thread>

using std::ifstream;
using std::ofstream;
using std::string;
using std::endl;
using std::cout;
using std::cin;
using std::map;
using std::istringstream;
using std::ostringstream;
using std::thread;

const string fileDataPath = "data.txt";

map<string, string> loadData(const string& fileDataPath) {
    map<string, string> data;

    ifstream inputFile(fileDataPath);
    if (!inputFile) {
        return data;
    }

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string key, value;
        if (getline(iss, key, '=') && getline(iss, value)) {
            data[key] = value;
        }
    }

    inputFile.close();

    return data;
}

void saveData(const map<string, string>& data, const string& fileDataPath) {
    ofstream outputFile(fileDataPath);
    for (const auto& pair : data) {
        outputFile << pair.first << "=" << pair.second << endl;
    }
    outputFile.close();
}

string getFileExtension(const string& filePath) {
    size_t lastDotPos = filePath.find_last_of(".");

    if (lastDotPos == string::npos)
        return "no_extension";

    return filePath.substr(lastDotPos);
}

string getExecutablePath(const string& extension) {
    cout << "Input path to executable for extension: \"" << extension << "\"" << endl;

    string executable;
    cin >> executable;

    if (executable.empty()) {
        return "start";
    }
    else {
        return executable;
    }
}

void runCommand(const string& cmd) {
    std::system(cmd.c_str());
}

int main(int argc, char** argv) {
    map<string, string> data = loadData(fileDataPath);

    for (int i = 1; i < argc; ++i) {
        string extension = getFileExtension(argv[i]);
        if (data.find(extension) == data.end()) {
            data[extension] = getExecutablePath(extension);
        }

        string cmd = data[extension] + " \"" + argv[i]+'"';

        thread commandThread(runCommand, cmd);
        commandThread.detach();
    }

    saveData(data, fileDataPath);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}