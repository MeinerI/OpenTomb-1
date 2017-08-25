-- OPENTOMB LEVEL SCRIPT
-- FOR TOMB RAIDER, CUT2

print("Level script loaded (CUT2.lua)");

level_PostLoad = function()
    playStream(25);
    entity_funcs[0] = {};
    entity_funcs[0].t = 0;
    entity_funcs[0].onLoop = function(object_id, tick_state)
        entity_funcs[0].t = entity_funcs[0].t + frame_time;
        if(not setCameraFrame(entity_funcs[0].t * 30)) then
            setGame(GAME_1, 10);
        end;
    end;
end;

level_PreLoad = function()
    setCinematicTransform(36668, 63180, 0, 90 * 16380 / 16384);
end;