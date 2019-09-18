# Game of Life

[Conway](https://en.wikipedia.org/wiki/John_Horton_Conway)'s [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) is entirely famous at this point.

This is a for-fun implementation of the game of life using ncurses.

## Features

* Runs at full terminal size
* Randomly seeds the universe with life
* Restarts the simulation with a new random board on `SIGHUP`
* Cleanly exits on `SIGINT` or `q`
* Allows modulation of the pause between ticks
* Allows pausing the simulation, and frame-by-frame playback once paused
