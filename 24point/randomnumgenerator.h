
#ifndef RANDOMNUMGENERATOR_H
#define RANDOMNUMGENERATOR_H

#include<vector>
#include<cmath>
#include<ctime>
#include<stdlib.h>
#include<algorithm>

using namespace std;

enum CalculateMode{TimeCritical,SpaceCritical};
enum ProbabilityModel{Normal,FixExtraction,Counter,RamdomStep}; //refer to https://www.cnblogs.com/HQFZ/p/5945219.html

class RandomNumGenerator
{
public:
    RandomNumGenerator();
    RandomNumGenerator(vector<double> p_vector);
    //RandomNumGenerator(RandomNumGenerator *Generator_copied);
    ~RandomNumGenerator();
    void reset_pAry(vector<double> new_pVector);
    int GenerateIndex();  //use default settings to generate
    int GenerateIndex(CalculateMode arg_mode,ProbabilityModel arg_pmodel); //use arg. settings and reset mode
    void setCalculateMode(CalculateMode mode);
    void setProbalilityModel(ProbabilityModel model);
    void init_rseed(unsigned int arg_seed);
private:
    //Part of internal logic
    int Generator_SpaceCritical_Normal();
    int Generator_TimeCritical_Normal();
    //
    double TH=1e-6;
    vector<double> pv; bool changed;
    CalculateMode cal_mode;
    ProbabilityModel P_model;

};

#endif // RANDOMNUMGENERATOR_H
