all: *.cpp
	$(CXX) *.cpp -std=c++17 -shared -fPIC -ldl -g -O3 -Werror -Wall -Wextra -pthread -pedantic -o sanitizer.so

clean:
	rm -f sanitizer.so

