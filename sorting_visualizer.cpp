// ============================================================
//   SORTING VISUALIZER - C++ Console Application
//   Supports: Bubble, Selection, Insertion, Merge, Quick,
//             Counting, Radix Sort
//   Features: Comparisons, Swaps, Time (chrono), Stability,
//             Step-by-step visualization
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;
using namespace chrono;

// ─────────────────────────────────────────────
//  Global counters (reset before each sort)
// ─────────────────────────────────────────────
long long comparisons = 0;
long long swaps       = 0;
bool      stepMode    = false;   // toggle step-by-step output

// ─────────────────────────────────────────────
//  Utility helpers
// ─────────────────────────────────────────────

void resetCounters() {
    comparisons = 0;
    swaps       = 0;
}

// Print the current state of the vector
void printArray(const vector<int>& arr, const string& label = "") {
    if (!label.empty()) cout << label << ": ";
    for (int v : arr) cout << setw(4) << v;
    cout << "\n";
}

// Pause for step-by-step mode
void stepPause() {
    if (!stepMode) {
        cout << "  [Press ENTER for next step]";
        cin.get();
    }
}

// Print a horizontal divider
void divider(char c = '-', int n = 56) {
    cout << string(n, c) << "\n";
}

// ─────────────────────────────────────────────
//  1. BUBBLE SORT
//  Repeatedly swaps adjacent elements that are
//  out of order. Stable. O(n²) average.
// ─────────────────────────────────────────────
void bubbleSort(vector<int> arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            ++comparisons;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                ++swaps;
                swapped = true;
                if (stepMode) printArray(arr, "  Pass " + to_string(i + 1));
                stepPause();
            }
        }
        if (!swapped) break;   // early exit if already sorted
    }
    printArray(arr, "Sorted ");
}

// ─────────────────────────────────────────────
//  2. SELECTION SORT
//  Finds the minimum element and places it at
//  the correct position each pass. NOT stable.
// ─────────────────────────────────────────────
void selectionSort(vector<int> arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            ++comparisons;
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            swap(arr[i], arr[minIdx]);
            ++swaps;
        }
        if (stepMode) {
            printArray(arr, "  Pass " + to_string(i + 1));
            stepPause();
        }
    }
    printArray(arr, "Sorted ");
}

// ─────────────────────────────────────────────
//  3. INSERTION SORT
//  Builds a sorted sub-array by inserting each
//  element into its correct position. Stable.
// ─────────────────────────────────────────────
void insertionSort(vector<int> arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j   = i - 1;
        while (j >= 0) {
            ++comparisons;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                ++swaps;
                --j;
            } else break;
        }
        arr[j + 1] = key;
        if (stepMode) {
            printArray(arr, "  Step " + to_string(i));
            stepPause();
        }
    }
    printArray(arr, "Sorted ");
}

// ─────────────────────────────────────────────
//  4. MERGE SORT (recursive)
//  Divides the array in half, sorts each half,
//  then merges. Stable. O(n log n).
// ─────────────────────────────────────────────
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < (int)left.size() && j < (int)right.size()) {
        ++comparisons;
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
            ++swaps;   // conceptual swap count for merge
        }
    }
    while (i < (int)left.size()) arr[k++] = left[i++];
    while (j < (int)right.size()) arr[k++] = right[j++];
}

void mergeSortHelper(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSortHelper(arr, l, m);
    mergeSortHelper(arr, m + 1, r);
    merge(arr, l, m, r);
    if (stepMode) {
        printArray(arr, "  Merge[" + to_string(l) + ".." + to_string(r) + "]");
        stepPause();
    }
}

void mergeSort(vector<int> arr) {
    mergeSortHelper(arr, 0, (int)arr.size() - 1);
    printArray(arr, "Sorted ");
}

// ─────────────────────────────────────────────
//  5. QUICK SORT (Lomuto partition)
//  Picks a pivot, partitions around it, and
//  recurses. NOT stable. O(n log n) average.
// ─────────────────────────────────────────────
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i     = low - 1;
    for (int j = low; j < high; ++j) {
        ++comparisons;
        if (arr[j] <= pivot) {
            ++i;
            swap(arr[i], arr[j]);
            ++swaps;
        }
    }
    swap(arr[i + 1], arr[high]);
    ++swaps;
    return i + 1;
}

void quickSortHelper(vector<int>& arr, int low, int high) {
    if (low >= high) return;
    int pi = partition(arr, low, high);
    if (stepMode) {
        printArray(arr, "  Partition[" + to_string(low) + ".." + to_string(high) + "] pivot@" + to_string(pi));
        stepPause();
    }
    quickSortHelper(arr, low, pi - 1);
    quickSortHelper(arr, pi + 1, high);
}

