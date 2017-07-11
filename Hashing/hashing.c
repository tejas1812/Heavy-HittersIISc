#include <stdlib.h>
#include <string.h>
#include "../massdalsketches/prng.h"
#include "hashing.h"

char * allocate_array(int size)
{
	return (char*)malloc(size);
}

char * add_arrays(char *a, char* b, int size_a, int size_b)
{
	if(size_b>size_a)
		return add_arrays(b,a,size_b,size_a);
	int size_res=size_a+1;
	char *res=malloc(size_res);
	int i=size_a-2;
	int diff=size_a-size_b;
	int carry=0;
	for(;i>=diff;--i)
	{
		int sum=(a[i]-'0')+(b[i-diff]-'0')+carry;
		res[i+1]=(sum%10)+'0';
		carry=sum/10;
	}
	for(;i>=0;--i)
	{
		int sum=(a[i]-'0')+carry;
		res[i+1]=(sum%10)+'0';
		carry=sum/10;
	}
	res[i+1]=carry+'0';
	res[size_res-1]='\0';
	return res;
}
char * add_zeroes_to_the_right(char *arr,int j)
{
	int oldlen=strlen(arr);
	arr=realloc(arr,strlen(arr)+1+j);
	int i=oldlen;
	for(;i<oldlen+j;++i)
		arr[i]='0';
	arr[i]='\0';
	return arr;
}
char * mul_arrays(char *a, char* b, int size_a, int size_b)
{
	char * res="";
	for(int i=size_b-2, pos=0;i>=0;--i,pos++)
	{
		int carry=0;
		char * temp=malloc(size_a+1);
		for(int j=size_a-2;j>=0;--j)
		{
			int prod=(a[j]-'0')*(b[i]-'0')+carry;
			temp[j+1]=(prod%10)+'0';
			carry=prod/10;
		}
		temp[0]=carry+'0';
		temp[size_a]='\0';
		temp=add_zeroes_to_the_right(temp,pos);
		int res_len=(strlen(res)==0)?0:(strlen(res)+1);
		res=add_arrays(res,temp,res_len,strlen(temp)+1);
		free(temp);
	}
	return res;
}
long long mod_arrays(char *a, long long b, int size_a)
{
	long long result=0;
	for(int i=0; i<size_a-1;++i)
	{
		result=(result*10 + (a[i]-'0'))%b;
	}
	return result;
}

long long hashval(char *num, char *a,char *b, long long modval)
{
	char * temp=mul_arrays(num,a,strlen(num)+1,strlen(a)+1);
	temp=add_arrays(temp,b,strlen(temp)+1,strlen(b)+1);
	long long res=mod_arrays(temp,modval,strlen(temp)+1);
	free(temp);
	return res;
}
void deallocate_array(char * a)
{
	free(a);
}

void generate_rand_using_prng(char* a,prng_type * prng)
{
	int numele=num_of_elements/4;
	int j=3;
	for(int i=0;i<numele;++i)
	{
		long res=prng_int(prng)%9999;
		for(int count=0;count<=3;++count)
		{
			a[j-count]=res%10+'0';
			res=res/10;
		}
		j+=4;
	}
	a[num_of_elements-1]='\0';
}
