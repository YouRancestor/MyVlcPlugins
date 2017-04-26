#ifndef Key_Value_List
#define Key_Value_List
#ifdef __cplusplus

extern "C" {

#endif

    /* -------------------------------------------*/
    /**
     * @brief  InitKeyValueList 初始化一个key,value队列，
     *
     * @param str 要初始化的key value字符串，格式为key=value&key=value
     *
     * @returns   返回一个KeyValueList句柄
     */
    /* -------------------------------------------*/
    void *InitKeyValueList(char *str);


    /* -------------------------------------------*/
    /**
     * @brief  GetValue 查找字符串的value
     *
     * @param p KeyValueList句柄指针
     * @param name 要查找的key
     *
     * @returns   返回找到的value指针，没有对应key返回0
     */
    /* -------------------------------------------*/
    const char* GetValue(void *p,char *name);


    /* -------------------------------------------*/
    /**
     * @brief  DestoryKeyValueList 销毁一个KeyValueList句柄
     *
     * @param p KeyValueList句柄指针
     */
    /* -------------------------------------------*/
    void DestoryKeyValueList(void *p);
#ifdef __cplusplus

}
#endif //cplusplus
#endif
