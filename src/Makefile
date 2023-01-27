CFLAGS = -std=c++17 -Wall -Wextra -Werror 
CFLAGS1 = -std=c++17
GFLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -L. -lcheck -lgcov -lm

all: clean build run

run:
	cd build; open MLP.app

build: clean
	mkdir build
	cd build/; qmake ../MLP.pro
	make -C build/
	cd build/; rm -rf *.o *.cpp *.h Makefile
	# mkdir $(HOME)/weights
	cp -rf model/weights/ $(HOME)/weights

install: build
	mkdir $(HOME)/Desktop/perceptron/
	cp -rf build/perceptron.app $(HOME)/Desktop/perceptron/


uninstall:
	rm -rf $(HOME)/Desktop/perceptron/
	rm -rf $(HOME)/weights

dvi: 
	open -a "Google Chrome" description.html

dist: clean
	mkdir Dist
	tar --totals --create --verbose --file Dist/perceptron.tar *

gcov_report: 
	g++ --coverage model/*.cc  test.cc -o test -lstdc++ -lgtest -lgtest_main
	./test
	lcov -t "test" -o test.info -c -d . --no-external 
	genhtml -o report test.info
	open ./report/index.html
	rm -rf model/*.weights

tests: clean
	g++ model/*.cc  test.cc -o test -lstdc++ -lgtest -lgtest_main
	./test
	rm -rf model/*.weights

check:
	cppcheck --language=c++ --suppress=missingIncludeSystem model/*.cc controller/*.cc model/*.h controller/*.h view/*.h view/*.cpp

clang:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i model/*.cc controller/*.cc model/*.h controller/*.h
	clang-format -n model/*.cc controller/*.cc model/*.h controller/*.h
	rm -rf .clang-format

clean:
	rm -rf *.o *.a *.gcda *.gcno perceptron test perceptron.tar gcov_report build/ report/ test.info Dist/ *.out *.cfg
