MEMBERS: Aman , Tony Cao, Lyon Manono

make server
g++ player.cpp server.cpp -o server -lsfml-system -lsfml-network -lsfml-graphics -lsfml-window -lsfml-audio -lpthread

client:
g++ player.cpp client.cpp -o client -lsfml-system -lsfml-network -lsfml-graphics -lsfml-window -lsfml-audio -lpthread


run:
open 5 terminals

in terminal 1:
./server


in rest
./client


game starts once 4 clients join
