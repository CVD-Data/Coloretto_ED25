all:
	g++ -std=c++17 main.cpp src/*.cpp -Iinclude -o coloretto -pthread
clean:
	del coloretto.exe
