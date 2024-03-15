#ifndef ELYSIUM_ENTITY_H
#define ELYSIUM_ENTITY_H

/* 12 female npcs */
/* 12 male npcs */
/* 2 non-binary npcs */
#define NPC_IDS                                                            \
        /* Female */                                                       \
        NPC_ID(Amanda, 'A', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)       \
        /* Male */                                                         \
        NPC_ID(Bjorn, 'B', B_1, A_0, GRASS, COLOR_GREEN, COLOR_BLUE)       \
        /* Male */                                                         \
        NPC_ID(Cristobal, 'C', C_1, A_0, SAND, COLOR_YELLOW, COLOR_CYAN)   \
        /* Male */                                                         \
        NPC_ID(Dimitrios, 'D', D_1, D_0, GRASS, COLOR_GREEN, COLOR_YELLOW) \
        /* Female */                                                       \
        NPC_ID(Evelyn, 'E', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)       \
        /* Male */                                                         \
        NPC_ID(Felix, 'F', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)        \
        /* Male */                                                         \
        NPC_ID(Guy, 'G', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)          \
        /* Female */                                                       \
        NPC_ID(Harini, 'H', H_1, H_0, STONE, COLOR_WHITE, COLOR_YELLOW)    \
        /* Male */                                                         \
        NPC_ID(Ichabod, 'I', I_1, I_0, GRASS, COLOR_GREEN, COLOR_MAGENTA)  \
        /* Ambiguous */                                                    \
        NPC_ID(Jesse, 'J', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)        \
        /* Male */                                                         \
        NPC_ID(Kristoff, 'K', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)     \
        /* Female */                                                       \
        NPC_ID(Lin, 'L', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)          \
        /* Female */                                                       \
        NPC_ID(Mandy, 'M', M_1, M_0, SAND, COLOR_YELLOW, COLOR_CYAN)       \
        /* Male */                                                         \
        NPC_ID(Nick, 'N', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)         \
        /* Female */                                                       \
        NPC_ID(Omari, 'O', O_1, O_0, STONE, COLOR_WHITE, COLOR_MAGENTA)    \
        /* Female */                                                       \
        NPC_ID(Penelope, 'P', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)     \
        /* Female */                                                       \
        NPC_ID(Quana, 'Q', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)        \
        /* Ambiguous */                                                    \
        NPC_ID(Riley, 'R', R_1, R_0, STONE, COLOR_WHITE, COLOR_RED)        \
        /* Female */                                                       \
        NPC_ID(Sui, 'S', S_1, S_0, SAND, COLOR_YELLOW, COLOR_GREEN)        \
        /* Male */                                                         \
        NPC_ID(Theo, 'T', T_1, T_0, GRASS, COLOR_GREEN, COLOR_RED)         \
        /* Male */                                                         \
        NPC_ID(Ulrich, 'U', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)       \
        /* Female */                                                       \
        NPC_ID(Valentina, 'V', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)    \
        /* Male */                                                         \
        NPC_ID(Wang, 'W', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)         \
        /* Male */                                                         \
        NPC_ID(Xavier, 'X', X_1, X_0, GRASS, COLOR_WHITE, COLOR_MAGENTA)   \
        /* Male */                                                         \
        NPC_ID(Yves, 'Y', A_1, A_0, GRASS, COLOR_GREEN, COLOR_RED)         \
        /* Female */                                                       \
        NPC_ID(Zora, 'Z', A_1, A_0, GRASS, COLOR_GREEN, COLOR_BLUE)

enum npc_id {
#define NPC_ID(ID, LETTER, NEW_GAME_DIALOGUE_ID, ENDING_DIALOGUE_ID, \
               TERRAIN_KIND, TERRAIN_COLOR, COLOR)                   \
        ID,
        NPC_IDS
#undef NPC_ID
                N_NPC_IDS
};

/* List of NPC names */
extern const char *(npc_names[N_NPC_IDS]);

enum entity_kind {
        EMPTY,
        NPC,
        WALL,
        ekWINDOW,
        DOOR,
        KEY,
        PORTAL,
        SIGN,
        SURFBOARD,
        PURPLE_HIBISCUS,
        SEASHELL,
        TREASURE,
        N_ENTITY_KINDS
};

extern char entity_glyphs[N_ENTITY_KINDS];

#endif
