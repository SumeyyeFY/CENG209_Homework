# CENG209_Homework
__Sümeyye Feyza YEMENOĞLU 23050111011__

This repository will be used for submission of the CENG209 System Programming Homework

## Game Play
Game starts with a small greeting. Then the player enters his/her name.
Later, game shows some inrtoductions and hints about the game. Then, a
menu bar appears. User enters a command from the menu bar. In some choises,
user may need to enter a second command. For example, if player chose 'move'
then a second choise, direction, appears. Or, sometimes player is not able to 
choose that action. For example, if user tries to take the treasure from the
room he/she cannot be able compleate this action since picking up the treasure
has a condition of not alive creature in the room. Player moves inside and among 
the rooms, attack creatures and collect items. 

## Code Structure
The code is divided into several .h and .c files in order to provide more readable
code. Some structs and closely related functions are in same .h/.c file. Also, for 
compilation, there is a Makefile file. However, it may need to be updated according to
local computer.

## Game Logic
Some basic game logic are the following
    1. Creature does not attacks player before the attack of the player.
    2. Treasure, the item, in the room cannot be added to inventory before killing the
    creature in the room.
    3. Player can move inside of the rooms. 
    4. Player can change room through the (exit)gates.
    5. Player cannot go back the previous room.
