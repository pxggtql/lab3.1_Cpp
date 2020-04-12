#include <string>
#include <string.h>
#include <math.h>
#include <iostream>
using namespace std;
string BitFlagString="01111110";
string ByteFlagString="7E";
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

    for(int i=0;i<sendStr.length();i+=2)//每次读取2个字符，即一个字节 
    {
        string padding="";
        switch (sendStr[i])
        {
        case '7':
        	if(sendStr[i+1]=='E')//7E
            {sendStr.erase(i,2);
            padding="7D5E";
            sendStr.insert(i,padding);
			i=i+2; 
			}else if(sendStr[i+1]=='D'){//7D
			sendStr.erase(i,2);
			padding="7D5D";
			sendStr.insert(i,padding);
			i=i+2; 	
			}
            break;
        case '0'://0x00-0x09
            {padding="7D2";            
            padding+=sendStr[i+1];
            sendStr.erase(i,2);
            sendStr.insert(i,padding);
            i=i+2;
            break;  }  
        case '1'://0x10-0x19
            {padding="7D3";            
            padding+=sendStr[i+1];
            sendStr.erase(i,2);
            sendStr.insert(i,padding);
            i=i+2;
            break;  }  
        default:
            break;
        }
    }
    return ByteFlagString+sendStr+ByteFlagString;
}
string byteUnpadding(string recStr)
{
    recStr=recStr.substr(2,recStr.length()-4);
    for(int i=0;i<recStr.length();i+=2)
    {
        string insert="";
        if(recStr[i]=='7'&&recStr[i+1]=='D')
        {
            switch (recStr[i+2])
            {
            case '5':
                if(recStr[i+3]=='E')//7D5E->7E
				{recStr.erase(i,4);
                insert="7E";
                recStr.insert(i,insert);
                }else if(recStr[i+3]=='D'){//7D5D->7D
                recStr.erase(i,4);
                insert="7D";
                recStr.insert(i,insert);
				}				
				break;
            case '2':   //7D20~7D29 ->00~09 	
                {
                insert="0";
                insert+=recStr[i+3];
                recStr.erase(i,4);
                recStr.insert(i,insert);
                break;}
            case '3':	//7D30~7D39 ->10~19 
                {
                insert="1";
                insert+=recStr[i+3];
                recStr.erase(i,4);
                recStr.insert(i,insert);
                break;}
            default:
                break;
            }
        }
    }
    return recStr;
}

int main()
{
    cout<<"[Zero Bit Padding]"<<endl;
    string zeroBitString="01101111101111111111101111110110";
    cout<<"Send String is "<<zeroBitString<<endl;
    string zeroBitStringPadding=zeroBitPadding(zeroBitString);
    cout<<"Send String in fact is "<<zeroBitStringPadding<<endl;
    string zeroBitReceiveString=zeroBitStringPadding;
    cout<<"receive String is "<<zeroBitReceiveString<<endl;
    string zeroBitReceiveStringUnpadding=zeroBitUnpadding(zeroBitReceiveString);
    cout<<"After decode is "<<zeroBitReceiveStringUnpadding<<endl;
    cout<<"[Byte Padding]"<<endl;
    string byteString = "347D7E807E40AA7D";
    cout<<"Send String is "<<byteString<<endl;
    string byteStringPadding=bytePadding(byteString);
    cout<<"Send String in fact is "<<byteStringPadding<<endl;
    string byteReceiveString=byteStringPadding;
    cout<<"receive String is "<<byteReceiveString<<endl;
    string byteReceiveStringUnpadding=byteUnpadding(byteReceiveString);
    cout<<"After decode is "<<byteReceiveStringUnpadding<<endl;
}
