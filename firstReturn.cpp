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
    vector<int> dims = {2};
    vector<long> timers = {0 /* , (int)pow(10, 1), (int)pow(10, 3), (int)pow(10, 5), (int)pow(10, 7), (int)pow(10, 8),  */, (long)pow(10, 10), (long)pow(10, 11)};

    if (argc != 3)
    {
        cerr << "Error! Provide command line arguments: (file_path, num_agents)" << endl;
        return 1;
    }
    char *outPath = argv[1];
    int N = atoi(argv[2]);

    ofstream outFile(outPath);
    if (!outFile)
    {
        cerr << "Error: Could not open the file for writing." << endl;
        return 1; // Exit the program with an error code
    }

    // random utilities
    random_device rd;
    mt19937 gen(rd());

    bernoulli_distribution randDirection(probDir);

    int _counter = 0;
    for (int d : dims)
    {
        cout << "--------------------------" << endl;
        cout << "Calculations for dimention " << d << endl;

        uniform_int_distribution<> randDim(0, d - 1);
        vector<vector<int>> agents(N, vector<int>(d));
        vector<bool> hasReturned(N, false);

        for (uint t = 1; t < timers.size(); t++)
        {
            cout << "Starting time: " << timers[t] << endl;

            int timeLimit = timers[t] - timers[t - 1];

            for (int n = 0; n < N; n++)
            {
                _counter = 0;

                if (hasReturned[n])
                    continue;

                do
                {
                    int _dim = randDim(gen);
                    if (randDirection(gen))
                        agents[n][_dim] += 1;
                    else
                        agents[n][_dim] += -1;

                    if (isAllZeros(agents[n]))
                    {
                        hasReturned[n] = 1;
                        break;
                    }
                    _counter++;
                } while (_counter < timeLimit);
            }

            int numReturned = 0;
            for (int i : hasReturned)
            {
                numReturned += i;
            }
            outFile << d << ',' << timers[t] << ',' << (float)numReturned / N << endl;
        }
    }

    clock_t end = clock();

    outFile.close();
    std::cout << "Calculations have been saved to " << outPath << std::endl;
    cout << "Execution time: " << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl;

    return 0;
}