#include <string>  
#include <fstream>  
#include <sstream>  
#include <iostream>
using namespace std;
int main()
{std::ifstream t("mkdir");  
std::string str((std::istreambuf_iterator<char>(t)),  
                 std::istreambuf_iterator<char>()); 
cout<<str.length();
}
