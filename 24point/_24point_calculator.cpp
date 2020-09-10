#include "_24point_calculator.h"

_24point_calculator::_24point_calculator()
{
    goal=24;
    solution_count=0;
    for(int i=0;i<4;i++)
    {
        src[i]=0;
    }
}

_24point_calculator::_24point_calculator(int a,int b,int c,int d,int arg_goal)
{
   //initialize the struct character
//		multiply.c = '*'; over.c = '/'; plus.c = '+'; minus.c = '-';
       src[0] = a; src[1] = b; src[2] = c; src[3] = d;
       solution_count = 0;
       goal = arg_goal;
}

void _24point_calculator::setNumber(int a, int b, int c, int d)
{
    src[0] = a; src[1] = b; src[2] = c; src[3] = d;
}

void _24point_calculator::setGoal(int arg_goal)
{
        goal = arg_goal;
}
    // a core function

bool _24point_calculator::infinitely_enum_and_print(std::string &Answer)
    {
        int tgt[4], help[4];
        memcpy(tgt, src, sizeof(int) * 4);
        bool repeat_signal = true;		bool stage = 0;

        for (int i = 0; i < 24 && repeat_signal; i++)  //24=4Permutation6
        {
            total_permutation(src, tgt, help, 4, i);
            float c1, c2,result;
            int method1, method2, method3; //by these variables , reckon the calculate method
            //2 pair 2
            for (method1=1; method1 <= 6; method1++)
            {
                c1 = SixTypesCalculateMethodResult(tgt[0], tgt[1], method1);
                for (method2 = 1; method2 <= 6; method2++)
                {
                    c2 = SixTypesCalculateMethodResult(tgt[2], tgt[3], method2);
                    for (method3 = 1; method3 <= 6; method3++)
                    {
                        result = SixTypesCalculateMethodResult(c1, c2, method3);
                        if (result == goal)
                        {
                            repeat_signal = false;
                            break;
                        }
                    }
                    if (!repeat_signal)
                    {
                        break;
                    }
                }
                if (!repeat_signal)
                {
                    break;
                }
            }
            //1 pair 3
            if (repeat_signal == true)
            {
                method1 = 1;
                while (method1 <= 6 && repeat_signal)
                {
                    c1 = SixTypesCalculateMethodResult(tgt[0], tgt[1], method1);
                    for (method2 = 1; method2 <= 6 && repeat_signal; method2++)
                    {
                        c2 = SixTypesCalculateMethodResult(c1, tgt[2], method2);
                        method3 = 1;
                        while (method3 <= 6 ) //goal 's type maybe incompatible with float type
                        {
                            result = SixTypesCalculateMethodResult(c2, tgt[3], method3);
                            if (result == goal)
                            {
                                repeat_signal = false;
                                stage = 1;
                                break;
                            }
                            ++method3;
                        }
                        if (!repeat_signal)
                        {
                            break;
                        }
                    }
                    if (!repeat_signal)
                    {
                        break;
                    }
                    method1 += 1;
                }
            }

//            if (EN_display_solution)
//            {
//                if (!repeat_signal && !stage)
//                {

//                    std::cout << print_calculate_method(
//                        print_calculate_method(tgt[0], tgt[1], method1,true),
//                        print_calculate_method(tgt[2], tgt[3], method2,true),
//                        method3, false).c_str();
//                    solution_count++;
//                    return true;
//                }
//                else if (repeat_signal == false && stage)
//                {
//                    std::string str1, str2, str3;
//                    str1 = print_calculate_method(tgt[0], tgt[1], method1,true);
//                    str2 = print_calculate_method(str1, tgt[2], method2,true);
//                    str3 = print_calculate_method(str2, tgt[3], method3,false);
//                    std::cout << str3.c_str();
//                    solution_count++;
//                    return true;
//                }
//            }
//            else
//            {
//                return false;
//            }
                            if (!repeat_signal && !stage)
                            {

                                Answer= print_calculate_method(
                                    print_calculate_method(tgt[0], tgt[1], method1,true),
                                    print_calculate_method(tgt[2], tgt[3], method2,true),
                                    method3, false);

                                return true;
                            }
                            else if (repeat_signal == false && stage)
                            {
                                std::string str1, str2, str3;
                                str1 = print_calculate_method(tgt[0], tgt[1], method1,true);
                                str2 = print_calculate_method(str1, tgt[2], method2,true);
                                str3 = print_calculate_method(str2, tgt[3], method3,false);
                                Answer=str3;

                                return true;
                            }

        }

        //here repeat_signal definitely still is true!!
            Answer="";
            return false;






    }

