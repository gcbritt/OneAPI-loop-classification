#include "CL/sycl.hpp"
#include "CL/sycl/intel/fpga_extensions.hpp"
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define LEN 32000
#define LEN2 256
#define TYPE float
#define ALIGNMENT 16
#define ntimes 200000
#define digits 6

constexpr auto dp_r = cl::sycl::access::mode::read;
constexpr auto dp_rw = cl::sycl::access::mode::read_write;
constexpr auto dp_w = cl::sycl::access::mode::write;

using namespace cl::sycl;


TYPE array[LEN2*LEN2] __attribute__((aligned(ALIGNMENT)));

TYPE x[LEN] __attribute__((aligned(ALIGNMENT)));
TYPE temp;
int temp_int;

struct GlobalData 
{
  __attribute__((aligned(ALIGNMENT))) TYPE a[LEN];
  int pad1[3];
  __attribute__((aligned(ALIGNMENT))) TYPE b[LEN];
  int pad2[5];
  __attribute__((aligned(ALIGNMENT))) TYPE c[LEN];
  int pad3[7];
  __attribute__((aligned(ALIGNMENT))) TYPE d[LEN];
  int pad4[11];
  __attribute__((aligned(ALIGNMENT))) TYPE e[LEN];

  int pad5[13];
  __attribute__((aligned(ALIGNMENT))) TYPE aa[LEN2][LEN2];
  int pad6[17];
  __attribute__((aligned(ALIGNMENT))) TYPE bb[LEN2][LEN2];
  int pad7[19];
  __attribute__((aligned(ALIGNMENT))) TYPE cc[LEN2][LEN2];
  int pad8[23];
  __attribute__((aligned(ALIGNMENT))) TYPE tt[LEN2][LEN2];
} global_data;


__attribute__((aligned(ALIGNMENT))) TYPE * const a = global_data.a;
__attribute__((aligned(ALIGNMENT))) TYPE * const b = global_data.b;
__attribute__((aligned(ALIGNMENT))) TYPE * const c = global_data.c;
__attribute__((aligned(ALIGNMENT))) TYPE * const d = global_data.d;
__attribute__((aligned(ALIGNMENT))) TYPE * const e = global_data.e;
__attribute__((aligned(ALIGNMENT))) TYPE (* const aa)[LEN2] = global_data.aa;
__attribute__((aligned(ALIGNMENT))) TYPE (* const bb)[LEN2] = global_data.bb;
__attribute__((aligned(ALIGNMENT))) TYPE (* const cc)[LEN2] = global_data.cc;
__attribute__((aligned(ALIGNMENT))) TYPE (* const tt)[LEN2] = global_data.tt;

int dummy(TYPE a[LEN], TYPE b[LEN], TYPE c[LEN], TYPE d[LEN], TYPE e[LEN], TYPE aa[LEN2][LEN2], TYPE bb[LEN2][LEN2], TYPE cc[LEN2][LEN2], TYPE s)
{
	// --  called in each loop to make all computations appear required
	return 0;
}

// initialize 1-d arrays
int set1d(TYPE arr[LEN], TYPE value, int stride)
{
	if (stride == -1) 
	{
		for (int i = 0; i < LEN; i++) 
			arr[i] = 1. / (TYPE) (i+1);
	} 
	else if (stride == -2) 
	{
		for (int i = 0; i < LEN; i++) 
			arr[i] = 1. / (TYPE) ((i+1) * (i+1));
	}
	else 
	{
		for (int i = 0; i < LEN; i += stride) 
			arr[i] = value;
	}
	return 0;
}

// initialize 2-d arrays
int set2d(TYPE arr[LEN2][LEN2], TYPE value, int stride)
{
	if (stride == -1) 
	{
		for (int i = 0; i < LEN2; i++) 
		{
			for (int j = 0; j < LEN2; j++) 
			{
				arr[i][j] = 1. / (TYPE) (i+1);
			}
		}
	} 
	else if (stride == -2) 
	{
		for (int i = 0; i < LEN2; i++) 
			for (int j = 0; j < LEN2; j++) 
				arr[i][j] = 1. / (TYPE) ((i+1) * (i+1));
	}
	else 
	{
		for (int i = 0; i < LEN2; i++) 
			for (int j = 0; j < LEN2; j += stride) 
				arr[i][j] = value;
	}
	return 0;
}

// initialize arrays for different types of tests
int init(const char* name)
{
	TYPE any=0.;
	TYPE zero=0.;
	TYPE half=.5;
	TYPE one=1.;
	TYPE two=2.;
	TYPE small = .000001;
	int unit =1;
	int frac=-1;
	int frac2=-2;

	// intra-dimension dependent and diagonal dependent
	if (!strcmp(name, "s119 ") || !strcmp(name, "s1119 ")) 
	{
		set2d(aa, one,unit);
		set2d(bb, any,frac2);
	}
	// conditional dependent
	else if(!strcmp(name, "s124 ")) 
	{
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
		set1d(d, any,frac);
		set1d(e, any,frac);
	}
	// anti dependent
	else if(!strcmp(name, "s211 ")) 
	{
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	}
	// half-parallelism half-dependent
	else if(!strcmp(name, "s221 ")) 
	{
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
		set1d(d, any,frac);
	}
	// otherwise
	else{}

	return 0;
}

// compute and print checksum
void check(int name)
{
	TYPE suma = 0;
	TYPE sumb = 0;
	TYPE sumc = 0;
	TYPE sumd = 0;
	TYPE sume = 0;

	for (int i = 0; i < LEN; i++)
	{
		suma += a[i];
		sumb += b[i];
		sumc += c[i];
		sumd += d[i];
		sume += e[i];
	}

	TYPE sumaa = 0;
	TYPE sumbb = 0;
	TYPE sumcc = 0;

	for (int i = 0; i < LEN2; i++)
		for (int j = 0; j < LEN2; j++)
		{
			sumaa += aa[i][j];
			sumbb += bb[i][j];
			sumcc += cc[i][j];

		}

	TYPE sumarray = 0;

	for (int i = 0; i < LEN2*LEN2; i++)
	{
		sumarray += array[i];
	}

	if (name == 1) printf("%.*g \n",digits,suma);
	if (name == 2) printf("%.*g \n",digits,sumb);
	if (name == 3) printf("%.*g \n",digits,sumc);
	if (name == 4) printf("%.*g \n",digits,sumd);
	if (name == 5) printf("%.*g \n",digits,sume);
	if (name == 11) printf("%.*g \n",digits,sumaa);
	if (name == 22) printf("%.*g \n",digits,sumbb);
	if (name == 33) printf("%.*g \n",digits,sumcc);
	if (name == 0) printf("%.*g \n",digits,sumarray);
	if (name == 12) printf("%.*g \n",digits,suma+sumb);
	if (name == 25) printf("%.*g \n",digits,sumb+sume);
	if (name == 13) printf("%.*g \n",digits,suma+sumc);
	if (name == 123) printf("%.*g \n",digits,suma+sumb+sumc);
	if (name == 1122) printf("%.*g \n",digits,sumaa+sumbb);
	if (name == 112233) printf("%.*g \n",digits,sumaa+sumbb+sumcc);
	if (name == 111) printf("%.*g \n",digits,sumaa+suma);
	if (name == -1) printf("%.*g \n",digits,temp);
	if (name == -12) printf("%.*g \n",digits,temp+sumb);

}
