#include "entity.h"

const char *(npc_names[N_NPC_IDS]) = {
#define NPC_ID(ID, LETTER, NEW_GAME_DIALOGUE_ID, ENDING_DIALOGUE_ID, \
               TERRAIN_KIND, TERRAIN_COLOR, COLOR)                   \
        #ID,
        NPC_IDS
#undef NPC_ID
};

char entity_glyphs[N_ENTITY_KINDS] = {
        ' ', /* EMPTY, */
        '@', /* NPC (should not be used), */
        '#', /* WALL, */
        ':', /* WINDOW, */
        '|', /* DOOR, */
        '%', /* KEY, */
        '0', /* PORTAL, */
        '?', /* SIGN, */
        '/', /* SURFBOARD, */
        ',', /* PURPLE_HIBISCUS, */
        '>', /* SEASHELL */
        '$', /* TREASURE */
};
