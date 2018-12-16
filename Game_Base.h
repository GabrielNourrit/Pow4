#ifndef Game_base
#define Game_base

#include "type.h"

void creer_tablette(tablette tab);
int est_legal(configuration *config, coup *cp);
void jouer_coup(configuration *config, coup *cp);
coup ia_coup(configuration *config);
int est_finie(configuration *config, coup *cp);
void jeu(configuration config);
void ia(configuration config);
void fin_du_game(configuration*config);
void match_nul(configuration *config);

#endif
