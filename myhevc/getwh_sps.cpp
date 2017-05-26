extern "C"{
#include"getwh_sps.h"
}
#include"SPSParser.h"
#include<stdlib.h>
#include<stdio.h>
void getWH_SPS(const char* filename, int* width, int *height)
{
    SPSParser sps;
    FILE *pf = NULL;
    char buffer[100];
    int readlen;
    int w,h;
    pf = fopen(filename,"rb");
    if(pf==NULL){
        return;
    }
    while((readlen=fread(buffer,1,sizeof(buffer),pf))>0)
    {
        if(!(sps.parseSPS(buffer,readlen,true)))
        {
            sps.getSize(w,h);
            *width = w;
            *height = h;
            goto end;
        }
    }

end:
    fclose(pf);
    return ;
}
