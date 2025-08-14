## Install dependencies
```
make install
```

## Build the game
```
make
```

## Show run options
```
./pacman --help
```

## Run the game in a maximised window (recommended the first time)
```
./pacman
```

## Run the game in full screen
```
./pacman --window-mode=1
```

The game will run by default on the main display. To run it as full screen on a secondary screen, use:
```
./pacman --window-mode=1 --display=1
```

To discover the number of displays available, run: 
```
./pacman --graphics-info
``` 

The game will run with sound enabled by default. If you want to disable sound at the start, use the `--no-sound` option:
```
./pacman --no-sound
```

## Commands

`S`: enable/disable sound
`<up>` or `K`: thrust the ship
`<left>` or `H`: rotate the ship left
`<right>` or `L`: rotate the ship right
`<space>`: shoots
