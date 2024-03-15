CC=gcc
DEBUG_TARGET=elysium_deb
RELEASE_TARGET=elysium
SRCS=src/*.c
HDRS=include/*.h
LIBS=-lncurses

MENU_GEN=scripts/gen_menus.py
WORLD_GEN=scripts/gen_world.c.py
MENU_H=include/menu.h
MENU_C=src/menu.c
WORLD_C=src/world.c
ENDING_C=src/ending.c
GENERATED=$(MENU_C) $(MENU_H) $(WORLD_C)

WORLD_HEIGHT_ROWS = 80
WORLD_WIDTH_COLS = 240
SCENE_HEIGHT_ROWS = 16
SCENE_WIDTH_COLS = 48

CC_FLAGS=-Wall -Wextra -std=c89 -pedantic -pedantic-errors \
		 -Wmissing-prototypes -Wstrict-prototypes -Werror \
		 -Wold-style-definition -O3 -Iinclude

.PHONY:	check clean debug release run_deb run_rel

debug: $(DEBUG_TARGET)
release: $(RELEASE_TARGET)

$(DEBUG_TARGET):	$(SRCS) $(HDRS) $(GENERATED)
	$(CC) $(CC_FLAGS) $(SRCS) $(LIBS) -DDEBUG -o $(DEBUG_TARGET)

$(RELEASE_TARGET):	$(SRCS) $(HDRS) $(GENERATED)
	$(CC) $(CC_FLAGS) $(SRCS) $(LIBS) -o $(RELEASE_TARGET)

$(WORLD_C):	data/world.txt $(WORLD_GEN)
	./scripts/gen_world.c.py "$<" "$(WORLD_C)" $(WORLD_HEIGHT_ROWS) $(WORLD_WIDTH_COLS) base_world WORLD_HEIGHT_ROWS WORLD_WIDTH_COLS

$(ENDING_C):	data/ending.txt $(WORLD_GEN)
	./scripts/gen_world.c.py "$<" "$(ENDING_C)" $(SCENE_HEIGHT_ROWS) $(SCENE_WIDTH_COLS) ending_world SCENE_HEIGHT_ROWS SCENE_WIDTH_COLS

$(MENU_H) $(MENU_C):	data/menus.json	$(MENU_GEN)
	./scripts/gen_menus.py "$<" "$(MENU_H)" "$(MENU_C)"

run_deb:	$(DEBUG_TARGET)
	./$(DEBUG_TARGET)

run_rel:	$(RELEASE_TARGET)
	./$(RELEASE_TARGET)

clean:
	rm -f $(DEBUG_TARGET) $(RELEASE_TARGET) $(GENERATED) *.save

check:
	$(CC) $(CC_FLAGS) -fsyntax-only $(SRCS) $(LIBS)
