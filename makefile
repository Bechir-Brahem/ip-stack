src = $(wildcard src/*.cpp)
obj = $(patsubst src/%.cpp, obj/%.o, $(src))
headers = $(wildcard include/*.h)


tuntap: print $(obj)
	g++ $(obj) -o tuntap

print:
	$(info	source files: $(src))
	$(info	headers: $(headers))
	$(info	object files: $(obj))

 
obj/%.o: src/%.cpp $(headers)
	g++ -I include -c $< -o $@

clean:
	rm obj/* tuntap
