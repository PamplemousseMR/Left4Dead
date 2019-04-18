CCFLAGS_STD = -Wall -Werror -O3 -pedantic
CCFLAGS = $(CC) $(CCFLAGS_STD) -ansi -o
CCFLAGS_MAIN = $(CC) $(CCFLAGS_STD) -o
CCLIB = -lcurses -lpthread

COMMUNDIR = commun/

all : 
	make editor
	make game
	make server
	make player

#=============== Editor ===============

EDITOROUTDIR = editorOFiles/
EDITORDIR = editor/
EDITOREXEC = editor_exe

EDITOROBJETS = $(EDITOROUTDIR)main.o $(EDITOROUTDIR)ncurses.o $(EDITOROUTDIR)map.o $(EDITOROUTDIR)signal.o $(EDITOROUTDIR)option.o

editor : editor_directory editor_compile

editor_directory :
	mkdir -p $(EDITOROUTDIR)

editor_compile : $(EDITOROBJETS)
	$(CCFLAGS_MAIN) $(EDITOREXEC) $(EDITOROBJETS) $(CCLIB)

$(EDITOROUTDIR)main.o : $(EDITORDIR)main.c $(EDITORDIR)ncurses.h $(EDITORDIR)signal.h $(EDITORDIR)option.h $(COMMUNDIR)map.h
	 $(CCFLAGS) $(EDITOROUTDIR)main.o -c $(EDITORDIR)main.c

$(EDITOROUTDIR)option.o : $(EDITORDIR)option.c $(EDITORDIR)option.h $(COMMUNDIR)default.h
	$(CCFLAGS) $(EDITOROUTDIR)option.o -c $(EDITORDIR)option.c

$(EDITOROUTDIR)signal.o : $(EDITORDIR)signal.c $(EDITORDIR)signal.h
	$(CCFLAGS) $(EDITOROUTDIR)signal.o -c $(EDITORDIR)signal.c

$(EDITOROUTDIR)ncurses.o : $(EDITORDIR)ncurses.c $(EDITORDIR)ncurses.h $(COMMUNDIR)map.h
	$(CCFLAGS) $(EDITOROUTDIR)ncurses.o -c $(EDITORDIR)ncurses.c

$(EDITOROUTDIR)map.o : $(COMMUNDIR)map.c $(COMMUNDIR)map.h
	$(CCFLAGS) $(EDITOROUTDIR)map.o -c $(COMMUNDIR)map.c

#=============== Game ===============

GAMEOUTDIR = gameOFiles/
GAMEDIR = game/
GAMEEXEC = game_exe

GAMEOBJETS = $(GAMEOUTDIR)main.o $(GAMEOUTDIR)option.o $(GAMEOUTDIR)signal.o $(GAMEOUTDIR)IPC.o $(GAMEOUTDIR)map.o $(GAMEOUTDIR)items.o $(GAMEOUTDIR)zombie.o $(GAMEOUTDIR)human.o $(GAMEOUTDIR)messages.o

game : game_directory game_compile

game_directory :
	mkdir -p $(GAMEOUTDIR)

game_compile : $(GAMEOBJETS)
	$(CCFLAGS_MAIN) $(GAMEEXEC) $(GAMEOBJETS) $(CCLIB)

$(GAMEOUTDIR)main.o : $(GAMEDIR)main.c $(GAMEDIR)option.h $(GAMEDIR)signal.h $(GAMEDIR)IPC.h $(GAMEDIR)items.h $(GAMEDIR)messages.h $(GAMEDIR)zombie.h $(GAMEDIR)human.h $(COMMUNDIR)map.h
	$(CCFLAGS) $(GAMEOUTDIR)main.o -c $(GAMEDIR)main.c

$(GAMEOUTDIR)messages.o : $(GAMEDIR)messages.c $(GAMEDIR)messages.h $(GAMEDIR)IPC.h $(GAMEDIR)zombie.h $(GAMEDIR)human.h $(COMMUNDIR)messagesStruct.h
	$(CCFLAGS) $(GAMEOUTDIR)messages.o -c $(GAMEDIR)messages.c

$(GAMEOUTDIR)human.o : $(GAMEDIR)human.c $(GAMEDIR)human.h $(GAMEDIR)IPC.h $(GAMEDIR)items.h $(GAMEDIR)zombie.h $(COMMUNDIR)messagesStruct.h $(COMMUNDIR)IPCAccess.h $(COMMUNDIR)map.h
	$(CCFLAGS) $(GAMEOUTDIR)human.o -c $(GAMEDIR)human.c

$(GAMEOUTDIR)items.o : $(GAMEDIR)items.c $(GAMEDIR)items.h $(GAMEDIR)IPC.h $(GAMEDIR)human.h $(COMMUNDIR)map.h $(COMMUNDIR)IPCAccess.h
	$(CCFLAGS) $(GAMEOUTDIR)items.o -c $(GAMEDIR)items.c

