CXXFLAGS=-O2 -std=c++11

.PHONY: all

all: gmonitor

gmonitor: console_writer.o main.o gpu.o manager.o stats_reader.o
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++ console_writer.o main.o gpu.o manager.o stats_reader.o -o gmonitor
	@echo 'Finished building target: $@'
	@echo ' '

console_writer.o: console_writer.cpp console_writer.h

main.o: main.cpp constants.h utils.h

gpu.o: gpu.cpp gpu.h

manager.o: manager.cpp manager.h

stats_reader.o: stats_reader.cpp stats_reader.h

.PHONY: clean
clean:
	rm -f *.o gmonitor
