CC:=clang
CXX:=clang++
CFLAGS+=-O0 -g
CXXFLAGS+=$(CFLAGS) -std=c++20

sepstack_transform: sepstack_transform.cpp sepstack_trampoline.S
	$(CXX) $(CXXFLAGS) sepstack_transform.cpp -c -o sepstack_transform.o
	$(CC) $(CFLAGS) sepstack_trampoline.S -c -o sepstack_trampoline.o
	$(CXX) sepstack_transform.o sepstack_trampoline.o -g -o $@
