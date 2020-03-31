#include <string>
#include <string.h>
#include <math.h>
#include <iostream>
using namespace std;
string BitFlagString="01111110";
string ByteFlagString="~";
string zeroBitPadding(string sendStr)
{

    int count=0;
    for(int i=0;i<sendStr.length();i++)
    {
        string padding="0";
        if(sendStr[i]=='1')
        {
            count++;
            if(count==5)
            {
                sendStr.insert(i+1,padding);
            }
        }
        else if(sendStr[i]=='0')
        {
            count=0;
        }
    }
    return BitFlagString+sendStr+BitFlagString;
}
string zeroBitUnpadding(string recStr)
{
    int count=0;
    int len=recStr.length();
    recStr=recStr.substr(8,len-16);
    cout<<"add "<<recStr<<endl;
    for(int i=0;i<recStr.length();i++)
    {
        if(recStr[i]=='1')
        {
            count++;
            if(count==5&&i!=recStr.length()-1)
            {
                if(recStr[i+1]=='0')
                {recStr.erase(i+1,1);
                   count=0;}
            }
        }
        else if(recStr[i]=='0')
        {
            count=0;
        }
    }

    return recStr;
}
string bytePadding(string sendStr)
{

    for(int i=0;i<sendStr.length();i++)
    {
        string padding="";
        switch (sendStr[i])
        {
        case 0x7E:
            {sendStr.erase(i,1);
            padding="";
            padding+=(char)0x7D;
            padding+=(char)0x5E;
            sendStr.insert(i,padding);
            break;}
        case 0x7D:
            {sendStr.erase(i,1);
            padding="";
            padding+=(char)0x7D;
            padding+=(char)0x5D;
            sendStr.insert(i,padding);
            break;}
        case 0x00 ... 0x20:
            {padding="";
            padding+=(char)0x7D;
            padding+=(char)((char)sendStr[i]+0x20);
            sendStr.erase(i,1);
            sendStr.insert(i,padding);
            break;  }  
        default:
            break;
        }
    }
    return ByteFlagString+sendStr+ByteFlagString;
}
string byteUnpadding(string recStr)
{
    recStr=recStr.substr(1,recStr.length()-2);
    for(int i=0;i<recStr.length();i++)
    {
        string insert="";
        if(recStr[i]==0x7D&&i!=recStr.length()-1)
        {
            switch (recStr[i+1])
            {
            case 0x5E:
                {recStr.erase(i,2);
                insert="";
                insert+=(char)0x7E;
                recStr.insert(i,insert);
                break;}
            case 0x5D:
                {recStr.erase(i,2);
                insert="";
                insert+=(char)0x7D;
                recStr.insert(i,insert);
                break;}
            case 0x20 ... 0x40:
                {
                insert="";
                insert+=(char)(recStr[i+1]-0x20);
                recStr.erase(i,2);
                recStr.insert(i,insert);
                break;}
            default:
                break;
            }
        }
    }
    return recStr;
}
void printhex(string str)
{
    int len=str.length();
    char result[100];
    strcpy(result,str.c_str());
    for(int i=0;i<len;i++)
        printf("%x",result[i]);
    printf("\n");
}
int main()
{
    cout<<"[Zero Bit Padding]"<<endl;
    string zeroBitString="0110011111101111111111101111110110";
    cout<<"Send String is "<<zeroBitString<<endl;
    string zeroBitStringPadding=zeroBitPadding(zeroBitString);
    cout<<"Send String in fact is "<<zeroBitStringPadding<<endl;
    string zeroBitReceiveString=zeroBitStringPadding;
    cout<<"receive String is "<<zeroBitReceiveString<<endl;
    string zeroBitReceiveStringUnpadding=zeroBitUnpadding(zeroBitReceiveString);
    cout<<"After decode is "<<zeroBitReceiveStringUnpadding<<endl;
    cout<<"[Byte Padding]"<<endl;
    string byteString = "347D7E807E40AA7D";
    cout<<"Send String is ";
    printhex(byteString);
    string byteStringPadding=bytePadding(byteString);
    cout<<"Send String in fact is ";
    printhex(byteStringPadding);
    string byteReceiveString=byteStringPadding;
    cout<<"receive String is ";
    printhex(byteReceiveString);
    string byteReceiveStringUnpadding=byteUnpadding(byteReceiveString);
    cout<<"After decode is ";
    printhex(byteReceiveStringUnpadding);


}