void quickSort(vector<int> arr) {
    quickSortHelper(arr, 0, (int)arr.size() - 1);
    printArray(arr, "Sorted ");
}

// ─────────────────────────────────────────────
//  6. COUNTING SORT
//  Counts occurrences of each value. Stable.
//  Only works for non-negative integers.
//  O(n + k) where k = range of values.
// ─────────────────────────────────────────────
void countingSort(vector<int> arr) {
    if (arr.empty()) { cout << "Empty array.\n"; return; }
    int maxVal = *max_element(arr.begin(), arr.end());
    int minVal = *min_element(arr.begin(), arr.end());

    if (minVal < 0) {
        cout << "  [!] Counting Sort requires non-negative integers. Skipping.\n";
        return;
    }

    vector<int> count(maxVal + 1, 0);
    for (int v : arr) { count[v]++; ++comparisons; }

    vector<int> sorted;
    sorted.reserve(arr.size());
    for (int v = 0; v <= maxVal; ++v) {
        while (count[v]-- > 0) {
            sorted.push_back(v);
            ++swaps;
            if (stepMode) {
                printArray(sorted, "  Building");
                stepPause();
            }
        }
    }
    printArray(sorted, "Sorted ");
}

// ─────────────────────────────────────────────
//  7. RADIX SORT (LSD, using counting sort)
//  Sorts digit by digit from least significant.
//  Stable. O(d * (n + k)).
// ─────────────────────────────────────────────
void countingSortByDigit(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    vector<int> count(10, 0);

    for (int i = 0; i < n; ++i) { count[(arr[i] / exp) % 10]++; ++comparisons; }
    for (int i = 1; i < 10; ++i) count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; --i) {
        output[--count[(arr[i] / exp) % 10]] = arr[i];
        ++swaps;
    }
    arr = output;
}

void radixSort(vector<int> arr) {
    if (arr.empty()) { cout << "Empty array.\n"; return; }
    int minVal = *min_element(arr.begin(), arr.end());
    if (minVal < 0) {
        cout << "  [!] Radix Sort requires non-negative integers. Skipping.\n";
        return;
    }
    int maxVal = *max_element(arr.begin(), arr.end());
    int pass   = 1;
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSortByDigit(arr, exp);
        if (stepMode) {
            printArray(arr, "  Digit pass " + to_string(pass++));
            stepPause();
        }
    }
    printArray(arr, "Sorted ");
}

// ─────────────────────────────────────────────
//  Run a chosen sort and display stats
// ─────────────────────────────────────────────
struct AlgoInfo {
    string name;
    bool   stable;
    string complexity;
};

AlgoInfo getAlgoInfo(int choice) {
    switch (choice) {
        case 1: return {"Bubble Sort",    true,  "O(n²)"      };
        case 2: return {"Selection Sort", false, "O(n²)"      };
        case 3: return {"Insertion Sort", true,  "O(n²)"      };
        case 4: return {"Merge Sort",     true,  "O(n log n)" };
        case 5: return {"Quick Sort",     false, "O(n log n) avg"};
        case 6: return {"Counting Sort",  true,  "O(n + k)"   };
        case 7: return {"Radix Sort",     true,  "O(d·(n+k))" };
        default: return {"Unknown", false, "?"};
    }
}

void runSort(const vector<int>& data, int choice) {
    AlgoInfo info = getAlgoInfo(choice);
    divider('=');
    cout << "  Algorithm : " << info.name << "\n";
    cout << "  Complexity: " << info.complexity << "\n";
    cout << "  Stable    : " << (info.stable ? "Yes" : "No") << "\n";
    divider();

    resetCounters();

    auto start = high_resolution_clock::now();

    switch (choice) {
        case 1: bubbleSort(data);    break;
        case 2: selectionSort(data); break;
        case 3: insertionSort(data); break;
        case 4: mergeSort(data);     break;
        case 5: quickSort(data);     break;
        case 6: countingSort(data);  break;
        case 7: radixSort(data);     break;
    }

    auto end     = high_resolution_clock::now();
    double ms    = duration<double, milli>(end - start).count();

    divider();
    cout << "  Comparisons: " << comparisons  << "\n";
    cout << "  Swaps      : " << swaps       << "\n";
    cout << "  Time taken : " << ms << " ms\n";
    cout << "STATS|" << info.name << "|"
     << comparisons << "|"
     << swaps << "|"
     << fixed << setprecision(4) << ms << "|"
     << (info.stable ? "Yes" : "No") << "\n";
    divider('=');
}

// ─────────────────────────────────────────────
//  Dataset loading
// ─────────────────────────────────────────────