$(GAMEOUTDIR)zombie.o : $(GAMEDIR)zombie.c $(GAMEDIR)zombie.h $(GAMEDIR)IPC.h $(COMMUNDIR)messagesStruct.h $(COMMUNDIR)IPCAccess.h $(COMMUNDIR)map.h
	$(CCFLAGS) $(GAMEOUTDIR)zombie.o -c $(GAMEDIR)zombie.c

$(GAMEOUTDIR)IPC.o : $(GAMEDIR)IPC.c $(GAMEDIR)IPC.h $(GAMEDIR)option.h  $(COMMUNDIR)map.h $(COMMUNDIR)IPCAccess.h
	$(CCFLAGS) $(GAMEOUTDIR)IPC.o -c $(GAMEDIR)IPC.c

$(GAMEOUTDIR)signal.o : $(GAMEDIR)signal.c $(GAMEDIR)signal.h
	$(CCFLAGS) $(GAMEOUTDIR)signal.o -c $(GAMEDIR)signal.c

$(GAMEOUTDIR)option.o : $(GAMEDIR)option.c $(GAMEDIR)option.h $(COMMUNDIR)default.h
	$(CCFLAGS) $(GAMEOUTDIR)option.o -c $(GAMEDIR)option.c

$(GAMEOUTDIR)map.o : $(COMMUNDIR)map.c $(COMMUNDIR)map.h
	$(CCFLAGS) $(GAMEOUTDIR)map.o -c $(COMMUNDIR)map.c

#=============== Server ===============

SERVEROUTDIR = serverOFiles/
SERVERDIR = server/
SERVEREXEC = server_exe

SERVEROBJETS = $(SERVEROUTDIR)main.o $(SERVEROUTDIR)option.o $(SERVEROUTDIR)signal.o $(SERVEROUTDIR)IPC.o $(SERVEROUTDIR)connection.o $(SERVEROUTDIR)UDPData.o

server : server_directory server_compile

server_directory :
	mkdir -p $(SERVEROUTDIR)

server_compile : $(SERVEROBJETS)
	$(CCFLAGS_MAIN) $(SERVEREXEC) $(SERVEROBJETS) $(CCLIB)

$(SERVEROUTDIR)main.o : $(SERVERDIR)main.c $(SERVERDIR)option.h $(SERVERDIR)signal.h $(SERVERDIR)IPC.h $(SERVERDIR)connection.h $(SERVERDIR)UDPData.h
	$(CCFLAGS) $(SERVEROUTDIR)main.o -c $(SERVERDIR)main.c

$(SERVEROUTDIR)UDPData.o : $(SERVERDIR)UDPData.c $(SERVERDIR)UDPData.h $(SERVERDIR)option.h $(SERVERDIR)connection.h $(COMMUNDIR)messagesStruct.h
	$(CCFLAGS) $(SERVEROUTDIR)UDPData.o -c $(SERVERDIR)UDPData.c

$(SERVEROUTDIR)connection.o : $(SERVERDIR)connection.c $(SERVERDIR)connection.h $(SERVERDIR)option.h $(SERVERDIR)IPC.h $(COMMUNDIR)IPCAccess.h $(COMMUNDIR)messagesStruct.h $(COMMUNDIR)default.h
	$(CCFLAGS) $(SERVEROUTDIR)connection.o -c $(SERVERDIR)connection.c

$(SERVEROUTDIR)IPC.o : $(SERVERDIR)IPC.c $(SERVERDIR)IPC.h $(SERVERDIR)option.h
	$(CCFLAGS) $(SERVEROUTDIR)IPC.o -c $(SERVERDIR)IPC.c

$(SERVEROUTDIR)signal.o : $(SERVERDIR)signal.c $(SERVERDIR)signal.h
	$(CCFLAGS) $(SERVEROUTDIR)signal.o -c $(SERVERDIR)signal.c

$(SERVEROUTDIR)option.o : $(SERVERDIR)option.c $(SERVERDIR)option.h $(COMMUNDIR)default.h
	$(CCFLAGS) $(SERVEROUTDIR)option.o -c $(SERVERDIR)option.c

#=============== Player ===============

PLAYEROUTDIR = playerOFiles/
PLAYERDIR = player/
PLAYEREXEC = player_exe

PLAYEROBJETS = $(PLAYEROUTDIR)main.o $(PLAYEROUTDIR)option.o $(PLAYEROUTDIR)signal.o $(PLAYEROUTDIR)connection.o $(PLAYEROUTDIR)ncurses.o $(PLAYEROUTDIR)map.o $(PLAYEROUTDIR)update.o $(PLAYEROUTDIR)getkey.o $(PLAYEROUTDIR)list.o $(PLAYEROUTDIR)astar.o $(PLAYEROUTDIR)move.o

