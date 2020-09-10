#include "randomnumgenerator.h"

RandomNumGenerator::RandomNumGenerator()
{
    P_model= ProbabilityModel::Normal;
    cal_mode= CalculateMode::SpaceCritical;
    changed=false;
}


RandomNumGenerator::RandomNumGenerator(vector<double> p_vector)
{
    if(p_vector.empty()) throw "empty probability vector";
    pv=p_vector;
    P_model=ProbabilityModel::Normal;
    cal_mode= CalculateMode::SpaceCritical;
    changed=true;
}

void RandomNumGenerator::reset_pAry(vector<double> new_pVector)
{
    pv=new_pVector;
    changed=true;
}

void RandomNumGenerator::setCalculateMode(CalculateMode mode)
{
    cal_mode=mode;
}

void RandomNumGenerator::setProbalilityModel(ProbabilityModel model)
{
    P_model=model;
}

void RandomNumGenerator::init_rseed(unsigned int arg_seed)
{
    srand(arg_seed);
}

int RandomNumGenerator::GenerateIndex()
{
    return GenerateIndex(cal_mode,P_model);
}

int RandomNumGenerator::GenerateIndex(CalculateMode arg_mode, ProbabilityModel arg_pmodel) //temporary arguments
{
    int index;
    if(arg_mode==SpaceCritical)
    {
        switch(arg_pmodel)
        {
        case ProbabilityModel::Normal:
            index=Generator_SpaceCritical_Normal();
            break;
        default:
            break;
        }
    }
    else
    {
        //... wait to be implemented
        switch(arg_pmodel)
        {
        case ProbabilityModel::Normal:
            index=Generator_TimeCritical_Normal();
            break;
        default:
            break;
        }
    }
    return index;
}


//消耗空间更多的算法：

/*
算法过程: 1. */
int RandomNumGenerator::Generator_SpaceCritical_Normal()
{
    //here i wish maintain a data structure to make searching efficient
    //but it seems that my consideration is needless,hope that
    static int * RangeAryCeil;
    int index=-1;
    if(changed) //reconstruct the range array
    {        
           delete [] RangeAryCeil;
           //malloc
           RangeAryCeil=new int[pv.size()]; //preserve a complement space
           int RangeCounts=pv.size();
           int up=0;   double SumOfP_Vector=0;

            //construct referenced range array
           for(int i=0;i<RangeCounts;i++)
           {
               up+=static_cast<int>(pv.at(i)*RAND_MAX);  //1..2...3...4....... floor int mode
               RangeAryCeil[i]=up;
               SumOfP_Vector+=pv.at(i);    //check the probability vector whether the sum of it is 1 !!
           }

           if(fabs(SumOfP_Vector-1)<=TH) // normalization correction
           {
              RangeAryCeil[RangeCounts-1]=RAND_MAX;  //there may be a little numeric error
           }

          ExtractionLoop:
           int rn=rand();
           for(int i=0;i<RangeCounts;i++) //find the rn's group (alias: index)
           {
               if(rn<RangeAryCeil[i])
               {
                   index=i;
                   break;
               }
           }
           if(index==-1)
           {
               goto ExtractionLoop;
           }

           changed=false;
    }
    else
    {
        //second check
        if(RangeAryCeil==nullptr) throw "RangeAryCeil crashed!";
        ExtractionLoop2:
        int rn=rand();
        int RangeCounts=static_cast<int>(pv.size());
         for(int i=0;i<RangeCounts;i++) //find the rn's group (alias: index)
         {
             if(rn<=RangeAryCeil[i])
             {
                 index=i;
                 break;
             }
         }
         if(index==-1)
         {
             goto ExtractionLoop2;
         }

    }

    return index;

}


//产生一个随机数，消耗时间更多的算法
//数据结构pv, 存储了概率数值的一个向量组。
int RandomNumGenerator::Generator_TimeCritical_Normal() //time become a bottleneck reason of limiting the speed
{
    int index=-1;
    ExtractionLoop3:
    int rn=rand(); int up=0;
    for(int i=0;i<pv.size();i++)
    {
        up+=static_cast<int>(pv.at(i)*RAND_MAX);  //累加up，直到up到了随机数之上。意思就是向量组里已经对RAND_MAX分好了区间，
                                                    //通过计算落点来映射值。
        if(rn<=up)
          index=i;
    }
    if(index=-1)
        goto ExtractionLoop3;
    return index;
}

RandomNumGenerator::~RandomNumGenerator()
{
    pv.~vector();
}

