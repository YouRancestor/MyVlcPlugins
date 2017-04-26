#include <iostream>
#include <vector>
#include <string>
using namespace std;

class k_v
{
    public:
        string key;
        string value;
    public:
        //eg :abc=123,start是a，val是1，end是3后面一个字符
        k_v(char* start,char *val,char *end):key(start,val-1),value(val,end)
    {
    }
};
//获取“abc=123”中最后一个字符3后面一个字符
char *get_end(char *p)
{
    while(*p != '\0' && *p != '&')
        p++;
    return p;
}
//获取“abc=123”中“=”后面一个字符“1”
char *get_value(char *p)
{
    while(*p != '\0' && *p != '=')
        p++;
    p++;
    return p;
}

extern "C"
void *InitKeyValueList(char *str)
{
    vector<k_v> *vec = new vector<k_v>;
    char *head=str;
    char *value;
    char *end;
    do
    {
        end = get_end(head);
        value = get_value(head);
        vec->push_back(k_v(head,value,end));
        head = end+1;
    }while (*end != '\0');

    return  vec;
}

extern "C"
const char* GetValue(void *p,char *name)
{
    vector<k_v> *vec =(vector<k_v> *) p;
    vector<k_v>::iterator it = vec->begin();
    string s_name(name);
    while(it!=vec->end())
    {
        if(it->key == s_name )
        {
            const char *c_tmp = (it->value).data();
            return c_tmp;
        }
        it++;
    }
    return NULL;

}

extern "C"
void DestoryKeyValueList(void *p)
{
    vector<k_v> *vec =(vector<k_v> *) p;
    delete vec;
}

//int main1()//示例
//{
//    char p[]="aaa=1111&bbb=222&cc=333";
//    void * head = Init_vec(p);
//    const char *value;
//    value = GetVa(head,(char*)"bbb");
//    printf("%s",value);
//    return 0;
//}



