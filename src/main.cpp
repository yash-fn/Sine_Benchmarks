#include <cmath>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ratio>
#include <thread>
#include <vector>

using namespace std;

#define MAX_SIN M_PI

#define NUM_DIVISIONS 1e9

#define INCREMENT MAX_SIN/NUM_DIVISIONS

#define ITERATIONS 7

#define FUNCTIONS_LIST empty, sin

#define _QUOTE(X, ...) #X __VA_OPT__(, QUOTE(__VA_ARGS__))  

#define QUOTE(X, ...) #X __VA_OPT__(, _QUOTE(__VA_ARGS__))  

#define QUOTE_LIST(X) QUOTE(X)

double empty(double x){return double(0);}

constexpr double (*func[])(double) = {FUNCTIONS_LIST};

string func_labels[] = {QUOTE_LIST(FUNCTIONS_LIST)};

constexpr auto num_functions = sizeof(func)/sizeof(*func);

struct results
{
    int64_t averages[num_functions] = { };
    int64_t (*datapoints)[num_functions] = { };
    results(int64_t Iterations) {
        datapoints = new int64_t[Iterations][num_functions]();
    }
    ~results() {
        delete[] datapoints;
    }
};

string function_label = "Function";

auto max_char = [func_labels, num_functions, function_label](){ auto max_char = function_label.length(); for(uint8_t i = 0; i< num_functions; i++) {
    max_char = max(func_labels[i].length(), max_char);
}; return max_char;}();

#define VAR_SPACES(x) cout<<x;for (int i = 0; i < max_char-x.length(); ++i){cout << " " ;}

template <typename Function> 
auto timedfunction(Function &func, double inc, int64_t *datapoint) {

for (uint8_t f = 0; f < num_functions; f++)
{

    auto begin = std::chrono::high_resolution_clock::now();

    for (double x = 0; x < MAX_SIN; x+=inc)
    {
        func[f](x);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    VAR_SPACES(func_labels[f]); cout<<" : "<< std::fixed << std::setprecision(9) << diff/1e9;
    if(!!f) {
        cout<<" ("<< std::fixed << std::setprecision(9) << (diff-datapoint[0])/1e9<<")"<<endl;
    } else {cout<<endl;}

    datapoint[f]=diff;
}

return datapoint;
}

int main(int argc, char *argv[]) {

vector<std::string> arg(argv, argv + argc);

uint16_t Iterations = ITERATIONS;

if (argc > 1)
{
    Iterations = std::stoi( arg[1] );
}

struct results results(Iterations);

cout<<"\nIteration Datapoints: "<<endl;
VAR_SPACES(function_label); cout<<" | Seconds w/1e-9 ns"<<endl;
cout<<"----------------------------"<<endl<<endl;

for(uint_fast32_t i = 0; i < Iterations; i++) {
    
    string nun = "";
    VAR_SPACES(nun); cout<< " " <<i+1<<endl;

    timedfunction(func, INCREMENT, results.datapoints[i]);
    cout<<endl;
    for (size_t j = 0; j < num_functions; j++)
    {
        results.averages[j]+=results.datapoints[i][j]/Iterations;    
    }
    
    
}

cout<<"\nAVERAGE RESULTS:\nIterations: "<<Iterations<<endl; 
VAR_SPACES(function_label); cout<<" | Average seconds"<<endl;
cout<<"----------------------------"<<endl;
for (uint8_t f = 0; f < num_functions; f++)
{
    VAR_SPACES(func_labels[f]); cout<<" : "<< std::fixed << std::setprecision(9) << results.averages[f]/1e9;
    if(!!f) {
        cout<<" ("<< std::fixed << std::setprecision(9) << (results.averages[f]-results.averages[0]) /1e9<<")"<<endl;
    } else {cout<<endl;}
}

cout<<endl;
}