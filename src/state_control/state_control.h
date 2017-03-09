
#ifndef STATE_CONTROL_H
#define STATE_CONTROL_H

struct entity_s;

#define STATE_FUNCTIONS_LARA        (0x01)
#define STATE_FUNCTIONS_BAT         (0x02)
#define STATE_FUNCTIONS_WOLF        (0x03)
#define STATE_FUNCTIONS_BEAR        (0x04)
#define STATE_FUNCTIONS_RAPTOR      (0x05)
#define STATE_FUNCTIONS_TREX        (0x06)
#define STATE_FUNCTIONS_LARSON      (0x07)
#define STATE_FUNCTIONS_LION        (0x08)
#define STATE_FUNCTIONS_GORILLA     (0x09)
#define STATE_FUNCTIONS_CROCODILE   (0x0A)
#define STATE_FUNCTIONS_RAT         (0x0B)
#define STATE_FUNCTIONS_CENTAUR     (0x0C)

#define TR_STATE_CURRENT (-1)

void StateControl_SetStateFunctions(struct entity_s *ent, int functions_id);

#endif
