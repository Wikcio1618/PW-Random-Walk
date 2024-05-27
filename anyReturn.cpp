#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
using namespace std;

bool isAllZeros(const std::vector<int> &vec)
{
    for (int num : vec)
    {
        if (num != 0)
        {
            return false; // If any element is non-zero, return false
        }
    }
    return true; // All elements are zero
}

int main(int argc, char *argv[])
{
    clock_t start = clock();

    float probDir = 0.5;
    vector<int> dims = {1, 2, 3};

    if (argc != 5)
    {
        cerr << "Error! Provide command line arguments: (file_path, max_time, check_freq, num_agents)" << endl;
        return 1;
    }
    char *outPath = argv[1];
    int maxTime = atoi(argv[2]);
    int checkFreq = atoi(argv[3]);
    int N = atoi(argv[4]);

    ofstream outFile(outPath);
    if (!outFile)
    {
        cerr << "Error: Could not open the file for writing." << endl;
        return 1; // Exit the program with an error code
    }
    outFile << "dimension,time,returned" << endl; 

    // random utilities
    random_device rd;
    mt19937 gen(rd());

    bernoulli_distribution randDirection(probDir);

    for (int d : dims)
    {
        cout << "--------------------------" << endl;
        cout << "Calculations for dimention " << d << endl;

        uniform_int_distribution<> randDim(0, d - 1);
        vector<vector<int>> agents(N, vector<int>(d));
        int returns = 0;

        for (int t = 0; t < maxTime; t++)
        {
            for (int n = 0; n < N; n++)
            {

                int _dim = randDim(gen);
                if (randDirection(gen))
                    agents[n][_dim] += 1;
                else
                    agents[n][_dim] += -1;

                if (!t % checkFreq == 0 && isAllZeros(agents[n]))
                {
                    returns++;
                }
            }

            if (!(t % checkFreq))
            {
                outFile << d << ',' << t << ',' << (float)returns / N << endl;
                returns = 0;
            }
        }
    }

    clock_t end = clock();

    outFile.close();
    std::cout << "Calculations have been saved to " << outPath << std::endl;
    cout << "Execution time: " << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl;

    return 0;
}