
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "script.h"

#include "../core/system.h"
#include "../core/gl_text.h"
#include "../core/console.h"
#include "../core/vmath.h"
#include "../core/polygon.h"
#include "../state_control/state_control.h"
#include "../entity.h"
#include "../world.h"
#include "../character_controller.h"



int lua_CharacterCreate(lua_State * lua)
{
    int top = lua_gettop(lua);
    if(top >= 1)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && !ent->character)
        {
            Character_Create(ent);
            if(top >= 2)
            {
                ent->character->parameters.param[PARAM_HEALTH] = lua_tonumber(lua, 2);
                ent->character->parameters.maximum[PARAM_HEALTH] = lua_tonumber(lua, 2);
            }
        }
        else
        {
            Con_Warning("no entity with id = %d, or character already created", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("characterCreate: expecting arguments (entity_id, (hp))");
    }

    return 0;
}


int lua_SetCharacterStateControlFunctions(lua_State * lua)
{
    int top = lua_gettop(lua);
    if(top >= 2)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            StateControl_SetStateFunctions(ent, lua_tointeger(lua, 2));
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("setCharacterStateControlFunctions: expecting arguments (entity_id, funcs_id)");
    }
    return 0;
}


int lua_SetCharacterTarget(lua_State * lua)
{
    int top = lua_gettop(lua);
    if(top >= 1)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            ent->character->target_id = ((top > 1) && !lua_isnil(lua, 2)) ? (lua_tointeger(lua, 2)) : (ENTITY_ID_NONE);
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("setCharacterTarget: expecting arguments (entity_id, (target_id))");
    }
    return 0;
}


int lua_GetCharacterParam(lua_State * lua)
{
    if(lua_gettop(lua) >= 2)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            int parameter = lua_tointeger(lua, 2);
            if(parameter < PARAM_LASTINDEX)
            {
                lua_pushnumber(lua, Character_GetParam(ent, parameter));
                return 1;
            }
            else
            {
                Con_Warning("wrong option index, expecting id < %d", PARAM_LASTINDEX);
            }
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("getCharacterParam: expecting arguments (entity_id, param)");
    }

    return 0;
}


int lua_SetCharacterParam(lua_State * lua)
{
    int top = lua_gettop(lua);
    if(top >= 3)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            int parameter = lua_tointeger(lua, 2);
            if(parameter < PARAM_LASTINDEX)
            {
                if(top == 3)
                {
                    Character_SetParam(ent, parameter, lua_tonumber(lua, 3));
                }
                else
                {
                    ent->character->parameters.param[parameter] = lua_tonumber(lua, 3);
                    ent->character->parameters.maximum[parameter] = lua_tonumber(lua, 4);
                }
            }
            else
            {
                Con_Warning("wrong option index, expecting id < %d", PARAM_LASTINDEX);
            }
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("setCharacterParam: expecting arguments (entity_id, param, value, (max_value))");
    }

    return 0;
}


int lua_GetCharacterCombatMode(lua_State * lua)
{
    if(lua_gettop(lua) >= 1)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            lua_pushnumber(lua, ent->character->weapon_current_state);
            return 1;
        }
    }

    return 0;
}


int lua_ChangeCharacterParam(lua_State * lua)
{
    if(lua_gettop(lua) >= 3)
    {
        entity_p ent   = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            int parameter  = lua_tointeger(lua, 2);
            if(parameter < PARAM_LASTINDEX)
            {
                Character_ChangeParam(ent, parameter, lua_tonumber(lua, 3));
            }
            else
            {
                Con_Warning("wrong option index, expecting id < %d", PARAM_LASTINDEX);
            }
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("changeCharacterParam: expecting arguments (entity_id, param, value)");
    }

    return 0;
}


int lua_AddCharacterHair(lua_State *lua)
{
    if(lua_gettop(lua) != 2)
    {
        Con_Warning("addCharacterHair: expecting arguments (entity_id, hair_setup_index)");
    }
    else
    {
        entity_p ent   = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            hair_setup_s *hair_setup = Hair_GetSetup(lua, lua_tointeger(lua, 2));
            if(hair_setup)
            {
                ent->character->hair_count++;
                ent->character->hairs = (struct hair_s**)realloc(ent->character->hairs, (sizeof(struct hair_s*) * ent->character->hair_count));
                ent->character->hairs[ent->character->hair_count-1] = Hair_Create(hair_setup, ent->physics);
                if(!ent->character->hairs[ent->character->hair_count - 1])
                {
                    ent->character->hair_count--;
                    Con_Warning("can not create hair for entity_id = %d", lua_tointeger(lua, 1));
                }
            }
            else
            {
                Con_Warning("wrong hair setup index = %d", lua_tointeger(lua, 2));
            }
            free(hair_setup);
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    return 0;
}


int lua_ResetCharacterHair(lua_State *lua)
{
    if(lua_gettop(lua) == 1)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            if(ent->character->hairs)
            {
                for(int i = 0; i < ent->character->hair_count; i++)
                {
                    Hair_Delete(ent->character->hairs[i]);
                    ent->character->hairs[i] = NULL;
                }
                free(ent->character->hairs);
                ent->character->hairs = NULL;
                ent->character->hair_count = 0;
            }
            else
            {
                Con_Warning("can not create hair for entity_id = %d", lua_tointeger(lua, 1));
            }
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("resetCharacterHair: expecting arguments (entity_id)");
    }
    return 0;
}


