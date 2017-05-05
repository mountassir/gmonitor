gmonitor: console_writer.o main.o gpu.o manager.o stats_reader.o constants.h utils.h
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++ console_writer.o main.o gpu.o manager.o stats_reader.o -o gmonitor
	@echo 'Finished building target: $@'
	@echo ' '
	
console_writer: console_writer.cpp console_writer.h
	g++ -c console_writer.cpp

main: main.cpp
	g++ -c main.cpp

gpu: gpu.cpp gpu.h
	g++ -c gpu.cpp

manager: manager.cpp manager.h
	g++ -c manager.cpp

stats_reader: stats_reader.cpp stats_reader.h
	g++ -c stats_reader.cpp
	
clean:
	rm *.o gmonitor