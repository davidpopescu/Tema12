
#include <fstream>
#include <cstdlib>
#include <ctime>

void generateAndWriteToFile(int n, const std::string& filename) {
    std::ofstream outFile(filename);

    std::srand(static_cast<unsigned int>(std::time(0)));

    for (int i = 0; i < n; i++) {
        outFile << std::rand() % 10001 << " "; 
    }

    outFile << std::endl;
    outFile.close();
}

int main() {
    int n = 1000000;
    std::string filename = "input.txt";

    generateAndWriteToFile(n, filename);

    return 0;
}
