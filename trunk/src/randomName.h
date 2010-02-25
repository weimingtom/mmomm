#ifndef RANDOMNAME_H_
#define RANDOMNAME_H_

#include <string>
#include <ctime>
#include <boost/random.hpp>

using namespace std;

class randomNameGenerator
{
public:
    randomNameGenerator();
    ~randomNameGenerator();

    string getRandomName(int nameStyle);
    string getRandomName(const std::string& nameStyle);

private:

    boost::mt19937 generator;

};

#endif
