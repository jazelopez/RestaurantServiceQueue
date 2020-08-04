
CC=g++

FLAGS=-Wall -std=c++11

all: Driver DriverSlow DriverSlow2

Driver: Driver.cpp ServiceQueue.h
	$(CC) $(FLAGS) Driver.cpp -o Driver

DriverSlow: Driver.cpp ServiceQueueSlow.h
	$(CC) $(FLAGS) -D_SLOW Driver.cpp -o DriverSlow

DriverSlow2: Driver.cpp ServiceQueueSlow2.h
	$(CC) $(FLAGS) -D_SLOW2 Driver.cpp -o DriverSlow2

clean:
	rm -f Driver DriverSlow DriverSlow2


