CFLAGS = -I include -Wno-format -Wall -Wextra -std=c++14 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Winline -fdiagnostics-parseable-fixits -fdiagnostics-generate-patch -fdiagnostics-show-template-tree -fsanitize=undefined -fno-sanitize-recover -Og -ggdb3


src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp, obj/%.o, $(src))
headers = $(wildcard include/*.h)


tuntap: print $(obj)
	g++ $(CFLAGS)  -fsanitize=address $(obj) -o tuntap

print:
	$(info	source files: $(src))
	$(info	headers: $(headers))
	$(info	object files: $(obj))

 
obj/%.o: src/%.cpp $(headers) | obj
	g++ $(CFLAGS) -c $< -o $@

obj:
	mkdir -p $@

clean:
	rm obj/* tuntap
all: tuntap
