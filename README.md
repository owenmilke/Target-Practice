<h1>Target Practice - Owen Milke</h1>

<h2>Required Installations:</h2>
There are no additional installations to run this program.
<h2>Program Summary:</h2>
<h4>Program Description:</h4>
My program is a virtual target practice game similar to other online aim training programs and inspired by the aim trainer module found at https://humanbenchmark.com.

The purpose of my aim training program is to provide the user with a simplified environment to improve their computer mouse speed and accuracy in a fun, competitive way.

The program consists of two main modes, normal mode, and challenge mode.
In normal mode, a total of 20 targets spawn at random positions onscreen and the user has as long as they need to successfully click them all.
In challenge mode, a total of 30 targets spawn at random positions onscreen, with each target also quickly moving either left or right across the screen.
Each game mode also tracks the time it takes the user to click all targets, as well as the total number of times the user clicks their mouse in order to determine their click accuracy.

In each game mode, the game is completed by successfully clicking the total amount of targets, with each target spawning only after the previous one has been clicked.
Once the user successfully clicks all the targets in either mode, they will be shown the time it took them to complete the game, their overall click accuracy, and will be prompted to return to the main menu.
In addition to each of the game modes, the user's personal best times for each mode will be shown on the main menu once they have played a game, and it will update each time they beat that time.
Users also have the ability to choose the color of the in-game targets from the main menu, being able to choose between red, green, blue, yellow, cyan, or magenta targets, with red being the default target color.

<h3>Concepts Used:</h3>

* GLFW graphics framework
* Keyboard inputs (menu transitions and ability to change target color)
* Mouse inputs (hover, and click effects)
* Non-input-based events (timers)
* Multiple screens in the same graphics window (menu screen, normal mode, challenge mode, end screen)
* Graphics with C++ classes

<h3>Known Bugs:</h3>
The only bug I can think of occurring would be if the user takes a very long time on a game, which would result in the elapsed time value not being fully shown. However, I wouldn't expect this issue to arise during normal gameplay.

<h3>Future Work:</h3>

* Tracking the user's overall click accuracy for each mode on the main menu.
* Adding more game modes/target colors.
* Different colored game screen backgrounds.
* Potentially finding a way to keep the user's cursor within the program window, to combat overshooting when attempting to click targets near the edges of the screen.
