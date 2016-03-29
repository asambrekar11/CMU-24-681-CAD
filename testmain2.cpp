#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "hashtable.h"


template <>
unsigned long long int HashTable<std::string,std::string>::HashCode(const std::string &key) const
{
	unsigned long long sum=0;
	for(auto c : key)
	{
		sum+=c;
	}
	return sum;
}

int main(void)
{
	HashTable <std::string,std::string> set;

	set.Update("Phantom","F-4");
	set.Update("Tiger","F-5");
	set.Update("Crusader","F-8");
	set.Update("Tomcat","F-14");
	set.Update("Eagle","F-15");
	set.Update("Falcon","F-16");
	set.Update("Hornet","F-18");
	set.Update("Raptor","F-22");


	for(auto hd=set.First(); set.IsNotNull(hd); hd=set.Next(hd))
	{
		auto valuePtr=set[hd];
		auto key=set.GetKey(hd);
		printf("Key=%s  Value=%s\n",key.c_str(),valuePtr->c_str());
	}


	for(;;)
	{
		printf("Enter String>");

		char str[256];
		fgets(str,255,stdin);
		for(int i=0; 0!=str[i]; ++i)
		{
			if(' '>str[i])
			{
				str[i]=0;
				break;
			}
		}

		std::string tst=str;
		auto valuePtr=set[tst];
		if(nullptr!=valuePtr)
		{
			printf("Value=%s\n",valuePtr->c_str());
		}
		else
		{
			printf("Not included.\n");
		}
	}

	return 0;
}

