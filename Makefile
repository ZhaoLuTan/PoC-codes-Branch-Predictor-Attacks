clean:
	rm *.out *_stat

btb_x86:
	gcc -std=gnu99  -O0 btb_mt_x86.c -lpthread -o btb.out
	taskset -c 0 ./btb.out | grep -v "^counter"> btb_stat

pht_x86:
	gcc -std=gnu99  -O0 pht_mt_x86.c -lpthread -o pht.out
	taskset -c 0 ./pht.out | grep -v "^counter"

count_btb_x86:
	rm -f btb_stat
	for i in {1..2}; \
	do \
		taskset -c 0 ./btb.out >> btb_stat; \
	done

count_pht_x86:
	rm -f pht_stat
	for i in {1..2}; \
	do \
		taskset -c 0 ./pht.out >> pht_stat; \
	done