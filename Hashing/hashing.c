#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../massdalsketches/prng.h"
#include "hashing.h"
#include <math.h>
#include <time.h>
char * allocate_array(int size)
{
	return (char*)malloc(size);
}

char * add_arrays(char *a, char* b, int size_a, int size_b)
{
	char* res;
	if(size_b>size_a)
	{
		char * temp=b;	b=a; a=temp;
		int temp_size=size_a; size_a=size_b; size_b=temp_size;
	}
	
		int size_res=size_a+1;
		res=malloc(size_res);
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
	//if(i==oldlen) ++i;
	arr[i]='\0';
	return arr;
}
char * mul_arrays(char *a, char* b, int size_a, int size_b)
{
	char * res="";
	int i,pos;
	for(i=size_b-2, pos=0;i>=0;--i,pos++)
	{
		int carry=0;
		char * temp=malloc(size_a+1);
		int j;
		for(j=size_a-2;j>=0;--j)
		{
			int prod=(a[j]-'0')*(b[i]-'0')+carry;			
			temp[j+1]=(prod%10)+'0';
			carry=prod/10;
		}
		temp[0]=carry+'0';
		temp[size_a]='\0';
		temp=add_zeroes_to_the_right(temp,pos);
		int res_len=(strlen(res)==0)?0:(strlen(res)+1);
		char* res2=add_arrays(res,temp,res_len,strlen(temp)+1);
		if(strcmp(res,"")!=0) free(res);
		res=res2;		
		free(temp);
	}
	return res;
}

//returns a%b where a is a string and b is of type long long.
long long mod_arrays(char *a, long long b, int size_a)
{
	long long result=0;
	int i;
	for(i=0; i<size_a-1;++i)
	{
		result=(result*10 + (a[i]-'0'))%b;
	}
	return result;
}

long long mod_arrays2(char *a, long long b)

{
	int size_a=strlen(a)+1;
	long long result=0;
	int i;
	for(i=0; i<size_a-1;++i)
	{
		result=(result*10 + (a[i]-'0'))%b;
	}
	return result;
}


//returns the hashvalue for num computed as ((a*num)+b)%modval
long long hashval(char *num, char *a,char *b, long long modval)
{
	char * temp=mul_arrays(num,a,strlen(num)+1,strlen(a)+1);
	char* temp2=add_arrays(temp,b,strlen(temp)+1,strlen(b)+1);
	free(temp);
//
	long long res=mod_arrays(temp2,modval,strlen(temp2)+1);
	free(temp2);
	return res;
}

char * get_string(long long val)
{
	int num_digits;
	if(val==0) num_digits=1;
	else num_digits=log10l(val)+1;
	char * res=malloc(num_digits*sizeof(char)+1);
	res[num_digits--]='\0';
	if(val==0) res[0]='0';
	while(val>0)
	{
		res[num_digits--]=(val%10)+'0';
		val=val/10;
	}
	return res;
}
void deallocate_array(char * a)
{
	free(a);
}

//Call allocate_arrays before calling this function.
void generate_rand_using_prng(char* a,prng_type * prng)
{
	int numele=num_of_elements/4;
	int j=3;
	int i;
	for(i=0;i<numele;++i)
	{
		long res=prng_int(prng)%9999;
		int count;
		for(count=0;count<=3;++count)
		{
			a[j-count]=res%10+'0';
			res=res/10;
		}
		j+=4;
	}
	a[num_of_elements-1]='\0';
}

void zipf(double alpha,unsigned int m)
{
	prng_type* prng=prng_Init(time(NULL),3);
	int n=logl(m)/log(alpha);
	char *map[n];
	long double zeta=1.0;
	int i=2,j=0,temp;
	for(;i<=n;++i)
		zeta+=1./pow(i,alpha);
	int freq[n],op[m];
	double mbyzeta=m/zeta;
	int randindex;
	for(i=1;i<=n;++i)
	{
		map[i-1]=allocate_array(sizeofdata);
		generate_rand_using_prng(map[i-1],prng);	
		freq[i-1]=mbyzeta/pow(i,alpha);	
		for(temp=0;temp<freq[i-1];++temp)
			op[j++]=i;
	}
	for(;j<m;)
	{	
		op[j++]=i;
		++i;
	}
	prng_Destroy(prng);
	for(i=0;i<m;++i)
	{
		temp=op[i];
		randindex=rand()%m;
		op[i]=op[randindex];
		op[randindex]=temp;
	}
	char* val=allocate_array(sizeofdata);
	for(i=0;i<m;++i)
	{
		if(op[i]<=n)
		printf("%s\n",map[op[i]-1]);
		else
		{
			generate_rand_using_prng(val,prng);
			printf("%s\n",val);
		}
	}
	free(val);
}

void generate_data(unsigned int N)
 {
	prng_type* prng=prng_Init(time(NULL),3);
 	srand((unsigned int)time(NULL));

    double no = 1.0;
    double i;
    

    for(i=0;i<=N-1;i++)    	
    {
    	char* a =(char*)malloc(sizeofdata);
    	double x  = ((double)rand()/(double)(RAND_MAX)) * no;
		int u=sizeofdata-2;	  
		a[u+1]='\0';  	
		if(x<=0.02)
    	{
    		int j;	
    		for (j=0;j<u;j++)
    			a[j] = 0 + '0';
        	a[j] = '0';
        	printf("%s\n" ,a);
        	free(a);
    		//a[0]+=1;
    	}
    	else if(x<=0.04)
    	{
    		int j;	
    		for (j=0;j<u;j++)
    			a[j] = 0 + '0';
        	a[j] = '1';
        	printf("%s\n" ,a);
        	free(a);
    		//a[1]+=1;
    	}
    	else if(x<=0.06)
    	{
    		int j;	
    		for (j=0;j<u;j++)
    			a[j] = 0 + '0';
        	a[j] = '2';
        	printf("%s\n" ,a);
        	free(a);	
    		//a[2]+=1;
    	}
    	else if(x<=0.08)
    	{
    		int j;	
    		for (j=0;j<u;j++)
    			a[j] = 0 + '0';
        	a[j] = '3';
        	printf("%s\n" ,a);
        	free(a);
    		//a[3]+=1;
    	}
    	else if(x<=0.10)
    	{
    		int j;	
    		for (j=0;j<u;j++)
    			a[j] = 0 + '0';
        	a[j] = '4';
        	printf("%s\n" ,a);
        	free(a);        	
    		//a[4]+=1;
    	}
    	else
    	{
    		//char* a =(char*)malloc(sizeofdata);
    		generate_rand_using_prng(a,prng);
			printf("%s\n",a);
    		free(a);
    	}
    }
	prng_Destroy(prng);
 }
