.PHONY:all 
all: libmy.so libmy.a

# 动态库
libmy.so:mymath_d.o myprint_d.o
	gcc -shared -o libmy.so mymath_d.o myprint_d.o
mymath_d.o:mymath.c
	gcc -o $@ -fPIC -c $^
myprint_d.o:myprint.c
	gcc -o $@ -fPIC -c $^


# 静态库
libmy.a:mymath_s.o myprint_s.o
	ar -rc libmy.a mymath_s.o myprint_s.o
mymath_s.o:mymath.c
	gcc -o $@ -c $^
myprint_s.o:myprint.c
	gcc -o $@ -c $^

.PHONY:lib
lib:
	mkdir -p lib-static/lib
	mkdir -p lib-static/include
	cp *.a lib-static/lib
	cp *.h lib-static/include

	mkdir -p lib-dynamic/lib
	mkdir -p lib-dynamic/include
	cp *.so lib-dynamic/lib
	cp *.h lib-dynamic/include

.PHONY:clean
clean:
	rm -rf lib-static lib-dynamic *.a *.o
