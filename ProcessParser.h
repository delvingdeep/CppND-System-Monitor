#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "constants.h"


using namespace std;

class ProcessParser{
    private:
        std::ifstream stream;
        
    public:
        static string getCmd(string pid);
        static vector<string> getPidList();
        static std::string getVmSize(string pid);
        static std::string getCpuPercent(string pid);
        static long int getSysUpTime();
        static std::string getProcUpTime(string pid);
        static string getProcUser(string pid);
        static vector<string> getSysCpuPercent(string coreNumber = "");
        static float getSysRamPercent();
        static string getSysKernelVersion();
        static int getTotalThreads();
        static int getTotalNumberOfProcesses();
        static int getNumberOfRunningProcesses();
        static string getOSName();
        static std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);
        static bool isPidExisting(string pid);
};

// TODO: Define all of the above functions below:
string ProcessParser::getVmSize(string pid) {
    string line;

    // declare name to search in file
    string name = "VmData"; // in case of doubt, verify it from "cat /proc/pid#/status"

    float result;

    // open stream for specific file
    std::ifstream stream;
    Util::getStream((Path::basePath() + pid + Path::statusPath()), stream);

    while(getline(stream, line)) {

        // searching line by line
        if(line.compare(0, name.size(), name) == 0) {

            //slicing string line on ws for values using sstream
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);

            // conversion of kB -> GB
            result = stof(values[1])/float(1024);
            break;
        }
    }
    return to_string(result);
}

std::string ProcessParser::getCpuPercent(string pid) {
    string line;
    float result;

    // open stream for specific file
    std::ifstream stream;
    Util::getStream((Path::basePath() + pid + "/" + Path::statPath()), stream);

    getline(stream, line);

    string str = line;

    //slicing string line on ws for values using sstream
    istringstream buf(str);        
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end);

    // acquring revelant time for calculation of CPU usage for selected process
    float utime = stof(ProcessParser::getProcUpTime(pid));

    float stime = stof(values[14]);
    float cutime = stof(values[15]);
    float cstime = stof(values[16]);
    float starttime = stof(values[21]);

    float uptime = ProcessParser::getSysUpTime();
    float freq = sysconf(_SC_CLK_TCK);

    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/freq);
    result = 100.0*((total_time/freq)/seconds);

    return to_string(result);
}
