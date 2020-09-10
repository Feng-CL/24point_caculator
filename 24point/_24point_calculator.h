#ifndef _24POINT_CALCULATOR_H
#define _24POINT_CALCULATOR_H
#include"Global_marco.h"
#include"randomnumgenerator.h"


enum Distribution{
    Poker,Morphon, EqualProbability
};

class _24point_calculator
{
public:
    _24point_calculator();
    _24point_calculator(int a, int b , int c ,int d,int arg_goal) ;
    void setNumber(int a, int b, int c, int d);
    void setGoal(int arg_goal);
    bool infinitely_enum_and_print();
    bool infinitely_enum_and_print(std::string &Answer);
    void produce_an_order(Distribution arg_distribution,int rg_low,int rg_up);  //linear 1-13 not poker distribution
    int getSolutionCount();
    int ArrangementCount(int m, int n);
    int CombinationCount(int m, int n);
    void display_current_order();
    std::string getAnswerText() const;
    bool Source_isEmpty();
    int Operand(int index)const;



private:
    float SixTypesCalculateMethodResult(float a, float b, int method);
    std::string print_calculate_method(int  op_a,int  op_b, int method,bool En_brackets);
    std::string print_calculate_method(std::string  op_a, std::string  op_b, int method,bool En_brackets);
    std::string print_calculate_method(std::string op_a, int op_b, int method,bool En_brackets);



    template<typename T>
    void total_permutation(T source[], T target[], int select[],  int length, int phase);
    template<typename T>
    void combination(const T source[], T target[], int total_N, int select_M, int phase);
    template<typename T>
    void print_the_array(T arg_array[], int length);
    //operators:
    //struct operators { char c; } multiply, over , plus , minus;
    int src[4];
    int goal; //expand the utilities of my class
    int solution_count;
    std::string AnswerText;


};

#endif // _24POINT_CALCULATOR;
