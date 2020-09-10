#ifndef M_RECORD_H
#define M_RECORD_H
#include<fstream>
#include<ctime>
#include<string>
#include<queue>
#include<iostream>
#include<windows.h> //this makes cross-platforms disabled
#include<ShlObj.h>  //SHGetSpecialFolderPath
using namespace std;

struct m_Entry{
    int op[4];
    char solution[128];
    char t[128];
};

#define DEFAULTPATH "24dotrecord.dat"  //still has some problems here
//#define ALTERNATIVEPATH "C:\user\24dotrecord.txt"

class M_Record
{
public:
    M_Record();
    bool WriteHistory();
    bool ReadHistory();
    void RecordInBuffer(int operand[4],string arg_solution,time_t ts); //contain solution process secondly
    void ClearWriteBuffer();
    void DeleteRecordInDisk();
    queue<m_Entry>* getReadBuffer();
    queue<m_Entry>* getWriteBuffer();
    bool Is_Read();
    bool ChangeStoragePath(const char* pathname);

private:
    const char * getStoragePath();
    string getLocaltime(time_t ts);
    queue<m_Entry> queue_w; //the queue ready to write
    queue<m_Entry> queue_r; //the queue ready out to process
    //bool is_read;
    char path[256];
};

#endif // M_RECORD_H
