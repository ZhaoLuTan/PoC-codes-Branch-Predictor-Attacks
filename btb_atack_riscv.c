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
static int (*p)();
void fence();

int flush_array[4*l2_size];
static int sec[1024*64*20];
static int sid[1024*64*20];

inline void flush(){
	int i;
	for(i=1024*64;i<1024*64*20;i+=1024*64){	// flush from L2
		sec[i]=0;	 // flush p
		sid[i]=0;
		sec[i+50]=0; // flush sec[50]
		sid[i+50]=0; // flush sid[50]
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

int func1(){
	sec[50]=sec[50]+1;
	return sec[50];
}

int func2(){
	sid[50]=sid[50]+1;
	return sid[50];
}

int main(int argc, char const *argv[])
{
	int temp;
	int a[64];
	uint64_t start,end;

	for (int i = 0; i < 50; ++i)
	{
		start=0;
		end=0;

		if (i<45)
			p=&func1;
		else
			p=&func2;

		flush(); // flush p from cache
		fence();
		p();	 // p is not ready, BTB predict to execute. In 45th iteration, mis-predict to execute func1.

		temp=0;
		fence();
		start=rdtsc();
		temp=(int)p;
		end=rdtsc();
		printf("p:%d\t", (int)(end-start));

		fence();
		start=rdtsc();
		temp=sec[50];
		end=rdtsc();
		printf("sec[50]:%d\t", (int)(end-start));

		fence();
		start=rdtsc();
		temp=sid[50];
		end=rdtsc();
		printf("sid[50]:%d\n", (int)(end-start));

	}
	return 0;
}
