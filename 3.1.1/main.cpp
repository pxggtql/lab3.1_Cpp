#include <string>
#include <math.h>
#include <iostream>
using namespace std;
#define CRC_CCITT 0x1021
uint8_t  senddata[6];
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
string bin2Str(uint16_t val2)
{
    string res="";
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
void str2Bin(string sendStr,int j)
{
    for(int i=0;i<j;i++) //string to bin
    {
        int length=sendStr.length();
        string subSend=sendStr.substr(0,8);
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
int main()
{
    cout<<"[Sending]......"<<endl<<"Input your message(0110XXXXXXXXXXXXXXXXXXXX110)"<<endl;
    string sendStr;
    cin>>sendStr;
    cout<<"Send string is "<<sendStr<<endl;
    string sendStr1="00000"+sendStr;
    str2Bin(sendStr1,4);
    unsigned int crcResult=crc16(senddata,4);
    string crcStr=bin2Str(crcResult);
    cout<<"CRC mode is CRC-CCITT poly is 0b10001000000100001"<<endl;
    cout<<"CRC Code is "<<crcStr<<endl;
    string receiveStr=sendStr+crcStr;
    cout<<"[Receiving]......"<<endl;
    cout<<"Receive message is "<<receiveStr<<endl;
    string receiveStr1="00000"+receiveStr;
    str2Bin(receiveStr1,6);
    crcResult=crc16(senddata,6);
    cout<<"CRC mode is CRC-CCITT poly is 0b10001000000100001"<<endl;
    crcStr=bin2Str(crcResult);
    cout<<"Received CRC Code is "<<crcStr<<endl;
    for(int p=0;p<16;p++) 
        if(crcStr[p]!='0')
            {cout<<"Receive string is invalid!"<<endl;
            return 0;
            }
    cout<<"Receive string is valid!"<<endl;
}