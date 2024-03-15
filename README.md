# Elysium

An ASCII adventure game where you try to convince 26 people to join you at the
Elysian Field.

Elysium is still a work in progress, but you can try a demo by downloading and
running one of the binaries in the releases section.

## Building from source

Elysium's build system relies on the following software:

- GCC
- Make
- Python 3

To bulild Elysium from source, clone this repo, navigate to it on the command
line, and run `make`. This will generate the `elysium` binary, which you may run
to play the game.

## Development

Elysium's C source files are in the `src` directory and their associated headers
are in the `include` directory. Elysium's game data is mostly separate from its
logic, and can be found in the `data` folder. In this folder there are three
files:

- `world.txt` which contain the data for the game's map. The file `gen_menus.py`
  parses this file and translates it into a C `char *` array in the file
  `world.c`, to be directly embedded in the game's code. When the player starts
  a new game, the `new_game()` function defined in the file `src/menu_manual.c`
  reads this array to load in the the player, objects, and NPCs at their correct
  positions. If you would like to extend Elysium with a greater variety of
  entities, then you'll want to take a look at the `new_game()` function.
- `ending.txt`, which is similar to `world.txt` except that it specifies the map
  for the game's ending.
- `menus.json`, which contains the data for the game's NPCs' dialogue trees. The
  script `scripts/gen_menus.py` reads this file and generates the files `menu.h`
  and `menu.c` from its contents. `menus.json` is an array of arrays. The first
  element of each nested array says what type of entity it describes:

  - `var`iables that trigger different NPC dialogue. The second element of the
    array is the variable's name, and the third is its C type.
  - `menu`s that describe phrases the NPCs can say to the player, and how they
    can respond. The second element of the array is the menu's unique ID, the
    third is the text the NPC says for this menu, and the fourth is an array of
    the player's possible responses to this menu. Each response option is either
    the ID of a response they player can always respond with, or a two-element
    array whose second element is the option's ID, and whose first element is a
    condition that needs to evaluate to true at runtime for the option to be
    available.
  - `option`s that describe phrases the players can say to NPCs, and how they
    will respond. The second element of the array is the option's unique ID, the
    third element is the option's text, the fourth is the ID of the menu the NPC
    will respond with (or `null` if there is no response), and the fifth element
    is custom C code to run after the player selects this option.

  If you would like to extend Elysium's dialogue, check out `data/menus.json`
  and `scripts/gen_menus.py`.
