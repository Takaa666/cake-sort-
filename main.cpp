#include <iostream>
#include <vector>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <algorithm> // for std::shuffle
#include <random>    // for std::default_random_engine

using namespace std;

// Plate structure to hold the array of integers
struct Plate {
    int slots[6];
};

// Function to fill empty slots from adjacent plates
void fillEmptySlots(vector<vector<Plate>>& matrix, size_t i, size_t j) {
    cout << "Filling empty slots for plate at (" << i << ", " << j << ")\n";
    // Checking the right plate
    if (j + 1 < 4) {
        for (size_t k = 0; k < 6; ++k) {
            if (matrix[i][j].slots[k] == 0 && matrix[i][j + 1].slots[k] != 0) {
                matrix[i][j].slots[k] = matrix[i][j + 1].slots[k];
                matrix[i][j + 1].slots[k] = 0;
            }
        }
    }

    // Checking the left plate
    if (j > 0) {
        for (size_t k = 0; k < 6; ++k) {
            if (matrix[i][j].slots[k] == 0 && matrix[i][j - 1].slots[k] != 0) {
                matrix[i][j].slots[k] = matrix[i][j - 1].slots[k];
                matrix[i][j - 1].slots[k] = 0;
            }
        }
    }

    // Checking the bottom plate
    if (i + 1 < 5) {
        for (size_t k = 0; k < 6; ++k) {
            if (matrix[i][j].slots[k] == 0 && matrix[i + 1][j].slots[k] != 0) {
                matrix[i][j].slots[k] = matrix[i + 1][j].slots[k];
                matrix[i + 1][j].slots[k] = 0;
            }
        }
    }

    // Checking the top plate
    if (i > 0) {
        for (size_t k = 0; k < 6; ++k) {
            if (matrix[i][j].slots[k] == 0 && matrix[i - 1][j].slots[k] != 0) {
                matrix[i][j].slots[k] = matrix[i - 1][j].slots[k];
                matrix[i - 1][j].slots[k] = 0;
            }
        }
    }
}

// Function to check and switch numbers between adjacent plates automatically
void checkAndSwitch(vector<vector<Plate>>& matrix, size_t i, size_t j) {
    cout << "Running the switch system for plate at (" << i << ", " << j << ")\n";

    auto switchIfSame = [&](size_t i, size_t j, size_t i1, size_t j1) {
        for (size_t k = 0; k < 6; ++k) {
            if (matrix[i][j].slots[k] != 0) {
                for (size_t l = 0; l < 6; ++l) {
                    if (matrix[i][j].slots[k] == matrix[i1][j1].slots[l] && matrix[i1][j1].slots[l] != 0) {
                        cout << "Filling slot in (" << i1 << ", " << j1 << ") with value from (" << i << ", " << j << ")\n";
                        for (size_t m = 0; m < 6; ++m) {
                            if (matrix[i1][j1].slots[m] == 0) {
                                matrix[i1][j1].slots[m] = matrix[i][j].slots[k];
                                matrix[i][j].slots[k] = 0;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
    };

    // Checking the right plate
    if (j + 1 < 4) {
        switchIfSame(i, j, i, j + 1);
    }

    // Checking the left plate
    if (j > 0) {
        switchIfSame(i, j, i, j - 1);
    }

    // Checking the bottom plate
    if (i + 1 < 5) {
        switchIfSame(i, j, i + 1, j);
    }

    // Checking the top plate
    if (i > 0) {
        switchIfSame(i, j, i - 1, j);
    }
}

int main() {
    vector<vector<Plate>> matrix(5, vector<Plate>(4));

    // Accessing and printing elements
    cout << "Initial Matrix:\n";
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            cout << "[";
            for (size_t k = 0; k < 6; ++k) {
                cout << matrix[i][j].slots[k] << " ";
            }
            cout << "] ";
        }
        cout << endl;
    }

    // Initialize random number generator
    srand(static_cast<unsigned>(time(0)));
    std::random_device rd;
    std::mt19937 g(rd());

    size_t numPlates;
    cout << "Enter the number of plates you want to generate and fill: ";
    cin >> numPlates;

    for (size_t n = 0; n < numPlates; ++n) {
        // Creating a Plate struct and filling it with random numbers between 0 and 6 (0 represents an empty slot)
        Plate plate;
        size_t emptySlots = 0;
        for (size_t i = 0; i < 6; ++i) {
            plate.slots[i] = rand() % 7;
            if (plate.slots[i] == 0) emptySlots++;
        }
        // Ensure at least two zeros
        while (emptySlots < 2) {
            size_t pos = rand() % 6;
            if (plate.slots[pos] != 0) {
                plate.slots[pos] = 0;
                emptySlots++;
            }
        }

        // Displaying the contents of the plate
        cout << "\nPlate " << n + 1 << " contents:\n";
        for (size_t i = 0; i < 6; ++i) {
            cout << plate.slots[i] << " ";
        }
        cout << "\n";

        // Requesting the user to enter table indices
        size_t i, j;
        cout << "Enter the table indices (i and j) you want to fill: ";
        cin >> i >> j;

        // Validating the indices
        if (i >= 5 || j >= 4) {
            cout << "Invalid indices, please enter indices within range: 0 <= i < 5 and 0 <= j < 4\n";
            --n; // Repeat this iteration
            continue;
        }

        // Inserting the contents of the plate into matrix[i][j]
        matrix[i][j] = plate;

        // Filling empty slots from adjacent plates
        fillEmptySlots(matrix, i, j);

        // Check and switch numbers with adjacent plates automatically
        checkAndSwitch(matrix, i, j);
    }

    // Displaying the matrix contents after modification
    cout << "\nMatrix contents after modification:\n";
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            cout << "[";
            for (size_t k = 0; k < 6; ++k) {
                cout << matrix[i][j].slots[k] << " ";
            }
            cout << "] ";
        }
        cout << "\n";
    }

    return 0;
}
