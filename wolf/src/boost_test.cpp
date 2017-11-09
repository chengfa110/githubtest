#include <boost/thread/thread.hpp> 
#include <iostream>
#include<time.h> 

//#include "stdafx.h"
using namespace std;

/*int hello(int n) 
{ 
        if(n == 0)
          return 0;
           else if(n == 1)
          return 1;
          else
          return(hello(n-2)+hello(n-1));
} 

void fibo()
{
    for(int n=0; n < 50 ;n++)
    {
        std::cout<<hello(n)<<std::endl;
    }

}

void write2()
{
int a[20];
    for(int i = 0;i < 20 ;i++)
    {
        a[i] = i;
        std::cout<<a[i]<<std::endl;
    }
}


  void write3()
	{
   int a[20];
    for(int i = 0;i < 20 ;i++)
    {
        a[i] = i*10;
        std::cout<<a[i]<<std::endl;
    }
}


int main(int argc, char* argv[]) 
{      clock_t start_time = clock();
        boost::thread thrd2(write2); 
        boost::thread thrd1(write2); 
        
        thrd1.join(); 
        thrd2.join(); 

        clock_t end_time = clock();
        std::cout<< "Running time is: "<<static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
          
        return 0; 
} */

//利用istream_iterator进行输入，利用ostream_iterator进行输出
//下面程序演示了使用istream_iterator从标准输入进行输入，以及使用ostream_iterator输出到标准输出