player : player_directory player_compile

player_directory :
	mkdir -p $(PLAYEROUTDIR)

player_compile : $(PLAYEROBJETS)
	$(CCFLAGS_MAIN) $(PLAYEREXEC) $(PLAYEROBJETS) $(CCLIB)

$(PLAYEROUTDIR)main.o : $(PLAYERDIR)main.c $(PLAYERDIR)option.h $(PLAYERDIR)signal.h $(PLAYERDIR)connection.h $(PLAYERDIR)ncurses.h $(PLAYERDIR)update.h $(PLAYERDIR)getkey.h $(PLAYERDIR)move.h $(COMMUNDIR)default.h
	$(CCFLAGS) $(PLAYEROUTDIR)main.o -c $(PLAYERDIR)main.c

$(PLAYEROUTDIR)list.o : $(PLAYERDIR)Astar/list.c $(PLAYERDIR)Astar/list.h
	$(CCFLAGS) $(PLAYEROUTDIR)list.o -c $(PLAYERDIR)Astar/list.c

$(PLAYEROUTDIR)ncurses.o : $(PLAYERDIR)ncurses.c $(PLAYERDIR)ncurses.h $(PLAYERDIR)getkey.h $(COMMUNDIR)map.h $(COMMUNDIR)messagesStruct.h
	$(CCFLAGS) $(PLAYEROUTDIR)ncurses.o -c $(PLAYERDIR)ncurses.c

$(PLAYEROUTDIR)getkey.o : $(PLAYERDIR)getkey.c $(PLAYERDIR)getkey.h $(PLAYERDIR)update.h $(PLAYERDIR)move.h $(PLAYERDIR)connection.h
	$(CCFLAGS) $(PLAYEROUTDIR)getkey.o -c $(PLAYERDIR)getkey.c

$(PLAYEROUTDIR)move.o : $(PLAYERDIR)move.c $(PLAYERDIR)move.h $(PLAYERDIR)connection.h $(PLAYERDIR)update.h $(PLAYERDIR)Astar/astar.h $(PLAYERDIR)Astar/list.h $(COMMUNDIR)default.h
	$(CCFLAGS) $(PLAYEROUTDIR)move.o -c $(PLAYERDIR)move.c

$(PLAYEROUTDIR)astar.o : $(PLAYERDIR)Astar/astar.c $(PLAYERDIR)Astar/astar.h $(PLAYERDIR)Astar/list.h $(PLAYERDIR)update.h $(COMMUNDIR)default.h
	$(CCFLAGS) $(PLAYEROUTDIR)astar.o -c $(PLAYERDIR)Astar/astar.c

$(PLAYEROUTDIR)update.o : $(PLAYERDIR)update.c $(PLAYERDIR)update.h $(PLAYERDIR)connection.h $(PLAYERDIR)ncurses.h $(COMMUNDIR)map.h $(COMMUNDIR)messagesStruct.h
	$(CCFLAGS) $(PLAYEROUTDIR)update.o -c $(PLAYERDIR)update.c

$(PLAYEROUTDIR)connection.o : $(PLAYERDIR)connection.c $(PLAYERDIR)connection.h $(PLAYERDIR)option.h $(COMMUNDIR)IPCAccess.h $(COMMUNDIR)messagesStruct.h
	$(CCFLAGS) $(PLAYEROUTDIR)connection.o -c $(PLAYERDIR)connection.c

$(PLAYEROUTDIR)signal.o : $(PLAYERDIR)signal.c $(PLAYERDIR)signal.h
	$(CCFLAGS) $(PLAYEROUTDIR)signal.o -c $(PLAYERDIR)signal.c

$(PLAYEROUTDIR)option.o : $(PLAYERDIR)option.c $(PLAYERDIR)option.h $(COMMUNDIR)default.h
	$(CCFLAGS) $(PLAYEROUTDIR)option.o -c $(PLAYERDIR)option.c

$(PLAYEROUTDIR)map.o : $(COMMUNDIR)map.c $(COMMUNDIR)map.h
	$(CCFLAGS) $(PLAYEROUTDIR)map.o -c $(COMMUNDIR)map.c

#=============== Clean ===============

clean:
	rm -f $(EDITOROBJETS)
	rm -f $(EDITOREXEC)
	rm -r -f $(EDITOROUTDIR)
	rm -f $(SERVEROBJETS)
	rm -f $(SERVEREXEC)
	rm -r -f $(SERVEROUTDIR)
	rm -f $(GAMEOBJETS)
	rm -f $(GAMEEXEC)
	rm -r -f $(GAMEOUTDIR)
	rm -f $(PLAYEROBJETS)
	rm -f $(PLAYEREXEC)
	rm -r -f $(PLAYEROUTDIR)
