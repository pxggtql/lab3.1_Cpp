#include "util.h"
#include <math.h>
#define CRC_CCITT 0x1021
unsigned int crc16(unsigned char* ptr, int len)
{    unsigned int crc = 0;
    while(len-- != 0)
    {
        for(unsigned char i = 0x80; i != 0; i /= 2)
        {
            crc *= 2;
            if((crc&0x10000) !=0) //上一位CRC乘 2后，若首位是1，则除以 0x11021
                crc ^= 0x11021;
            if((*ptr&i) != 0)    //如果本位是1，那么CRC = 上一位的CRC + 本位/CRC_CCITT
                crc ^= CRC_CCITT;
        }
        ptr++;
    }
    return crc;
}
std::string bin2Str(uint16_t val2)
{
    std::string res="";
    unsigned char *p = (unsigned char*)&val2 + 3; //从低位到高位,低端字节计算机
    for(int k = 2; k <= 3; k++)
    {
        int val2 = *(p-k);
        for (int i = 7; i >= 0; i--)
        {
            if(val2 & (1 << i))
            res+='1';
            else
            res+='0';
        }
    }
    return res;
}
void str2Bin(std::string sendStr,char * senddata,int j)
{
    for(int i=0;i<j;i++) //string to bin
    {
        int length=sendStr.length();
        std::string subSend=sendStr.substr(0,8);
        //cout<<subSend<<endl;
        senddata[i]=0;
        for(int j=0;j<8;j++)
        {
            if(subSend[j]=='1')
               { senddata[i]+=(uint8_t )pow(2,7-j);
               }
        }
        sendStr=sendStr.substr(8);
    }
}