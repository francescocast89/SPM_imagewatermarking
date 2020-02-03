#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <climits>
#include <dirent.h>
#include <algorithm>
#include <numeric>
#include <typeinfo>
#include "CImg.h"
#include "task_queue.cpp"
#include "functions.cpp"
#include "sequentialwatermarking.cpp" 
#include "farmwatermarking.cpp" 
#include "ffwatermarking.cpp" 

char* getCmdOption(char ** begin, char ** end, const std::string & option){
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) return *itr;
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option){
    return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[]){
    
    if (argc < 5) {
        std::cout<<"Error:Few arguments"<<std::endl;
        return 1;
    }
    std::string path_in = argv[1];
    std::string w_path_in = argv[2];
    std::string path_out = argv[3];

    bool compute_avg=false;
    if(cmdOptionExists(argv, argv+argc, "-average"))
    	compute_avg = true;
    int par_deg = 1;
    if(cmdOptionExists(argv, argv+argc, "-nw") )
    	par_deg =  atoi( getCmdOption(argv, argc + argv, "-nw") );
    if(cmdOptionExists(argv, argv+argc, "-seq"))
    	sequentialwatermarking(path_in, w_path_in, path_out,compute_avg);
    else if(cmdOptionExists(argv, argv+argc, "-par"))
    	farmwatermarking(path_in, w_path_in, path_out,compute_avg, par_deg);
    else if(cmdOptionExists(argv, argv+argc, "-ff"))
    	ffwatermarking(path_in, w_path_in, path_out, compute_avg, par_deg);

return 0;
}