bool _24point_calculator::infinitely_enum_and_print()
{
    return infinitely_enum_and_print(AnswerText);
}


void _24point_calculator::produce_an_order(Distribution arg_distribution,int rg_low,int rg_up)
    {
        switch(arg_distribution)
        {
        case Poker:
            {
                RandomNumGenerator G1;
                vector<double> PVector1;
                int size=rg_up-rg_low+1;      //size's range is controlled by the parent layer
                PVector1.reserve(size);
                int * Cardcount=new int[size];
                //memset(Cardcount,4,sizeof(int)*size); //why doesn't work?
                for(int i=0;i<size;i++) Cardcount[i]=4;

                int sum=size*4;  //记录牌堆剩余数，纠正概率模型。
                PVector1.resize(13);
                for(int i=0;i<4;i++)
                {
                   for(int j=0;j<size;j++) //make P value
                      PVector1.at(j)=(double)Cardcount[j]/(double)sum;
                   G1.reset_pAry(PVector1);
                   src[i]=G1.GenerateIndex()+rg_low; //shift the index to correct position
                   sum--;
                   Cardcount[src[i]-rg_low+1]--; //easily occur error

                }
                break;
            }
        case EqualProbability:
            {
                {
                    int rangesize=rg_up-rg_low+1;
                    for (int i = 0; i < 4; i++)
                    {
                        src[i] = rand() % rangesize + rg_low;
                    }
                    break;
                }
            }
        case Morphon:   //no repetitive
            {
                RandomNumGenerator G2; vector<double> PVector2;
                int size1=rg_up-rg_low+1;
                PVector2.reserve(size1);
                int sum1=size1;
                PVector2.resize(13);
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<size1;j++)
                        PVector2.at(j)=(double)1/(double)sum1;
                    G2.reset_pAry(PVector2);
                    src[i]=G2.GenerateIndex();
                    sum1--;
                }
                 break;
            }
        }

    }

    int _24point_calculator::getSolutionCount()
    {
        return solution_count;
    }

    int _24point_calculator::ArrangementCount(int m, int n)
    {
        int rm = n - m;
        if (n > rm)
        {
            return ArrangementCount(m - 1, n - 1)*n;
        }
        else
        {
            return 1;
        }
    }

    int _24point_calculator::CombinationCount(int m, int n)
    {
        int x = ArrangementCount(m, n);
        int y=ArrangementCount(m, m);
        return x / y;
    }

    void _24point_calculator::display_current_order()
    {
        std::cout << std::endl << src[0] << " " << src[1] << " " << src[2] <<" "<< src[3] << std::endl;
    }

    float _24point_calculator::SixTypesCalculateMethodResult(float a, float b, int method)
        {
            float result;
            switch (method)
            {
            case 1:
                result = a + b;
                break;
            case 2:
                result = a - b;
                break;
            case 3:
                result = b - a;
                break;
            case 4:
                result = a * b;
                break;
            case 5:
                result = a / b;
                break;
            case 6:
                result = b / a;
                break;
            }
            return result;
        }


        //these three functions help patterned output
        std::string _24point_calculator::print_calculate_method(int  op_a,int  op_b, int method,bool En_brackets)
        {
            std::string s;
            char c_opA[4], c_opB[4];
            sprintf_s(c_opA, "%d", op_a);	sprintf_s(c_opB, "%d", op_b);
            switch (method)
            {
            case 1:
                s.assign("("); s.append(c_opA); s.append("+"); s.append(c_opB); s.append(")");
                break;
            case 2:
                s.assign("("); s.append(c_opA); s.append("-"); s.append(c_opB); s.append(")");
                break;
            case 3:
                s.assign("("); s.append(c_opB); s.append("-"); s.append(c_opA); s.append(")");
                break;
            case 4:
                s.assign("("); s.append(c_opA); s.append("×"); s.append(c_opB); s.append(")");
                break;
            case 5:
                s.assign("("); s.append(c_opA); s.append("÷"); s.append(c_opB); s.append(")");
                break;
            case 6:
                s.assign("("); s.append(c_opB); s.append("÷"); s.append(c_opA); s.append(")");
                break;
            default:
                s = "";
            }
            if (!En_brackets)
            {
                s.pop_back();
                s.erase(s.begin());
            }
            return s;
        }
        std::string _24point_calculator::print_calculate_method(std::string  op_a, std::string  op_b, int method,bool En_brackets)
        {
            std::string s;
                switch (method)
                {
                case 1:
                    s.append("(");
                    s.append(op_a.data());
                    s.append("+");
                    s.append(op_b.data());
                    s.append(")");
                    break;
                case 2:
                    s.append("(");
                    s.append(op_a.data());
                    s.append("-");
                    s.append(op_b.data());
                    s.append(")");
                    break;
                case 3:
                    s.append("(");
                    s.append(op_b.data());
                    s.append("-");
                    s.append(op_a.data());
                    s.append(")");
                    break;
                case 4:
                    s.append("(");
                    s.append(op_a.data());
                    s.append("×");
                    s.append(op_b.data());
                    s.append(")");
                    break;
                case 5:
                    s.append("(");
                    s.append(op_a.data());
                    s.append("÷");
                    s.append(op_b.data());
                    s.append(")");
                    break;
                case 6:
                    s.append("(");
                    s.append(op_b.data());
                    s.append("÷");
                    s.append(op_a.data());
                    s.append(")");
                    break;
                default:
                    s = "";
                }
                if (!En_brackets)
                {
                    s.pop_back();
                    s.erase(s.begin());
                }
            return s;
        }
        std::string _24point_calculator::print_calculate_method(std::string op_a, int op_b, int method,bool En_brackets)
        {
            std::string s;
            char C_opB[10]; sprintf_s(C_opB, "%d", op_b);

            switch (method)
            {
            case 1:
                s.assign("("); s.append(op_a.data()); s.append("+"); s.append(C_opB); s.append(")");
                break;
            case 2:
                s.assign("("); s.append(op_a.data()); s.append("-"); s.append(C_opB); s.append(")");
                break;
            case 3:
                s.assign("("); s.append(C_opB); s.append("-"); s.append(op_a.data()); s.append(")");
                break;
            case 4:
                s.assign("("); s.append(op_a.data()); s.append("×"); s.append(C_opB); s.append(")");
                break;
            case 5:
                s.assign("("); s.append(op_a.data()); s.append("÷"); s.append(C_opB); s.append(")");
                break;
            case 6:
                s.assign("("); s.append(op_a.data()); s.append("÷"); s.append(C_opB); s.append(")");
                break;
            default:
                s = "";
                break;
            }
            if (!En_brackets)
            {
                s.pop_back();
                s.erase(s.begin());
            }
            return s;
        }
        //when phase=0 , return the source array
        //we use phase to avoid the repeated one

        template<typename T>
        void _24point_calculator::total_permutation(T source[], T target[], int select[],  int length, int phase)
        {

            int power = 1;
            for (int stage = 0, n=length; stage < length; stage++,n--)
            {

                while (phase - power * ArrangementCount(n - 1, n - 1) > 0)
                {
                    power++;
                }
                if (phase - power * ArrangementCount(n - 1, n - 1) != 0)
                {
                    power = power - 1;
                }

                phase = phase - power * ArrangementCount(n - 1, n - 1);

                select[stage] = power;
                power = 1;
            }

            std::list<T> choices_list;
            for (int i = 0; i < length; i++, choices_list.push_back(source[i-1]));
            typename std::list<T>::iterator Iter = choices_list.begin(); //typename前缀 暂时不懂
            for (int i = 0; i <length; i++)  //use the select[] array to copy the corresponding element in source into the target
            {
                for (int j = 0; j != select[i]; j++, Iter++);
                target[i] = *Iter;
                choices_list.erase(Iter);
                Iter = choices_list.begin();
            }

        }

        template<typename T>
        void _24point_calculator::combination(const T source[], T target[], int total_N, int select_M, int phase)
        {
            static int select_arrow = 0;	//cannot change without special conditions.
            if (select_M == 0 || total_N == 0){}
            else
            {
                if (phase < CombinationCount(select_M - 1, total_N - 1))
                {
                    target[0] = source[0];
                    combination(&source[1], &target[1], total_N - 1, select_M - 1, phase);
                }
                else
                {
                    combination(&source[1], target, total_N - 1, select_M , phase - CombinationCount(select_M - 1, total_N - 1));
                }
            }
        }



        //calculate the upper bound of the "phase"

        template<typename T>
        void _24point_calculator::print_the_array(T arg_array[], int length)
        {
            std::cout << "\n";
            for (int i = 0; i < length; i++)
            {
                std::cout << " " << arg_array[i];
            }
            std::cout << "\n";
        }

std::string _24point_calculator::getAnswerText() const
{
    return AnswerText;
}

bool _24point_calculator::Source_isEmpty() //for instance, in the future
{                                          //when unlocked the OP. range
    bool check=false;
    for(int i=0;i<4;i++)                   //a new problem arises and the
    {                                      //standard of determined EMPTY
        if(src[i]==0)                      //need to update
        {
            check=true;
            break;
        }
    }
    return check;
}

int _24point_calculator::Operand(int index) const
{
    if(index>4) throw "Access Out of range";
    return src[index-1];
}
