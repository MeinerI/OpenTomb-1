-- OPENTOMB LEVEL SCRIPT
-- FOR TOMB RAIDER, CUT4

print("Level script loaded (CUT4.lua)");

level_PostLoad = function()
    playStream(22);
    entity_funcs[1] = {};
    entity_funcs[1].t = 0;
    entity_funcs[1].onLoop = function(object_id, tick_state)
        entity_funcs[1].t = entity_funcs[1].t + frame_time;
        if(not setCameraFrame(entity_funcs[1].t * 30) or (0 ~= getActionState(act.look))) then
            gameflowSend(GF_OP_LEVELCOMPLETE, getLevel() + 1);
        end;
    end;
end;

level_PreLoad = function()
    setCinematicTransform(49664, 51712, 18688, -90);
end;