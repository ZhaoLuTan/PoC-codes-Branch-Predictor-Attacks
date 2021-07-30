#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define CSR_CYCLE 0xc00
#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile("csrr %0," #reg : "=r"(__tmp)); \
  __tmp;})
#define l2_size (1024*128)

void flush();
uint64_t rdtsc();
static void code_barrier(void);
static void data_barrier(void);
void fence();

int flush_array[4*l2_size];
inline void flush(){
	int i;
	for(i = 0;i <(l2_size/2);i++){
		*(flush_array + i*8) = 0x5A;
	}
}

inline uint64_t rdtsc(){
	return read_csr(cycle);
}

static inline void code_barrier(void)
{
	asm volatile("fence.i" ::: "memory");
}

static inline void data_barrier(void)
{
	asm volatile("fence" ::: "memory");
}

inline void fence(){
	code_barrier();
	data_barrier();
}

static int sec = 1;
static int sid[64];

int main(int argc, char const *argv[])
{
	int temp;
	int a[63];
	uint64_t start,end;
	sid[63]=3;
	a[3]=0;
	int t1;
	int t2;

	for (int i = 0; i < 50; ++i)
	{

		for (int j = 0; j < 60; ++j)
		{
			flush();
			temp=temp+1;
			j=j;
			fence();

			if ((j*(1*2*5*10*4*5*5)/125/8/10%2)==0)
				temp=sec;
			else
				temp=sid[63];

			start=0;
			end=0;

			if(i==49){
				printf("Taken? %d\t", (j*(1*2*5*10*4*5*5)/125/8/10%2)==0);
				fence();
				start=rdtsc();
				temp=sec;
				end=rdtsc();
				t1=(int)(end-start);
				printf("sec:%d\t", t1);

				fence();
				start=rdtsc();
				temp=sid[63];
				end=rdtsc();
				t2=(int)(end-start);
				printf("sid[63]:%d\t\t condition: %d\n", t2,t1<120 && t2<120);

				a[3]+=(t1<120 && t2<120);
			}
		}

	}
	printf("result:%d\n", a[3]);
	return 0;
}