int lua_AddEntityRagdoll(lua_State *lua)
{
    if(lua_gettop(lua) >= 2)
    {
        entity_p ent   = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent)
        {
            struct rd_setup_s *ragdoll_setup = Ragdoll_GetSetup(lua, lua_tointeger(lua, 2));
            if(ragdoll_setup)
            {
                if(!Ragdoll_Create(ent->physics, ent->bf, ragdoll_setup))
                {
                    Con_Warning("can not create ragdoll for entity_id = %d", lua_tointeger(lua, 1));
                }
                ent->type_flags |=  ENTITY_TYPE_DYNAMIC;
                Ragdoll_DeleteSetup(ragdoll_setup);
            }
            else
            {
                Con_Warning("no ragdoll setup with id = %d", lua_tointeger(lua, 2));
            }
        }
        else
        {
            Con_Warning("no entity with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("addEntityRagdoll: expecting arguments (entity_id, ragdoll_setup_index)");
    }
    return 0;
}


int lua_RemoveEntityRagdoll(lua_State *lua)
{
    if(lua_gettop(lua) >= 1)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent)
        {
            if(!Ragdoll_Delete(ent->physics))
            {
                Con_Warning("can not remove ragdoll for entity_id = %d", lua_tointeger(lua, 1));
            }
            ent->type_flags &= ~ENTITY_TYPE_DYNAMIC;
        }
        else
        {
            Con_Warning("no entity with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("removeEntityRagdoll: expecting arguments (entity_id)");
    }
    return 0;
}


int lua_SetCharacterState(lua_State * lua)
{
    if(lua_gettop(lua) >= 3)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            int8_t value = (int8_t)lua_tointeger(lua, 3);
            switch(lua_tointeger(lua, 2))
            {
                case CHARACTER_STATE_DEAD:
                    ent->character->state.dead = value;
                    break;

                case CHARACTER_STATE_SLIDE:
                    ent->character->state.slide = value;
                    break;

                default:
                    break;
            }
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("setCharacterState: expecting arguments (entity_id, response_id, value)");
    }

    return 0;
}


int lua_GetCharacterState(lua_State * lua)
{
    if(lua_gettop(lua) >= 2)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            switch(lua_tointeger(lua, 2))
            {
                case CHARACTER_STATE_DEAD:
                    lua_pushinteger(lua, ent->character->state.dead);
                    break;

                case CHARACTER_STATE_SLIDE:
                    lua_pushinteger(lua, ent->character->state.slide);
                    break;

                default:
                    lua_pushnil(lua);
                    break;
            }
            return 1;
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("getCharacterState: expecting arguments (entity_id, response_id)");
    }

    return 0;
}


int lua_SetCharacterWeaponModel(lua_State *lua)
{
    if(lua_gettop(lua) >= 3)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            Character_SetWeaponModel(ent, lua_tointeger(lua, 2), lua_tointeger(lua, 3));
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Printf("setCharacterWeaponModel: expecting arguments (id_entity, id_weapon_model, armed_state)");
    }

    return 0;
}


int lua_GetCharacterCurrentWeapon(lua_State *lua)
{
    if(lua_gettop(lua) >= 1)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            lua_pushinteger(lua, ent->character->current_weapon);
            return 1;
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Warning("getCharacterCurrentWeapon: expecting arguments (entity_id)");
    }

    return 0;
}


int lua_SetCharacterCurrentWeapon(lua_State *lua)
{
    if(lua_gettop(lua) >= 2)
    {
        entity_p ent = World_GetEntityByID(lua_tointeger(lua, 1));
        if(ent && ent->character)
        {
            ent->character->current_weapon = lua_tointeger(lua, 2);
        }
        else
        {
            Con_Warning("no character with id = %d", lua_tointeger(lua, 1));
        }
    }
    else
    {
        Con_Printf("setCharacterCurrentWeapon: expecting arguments (id_entity, id_weapon)");
    }

    return 0;
}


void Script_LuaRegisterCharacterFuncs(lua_State *lua)
{
    lua_register(lua, "addEntityRagdoll", lua_AddEntityRagdoll);
    lua_register(lua, "removeEntityRagdoll", lua_RemoveEntityRagdoll);

    lua_register(lua, "characterCreate", lua_CharacterCreate);
    lua_register(lua, "setCharacterStateControlFunctions", lua_SetCharacterStateControlFunctions);
    lua_register(lua, "setCharacterTarget", lua_SetCharacterTarget);
    lua_register(lua, "getCharacterParam", lua_GetCharacterParam);
    lua_register(lua, "setCharacterParam", lua_SetCharacterParam);
    lua_register(lua, "changeCharacterParam", lua_ChangeCharacterParam);

    lua_register(lua, "getCharacterState", lua_GetCharacterState);
    lua_register(lua, "setCharacterState", lua_SetCharacterState);
    lua_register(lua, "getCharacterCurrentWeapon", lua_GetCharacterCurrentWeapon);
    lua_register(lua, "setCharacterCurrentWeapon", lua_SetCharacterCurrentWeapon);
    lua_register(lua, "setCharacterWeaponModel", lua_SetCharacterWeaponModel);
    lua_register(lua, "getCharacterCombatMode", lua_GetCharacterCombatMode);
    lua_register(lua, "addCharacterHair", lua_AddCharacterHair);
    lua_register(lua, "resetCharacterHair", lua_ResetCharacterHair);
}