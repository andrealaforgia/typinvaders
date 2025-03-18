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
./typinvaders --help
```

## Run the game in a maximised window (recommended the first time)
```
./typinvaders
```

## Run the game in full screen
```
./typinvaders --window-mode=1
```

The game will run by default on the main display. To run it as full screen on a secondary screen, use:
```
./typinvaders --window-mode=1 --display=1
```

To discover the number of displays available, run: 
```
./typinvaders --graphics-info
``` 

The game will run with sound enabled by default. If you want to disable sound at the start, use the `--no-sound` option:
```
./typinvaders --no-sound
```

## Commands

`S`: enable/disable sound
`<up>` or `K`: thrust the ship
`<left>` or `H`: rotate the ship left
`<right>` or `L`: rotate the ship right
`<space>`: shoots
