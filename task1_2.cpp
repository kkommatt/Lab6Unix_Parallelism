#include <iostream>
#include <vector>
#include <cstdlib> // For std::rand()
#include <ctime>   // For std::time()
#include <mutex>
#include <thread>
#include <iomanip> // For std::setw()
#include <chrono>


int BIGGEST_POSSIBLE_ELEMENT = 10;

std::mutex coutMutex{}; // Mutex for cout synchronization

// Function to generate a random matrix
std::vector<std::vector<int>> generateRandomMatrix(int rows, int columns) {
    std::vector<std::vector<int>> result(rows, std::vector<int>(columns));
    for (auto &row: result) {
        for (auto &el: row) {
            el = std::rand() % BIGGEST_POSSIBLE_ELEMENT;
        }
    }
    return result;
}

// Function to print a matrix
void printMatrix(const std::vector<std::vector<int>> &matrix) {
    for (const auto &row: matrix) {
        for (const auto &el: row) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << std::setw(4) << el << " ";
        }
        std::cout << "\n";
    }
}

// Function to compute a single element of the resulting matrix
void computeElement(int &result, const std::vector<std::vector<int>> &first, const std::vector<std::vector<int>> &second, int row, int col) {
    if (first[row].size() != second.size()) {
        exit(-2); // Error handling: Incompatible matrix dimensions
    }
    result = 0;
    int m = first[row].size();
    for (int i = 0; i < m; ++i) {
        result += first[row][i] * second[i][col];
    }
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "[" << row << "," << col << "] = " << result << "\n";
    }
}

int main(int argc, char **argv) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <rows> <cols> <cols> <max_threads>" << std::endl;
        return -1; // Incorrect number of arguments
    }

    // Parse command-line arguments
    int n = std::stoul(argv[1]);
    int m = std::stoul(argv[2]);
    int k = std::stoul(argv[3]);
    int maxThreads = std::stoul(argv[4]);

    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate random matrices
    std::vector<std::vector<int>> first = generateRandomMatrix(n, m);
    std::vector<std::vector<int>> second = generateRandomMatrix(m, k);
    


    // Print the generated matrices
    std::cout << "Matrix1:" << std::endl;
    printMatrix(first);
    
    std::cout << std::endl;
    
    std::cout << "Matrix2:" << std::endl;
    printMatrix(second);
    std::cout << std::endl;
    
    // Now
    auto start = std::chrono::high_resolution_clock::now();
    
    // Initialize threads vector
    std::vector<std::thread> threads;
    
    // Initialize result matrix
    std::vector<std::vector<int>> result(n, std::vector<int>(k));
        
    int threadsCount = std::min(maxThreads, n * k);


    
    std::cout << "Finding each element:" << std::endl;
    
    // Start threads to compute elements of the result matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            if (threads.size() < threadsCount) {
                threads.emplace_back(computeElement, std::ref(result[i][j]), std::cref(first), std::cref(second), i, j);
            } else {
                computeElement(result[i][j], first, second, i, j); 
            }
        }
    }

    // Wait for all threads to finish
    for (auto &thread: threads) {
        thread.join();
    }
    // Now
    auto end = std::chrono::high_resolution_clock::now();
    
    // Delta-time
    std::chrono::duration<double, std::milli> delta = end - start;
    
    std::cout << std::endl;
    
    // Print the resulting matrix
    std::cout << "Result:" << std::endl;
    printMatrix(result);
    
    std::cout << "Threads: " << threadsCount << " Delta-time: " << delta.count() << std::endl;

    return 0;
}

