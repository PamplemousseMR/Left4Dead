# Left4Dead

Multiplayer game project in several applications using IPC Linux.
The object of the game is to survive in a territory infested with zombies by recovering resources on the map.

The common part of the game consists of two applications, a game engine managing its logic and a server
managing communications. Players have the "client" application and thus the game interface.
There is also a fourth application for creating and editing maps.

The client and editor interface were made with the ncurses library and react to the mouse.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

- [ncurses](https://en.wikipedia.org/wiki/Ncurses) : A library providing an API that allows to write Text-based user interface in a terminal.
- [pthread](https://en.wikipedia.org/wiki/POSIX_Threads) : A library providing an API that allows to control multiple different flows of work that overlap in time.

### compilation

Compile using the makefile : `make`.

```
- editor : Compiles the editor.
- server : Compile the server.
- game : Compiles the game engine.
- player : Compile the game.
- clean : Clean previous compilation.
```

### use

Start the editor to create maps : `./editor`.
```
-h [options] : Displays help for execution arguments. 
-n [options] <path> : Specify the name of the backup file.
-l [options] <path> : Specify a file to load.
```

Start the game to create the game motor : `./game`.
```
-h [options] : Displays help for execution arguments. 
-c [options] <key> : to specify the key of the message queue.
-s [options] <key> : to specify the key of the semaphore array. 
-m [options] <key> : Specify the key for the shared memory segment.
-n [options] <name> : Allows to specify name of the named tube.
-d [options] : Cleans the IPC if there is a problem during a previous creation that would not have deleted the IPC tools itself.
```

Start the server to create the server : `./server`.
```
-h [options] : Displays help for execution arguments. 
-c [options] <key> : to specify the key of the message queue.
-s [options] <key> : to specify the key of the semaphore array. 
-m [options] <key> : Specify the key for the shared memory segment.
-p [options] <port>: Specify the first TCP connection port.
-u [options] <port>: Specify the UDP connection port.
-n [options] <name> : Allows to specify name of the named tube.
```

Start the player to create a new player : `./player`.
```
-h [options] : Displays help for execution arguments. 
-p [options] <port> : Specify the first TCP connection port.
-u [options] <port> : Specify the UDP connection port.
-a [options] <name> : Allows to specify address of the server.
```

## Authors

* **MANCIAUX Romain** - *Initial work* - [PamplemousseMR](https://github.com/PamplemousseMR).

## License

This project is licensed under the GNU Lesser General Public License v3.0 - see the [LICENSE.md](LICENSE.md) file for details.