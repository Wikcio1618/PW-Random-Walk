#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
using namespace std;

int main(int argc, char *argv[])
{

    clock_t start = clock();

    float probDir = 0.5;
    vector<int> dims = {1, 2, 3, 4};
    vector<long> timers = {0, (int)pow(10, 2), (int)pow(10, 4), (int)pow(10, 6)};

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

        for (uint t = 1; t < timers.size(); t++)
        {
            cout << "Starting time: " << timers[t] << endl;

            int timeLimit = timers[t] - timers[t - 1];

            for (int n = 0; n < N; n++)
            {
                _counter = 0;

                do
                {
                    int _dim = randDim(gen);
                    if (randDirection(gen))
                        agents[n][_dim] += 1;
                    else
                        agents[n][_dim] += -1;

                    _counter++;
                } while (_counter < timeLimit);
            }

            for (int i = 0; i < N; i++)
                for (int j = 0; j < i; j++)
                {
                    float distance = 0;
                    for (int k = 0; k < d; k++)
                        distance += pow((agents[i][k] - agents[j][k]), 2);
                    outFile << d << ',' << timers[t] << ',' << sqrt(distance) << endl;
                    
                }
        }
    }

    clock_t end = clock();

    outFile.close();
    std::cout << "Calculations have been saved to " << outPath << std::endl;
    cout << "Execution time: " << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl;

    return 0;
}