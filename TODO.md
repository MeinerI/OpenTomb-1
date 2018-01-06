OpenTomb — TODO list for high-priority bugs / tasks
---------------------------------------------------

### Table of contents ###

1. The main plan
2. Build configuration
3. Git/GitHub workflow
4. Code in main
5. Collision system
6. Character controller
7. Animation control
8. Camera control
9. Scripting
10. Audio


1. The main plan
----------------
First we need to implement TR1 gameplay, so TR1/2/3 functions tasks have higher priority over TR4/TR5 functions. The first objective is to make a simple, but stable and working version, then extend functionality to it step by step.

2. Build configuration
----------------------
* Todo:
	* Update internal image lib
	* Reduce number of dependecies
	* Make good autobuild system
	  
3. Git/GitHub workflow
---------------
* Commits to `master` may be:
	* Merge with feature branch (branch that contains new feature, or big bug fixing) - feature branch must be avaliable on GitHub; after merging it must be deleted
	* Little and clean bug fixes
	* Documentation updates
	* Build scripting updates (may be done in a feature branch, by author's choice)
	* Independent resources updates

* Creating feature branches and committing to them is free
* Special merge requests, not for merging or with delayed time for merging must contain the `[NOT_FOR_MERGING]` prefix; such requests may be merged only after request's author writes comment `[CAN_BE_MERGED_NOW]`
* 
* How to submit a feature branch to `master`:
	1. Install GIT bash and launch the terminal (or use your own GUI)
	2. Create a local feature branch by issuing: `git checkout -b feature_branch_name` (or make one in the repo with GitHub's web UI)
	3. Commit the local branch with `git add -u`, `git commit` (if you use Vim, press 'i', enter message, press escape, then enter)
	4. Merge the feature branch with the local master branch with `git merge master` and resolve any conflict (rebuild the project)
	5. Commit the changes and push the local feature branch to the remote on GitHub with: `git push origin feature_branch_name`
	6. Create a merge (pull) request with the GitHub web UI
	7. After merging is accepted, delete the feature branch

* _extern_ folder is not to be edited - it's a place for external libraries sources/headers
* _src/core_ folder: only by TeslaRus, make request if you want to change something
* Other folders: by merge requests to TeslaRus or, after code review (by merge request) by command (more details will be discussed); I will make some commits after merge request too (number of errors will be decreased significantly in case of review)

4. Code in main
---------------
* Todo:
	* `game.cpp`: many different logics in one place, needs to be refactored
	* make some modules (not all!) interface more abstract (hide internal realisation, like `physics.h`/`physics_bullet.cpp`)

5. Engine
-------------------
* Current situation:
	* Implemented entity spawning and safety deleting, projectiles, player switching...

* Todo:
	* Add activation callbacks for inventory items (no more `read-only` for inventory)
	* Reduce globals using (shared between modules globals)
	* Move out the console.c rendering code 
	* Make alt-room state savable in cases where there's a chain of 3 or more alt-rooms

6. Collision system
-------------------
* Current situation:
	* Fixed back/front-facing polygons orientation for physics geometry, now the engine has a working _Filtered Ray Test_ (skips back-faced polygons)
	* Collision margin is zero, otherwise normals in near edges become smooth and Lara slides down or stops in places she should not
	* Refactored collision callbacks implementation that allow to register hit damage and any other collisions
  
* Todo:
	* Fix moving after landing on sloped surface:
		* Find body parts that stop Lara
		* Tune collision form, or disable collision checking for them
		* Bind with 3
	* Make rigid body parts shapes tunable by config
	* For future optimazation, add switchable single ghost object for character
	* Add _Long Ray Test_ (pierces rooms portals and builds room list for collisional checking) - needed for long range shooting and AI
	* Re-implement Character_FixPosByFloorInfoUnderLegs(...) it has been deleted
	* Check room tween butterfly normals

7. Character controller
-----------------------
* Todo:
	* Base AI, path finding, boxes...
	* Weapon control system needs to be refactored/fixed (2-handed weapon model switches in wrong frame)
	* Add auto weapon hiding in water environment e.t.c. (simple task)
	* Fix usage of weapons while crouching

8. Animation control
--------------------
* Todo:
	* Update documentation about `ss_animation` structure and functions
	* Fix incorrect smoothing if there are _move_ or _rotate_ anim commands
	* Fix dive-rolls:
		* Roll is not commencing immediately while moving forward
		* Rolls distance too great (e.g. falling off 1x1m ledges when on opposite edge)
	* Fix forward and backward consecutive jump rolls (mid-air rolls) not concatenating correctly on keypress (TR2+)
	* Fix swan dive not doable when jumping off of irregular (diamond shaped <>) slopes
	* Fix edge climbing:
		* Climbability distance threshold too high when jumping (i.e. reaching heights that shouldn't be reached)
		* Reduce height correction when Lara lets go the hold on an edge (let go hold button)

9. Camera control
-----------------
* Todo:
	* Fix camera targeting to correct body part or OBB center (add special function to get targeting pos by target entity id)
	* Implement camera flags and their function (e.g. "flyby", "once")
	* Add special `camera_entity`, store it in world module, access by `entity_p World_GetCameraEntity();` - needed for heavy triggers

10. Scripting
------------
* Current situation:
	* SEE TRIGGERS_tasks.md
	* Scripts update EVERY game frame! Use the global engine frame time inside time depended scripts!
* Todo:
	* Add function like `lua_SaveTable(...)` that recursively print to file/buffer/clay tablets lua code with table content (i.e. `table_name = { red = 1; green = 0; blue = 0; name = "name"; is_u = true; in_tbl = { p1 = "inner"; val = 32.45 } }`)
	* In all scripts that may change game state, data must be stored in special global table (that will be saved in save game) - needed for game save/load functions to correctly work

11. Audio
---------
* Current situation:
	* Sound tracks playing was disabled
	* AL build-in library works on Windows and MacOS, but under Linux native AL library are required
* Todo:
	* In `audio.cpp` implement class for sound track data manipulation (e.g. `result GetBufferData(track_id, buffer, size, offset, flag)`)
	* Implement own audio routine thread (APIs like `Audio_Send(...)` allow that)
	* Use something else instead of Vorbis (it can't read _OGG_ from memory, and uses default functions for files opening, so engine can't precache tracks in memory or use `SDL_rwops`)
