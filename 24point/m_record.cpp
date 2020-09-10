#include "m_record.h"

M_Record::M_Record()
{
    //check the record whether it's still in position, if not ,then create a new file and open it
    strcpy(path,getStoragePath());
    if(path==nullptr)
    {
       strcpy(path,DEFAULTPATH); //safety operation
    }

    ReadHistory();
}

void M_Record::RecordInBuffer(int operand[4], string arg_solution, time_t ts)
{
    m_Entry t_entry;
    memcpy(t_entry.op,operand,sizeof(int)*4);
    strcpy_s(t_entry.solution,arg_solution.c_str());
    strcpy_s(t_entry.t,getLocaltime(ts).c_str());
    queue_w.push(t_entry);
}

string M_Record::getLocaltime(time_t ts)
{
    string str_time;
    char buff[64]; tm out;
    localtime_s(&out,&ts);
    asctime_s(buff,&out);
    str_time.assign(buff);
    str_time.pop_back();
    return str_time;
}

void M_Record::ClearWriteBuffer()
{
    while(!queue_w.empty())  //pop the queue until it becomes empty
    {
        queue_w.pop();
    }
}

void M_Record::DeleteRecordInDisk()
{
    cerr<<remove(DEFAULTPATH);
    ClearWriteBuffer();
    //is_read=false;
}

queue<m_Entry> * M_Record::getReadBuffer()
{
   return &queue_r;
}

queue<m_Entry> * M_Record::getWriteBuffer()
{
    return &queue_w;
}

bool M_Record::ReadHistory() //too many unsafety operation
{

    ifstream inData(path,ios::in|ios::binary);
    if(!inData)
    {
        cerr<<"file could not be opened"<<endl;
        return false;
    }
//    else
//    {
//        inData.open(ALTERNATIVEPATH);
//        if(!inData)
//        {
//            cerr<<"File could not be opened"<<endl;
//            return false;
//        }
//    }

    m_Entry t_entry;
    inData.seekg(0,ios::beg);
   // cerr<<"why not display"<<endl;
    while(!(inData.peek()==EOF))
    {
        inData.read(reinterpret_cast<char *>(&t_entry),sizeof(t_entry));//空字符串引起的意外,读取失败
//        if(inData.peek()==static_cast<char>('\n'))
//        {
//            inData.seekg(1,ios::cur); //jump the substring: "\n" ,just try
//        }
        queue_r.push(t_entry);

      //  cerr<<inData.tellg();
    }

    inData.close(); //close the file for write
    return true;
}

bool M_Record::WriteHistory()
{

    ofstream outputTofile; outputTofile.open(path,ios::app|ios::binary);  //what if there is no specific: ios::bin
    if(!outputTofile)
    {
       // outputTofile.open(ALTERNATIVEPATH,ios::app); //check another path
       // if(!outputTofile)
        {
        cerr<<"File could not be opened " <<endl;  //still fail
        return false;
        }
    }

    while(!queue_w.empty())
    {
        m_Entry t_entry=queue_w.front();
        outputTofile.write(reinterpret_cast<const char *>(&t_entry),sizeof(t_entry)); //pop out of buffer one by one
        //outputTofile.put('\n');  //try
        queue_w.pop();
    }
    outputTofile.close();
    return true;
}

bool M_Record::Is_Read()
{
    return !queue_r.empty();
}

bool M_Record::ChangeStoragePath(const char *pathname)
{
    char temp[256];
    memcpy_s(temp,sizeof(pathname),pathname,sizeof(pathname));
    if(temp==nullptr)
    {
        return false;
    }
    else
    {
        memcpy_s(path,128,temp,sizeof(temp));
        return true;
    }
}

const char * M_Record::getStoragePath()  //a plenty of MRSCO inburst
{
    TCHAR pathbuffer[100];
    BOOL checkReturn=SHGetSpecialFolderPath(NULL,pathbuffer,CSIDL_APPDATA,FALSE); //windows API
    string fullpath;
    if(checkReturn==TRUE)
    {
        //continue to expand the fold directory
        int T_length=WideCharToMultiByte(CP_ACP,NULL,pathbuffer,-1,NULL,0,NULL,NULL);//无法判断T_length是字节数还是字符数
        char *_charbuffer=new char[T_length];
        WideCharToMultiByte(CP_ACP,0,pathbuffer,-1,_charbuffer,T_length,NULL,NULL);

        fullpath.assign(_charbuffer);
        fullpath.append("\\24dot");
        if(GetFileAttributesA(fullpath.c_str())==INVALID_FILE_ATTRIBUTES)
        {
           //can't not find the file ,file is not exist,create file by below code
            bool flag=CreateDirectoryA(fullpath.c_str(),NULL);
            cerr<<"The flag that file create returns is"<<flag<<endl;
        }
        else
        {   
                if(!(GetFileAttributesA(fullpath.c_str())&FILE_ATTRIBUTE_DIRECTORY)) //check the path's attribute,check whether there is
                {
                    //这里的检测依然采用寄存器形式的&操作 ,//判定为非文件夹类型，创建文件夹
                    bool flag=CreateDirectoryA(fullpath.c_str(),NULL);
                    cerr<<"The flag that file create returns is"<<flag<<endl;
                }
                else
                {
                cerr<<"Directory already exists";
                }
        }
        fullpath.append("\\24dotrecord.dat");
        return fullpath.c_str(); //make sure the fullpath is available
    }
    else
    {
        return nullptr;
        cerr<<"can not open directory APPDATA";
    }


}
