#.PHONY:ls
ls:libcopyls.a
	gcc -g -o ls main.c -lcopyls -L.
libcopyls.a:copy_ls.o
	ar rcs libcopyls.a copy_ls.o
copy_ls.o:copy_ls.c copy_ls.h
	gcc -g -c -Wall copy_ls.c
.PHONY:clean
clean:
	rm *.o ls

