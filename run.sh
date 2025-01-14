#!/bin/bash

#Provide 2 args to start the server. Arg 1 is the webserver port to use in your browser. Arg 2 is the server port used to connect the web game to the game server

terminate() {
	echo "Shutting down..."
	kill -TERM "$pid" 2>/dev/null
	kill -TERM "$pid2" 2>dev/null
}

trap terminate SIGTERM

cd src/Client/
bash build.sh
./webserver $1 &
pid=$!
printf "\n"

cd ../Server/
bash build.sh
./server $2
pid2=$1
