#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <cstdlib> // For std::rand()
#include <ctime>   // For std::time()
#include <iomanip> // For std::setw()
#include <chrono>  // For timing

// Global constants
const int MAX_ITERATIONS = 1000000000;
const int MAX_ELEMENT_VALUE = 10;

// Mutex for synchronization
std::mutex coutMutex{};

// Thread-safe increment function
void threadSafeIncrement(long long &value) {
    for (int i = 0; i < MAX_ITERATIONS; ++i) {
        std::lock_guard<std::mutex> lock(coutMutex);
        ++value;
    }
}

// Thread-unsafe increment function
void threadUnsafeIncrement(long long &value) {
    for (int i = 0; i < MAX_ITERATIONS; ++i) {
        ++value;
    }
}

// Function to generate a random matrix
std::vector<std::vector<int>> generateRandomMatrix(int rows, int columns) {
    std::vector<std::vector<int>> result(rows, std::vector<int>(columns));
    for (auto &row: result) {
        for (auto &element: row) {
            element = std::rand() % MAX_ELEMENT_VALUE;
        }
    }
    return result;
}

// Function to print a matrix
void printMatrix(const std::vector<std::vector<int>> &matrix) {
    for (const auto &row: matrix) {
        for (const auto &element: row) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << std::setw(4) << element << " ";
        }
        std::cout << "\n";
    }
}

// Function to compute a single element of the resulting matrix
void computeElement(int &result, const std::vector<std::vector<int>> &firstMatrix, const std::vector<std::vector<int>> &secondMatrix, int row, int col) {
    if (firstMatrix[row].size() != secondMatrix.size()) {
        exit(-2); // Error handling: Incompatible matrix dimensions
    }
    result = 0;
    int size = firstMatrix[row].size();
    for (int i = 0; i < size; ++i) {
        result += firstMatrix[row][i] * secondMatrix[i][col];
    }
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "[" << row << "," << col << "] = " << result << std::endl;
    }
}

int main(int argc, char **argv) {
    // Check for correct usage
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <rows> <cols> <cols>" << std::endl;
        return -1; // Incorrect number of arguments
    }

    // Parse command-line arguments
    int rows = std::stoi(argv[1]);
    int cols1 = std::stoi(argv[2]);
    int cols2 = std::stoi(argv[3]);

    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate random matrices
    std::vector<std::vector<int>> matrix1 = generateRandomMatrix(rows, cols1);
    std::vector<std::vector<int>> matrix2 = generateRandomMatrix(cols1, cols2);

    // Print the generated matrices
    std::cout << "Matrix1:" << std::endl;
    printMatrix(matrix1);
    std::cout << "\nMatrix2:" << std::endl;
    printMatrix(matrix2);
    std::cout << std::endl;

    // Mutex
    auto start = std::chrono::high_resolution_clock::now();
    long long sharedVarMutex = 0;
    std::thread thread1(threadSafeIncrement, std::ref(sharedVarMutex));
    std::thread thread2(threadSafeIncrement, std::ref(sharedVarMutex));
    thread1.join();
    thread2.join();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeDifferenceMutex = end - start;
    std::cout << "Safe increment: " << sharedVarMutex << " Delta: " << timeDifferenceMutex.count() << std::endl;

    // Without Mutex
    start = std::chrono::high_resolution_clock::now();
    long long sharedVarNoMutex = 0;
    std::thread thread3(threadUnsafeIncrement, std::ref(sharedVarNoMutex));
    std::thread thread4(threadUnsafeIncrement, std::ref(sharedVarNoMutex));
    thread3.join();
    thread4.join();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeDifferenceNoMutex = end - start;
    std::cout << "Unsafe increment: " << sharedVarNoMutex << " Delta: " << timeDifferenceNoMutex.count() << std::endl;

    return 0;
}