// Preset datasets
vector<int> loadPreset(int type) {
    switch (type) {
        case 1: // Marks (0-100)
            return {78, 91, 45, 62, 88, 34, 99, 55, 72, 60,
                    83, 47, 91, 25, 76, 50, 68, 89, 41, 95};
        case 2: // Sales figures
            return {15200, 8750, 23100, 4500, 19800, 31200, 7600,
                    28400, 12000, 5100, 17600, 9800, 22400, 3300};
        case 3: // Log severity codes (0-5)
            return {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3, 2, 3, 8, 4};
        default:
            return {};
    }
}

// Load integers from a text file (one per line or space-separated)
vector<int> loadFromFile(const string& filename) {
    ifstream fin(filename);
    vector<int> data;
    if (!fin) {
        cout << "  [!] Cannot open file: " << filename << "\n";
        return data;
    }
    int v;
    while (fin >> v) data.push_back(v);
    cout << "  Loaded " << data.size() << " values from '" << filename << "'.\n";
    return data;
}

// Manual entry
vector<int> loadManual() {
    vector<int> data;
    cout << "  Enter integers separated by spaces (then press ENTER):\n  > ";
    string line;
    cin.ignore();
    getline(cin, line);
    istringstream iss(line);
    int v;
    while (iss >> v) data.push_back(v);
    return data;
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main(int argc, char* argv[]) {
    if (argc >= 5) {
    string dataStr, algoStr;
    for (int i = 1; i < argc - 1; ++i) {
        if (string(argv[i]) == "--data") dataStr = argv[i + 1];
        if (string(argv[i]) == "--algo") algoStr = argv[i + 1];
    }

    vector<int> data;
    istringstream iss(dataStr);
    int v;
    while (iss >> v) data.push_back(v);

    stepMode = true;   // important
    runSort(data, stoi(algoStr));
    return 0;
}
    cout << "\n";
    divider('*');
    cout << "       S O R T I N G   V I S U A L I Z E R\n";
    divider('*');

    vector<int> data;
    bool running = true;

    while (running) {
        // ── Load data menu ──────────────────────────
        cout << "\n[1] Dataset Menu\n";
        divider();
        cout << "  1. Marks dataset    (20 values, 0-100)\n";
        cout << "  2. Sales dataset    (14 values)\n";
        cout << "  3. Log codes dataset(20 values, 0-9)\n";
        cout << "  4. Load from file\n";
        cout << "  5. Enter manually\n";
        if (!data.empty())
            cout << "  6. Keep current dataset (" << data.size() << " values)\n";
        cout << "  0. Exit\n";
        cout << "  Choice: ";

        int dc; cin >> dc;
        if (dc == 0) { cout << "Goodbye!\n"; break; }

        if      (dc >= 1 && dc <= 3) data = loadPreset(dc);
        else if (dc == 4) {
            string fname;
            cout << "  Filename: "; cin >> fname;
            data = loadFromFile(fname);
        }
        else if (dc == 5) data = loadManual();
        else if (dc == 6 && !data.empty()) { /* keep */ }
        else { cout << "  Invalid choice.\n"; continue; }

        if (data.empty()) { cout << "  Dataset is empty, please try again.\n"; continue; }

        printArray(data, "\n  Dataset");

        // ── Step mode ───────────────────────────────
        cout << "\n  Enable step-by-step visualization? (y/n): ";
        char sv; cin >> sv;
        stepMode = (sv == 'y' || sv == 'Y');

        // ── Algorithm menu ──────────────────────────
        bool sortAgain = true;
        while (sortAgain) {
            cout << "\n[2] Choose Sorting Algorithm\n";
            divider();
            cout << "  1. Bubble Sort\n";
            cout << "  2. Selection Sort\n";
            cout << "  3. Insertion Sort\n";
            cout << "  4. Merge Sort\n";
            cout << "  5. Quick Sort\n";
            cout << "  6. Counting Sort  (non-negative integers only)\n";
            cout << "  7. Radix Sort     (non-negative integers only)\n";
            cout << "  8. Run ALL algorithms & compare\n";
            cout << "  0. Back to dataset menu\n";
            cout << "  Choice: ";

            int ac; cin >> ac;
            if (ac == 0) { sortAgain = false; break; }

            if (ac >= 1 && ac <= 7) {
                runSort(data, ac);
            } else if (ac == 8) {
                cout << "\n  Running all algorithms on same dataset...\n";
                bool savedStep = stepMode;
                stepMode = false;   // disable step-by-step for batch run
                for (int i = 1; i <= 7; ++i) runSort(data, i);
                stepMode = savedStep;
            } else {
                cout << "  Invalid choice.\n";
            }

            cout << "\n  Sort again with this dataset? (y/n): ";
            char again; cin >> again;
            if (again != 'y' && again != 'Y') sortAgain = false;
        }
    }

    return 0;
}
