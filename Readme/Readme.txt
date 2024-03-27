Simple Wars by Chuang Sing, Kee
-https://github.com/Knae/SimpleWars
Credits:
Elite Command Art Units by Chris Vincent (https://opengameart.org/content/pixel-art-units-from-elite-command)
Smoke particle pack by Kenney Vleugels (www.kenney.nl)

*Notes
-Unit stats:
	-HP
	-MOV -Movement points
	-RGE -Range
	-DMG -Damage dealt
-Terrain has additional effects on unit in it. Effects include extra movement cost,damage multiplier, damage taken multiplier, range modifier.
-When in attack/move mode, click any empty space in the side panel to unselect the current unit and also exit that mode.
-When in attack/move mode, click any other unit to switch to that unit.
-When in move mode, text near the cursor indicates additional move cost if moving to the terrain under the mouse. It does not indicate if the unit can move into it.
-Units can only move to one adjacent tile at a time, no diagonal movement. Unit cannot move to a tile if doing so brings it's MOV points to less than 0
-Side panel has 2 sections that display terrain effect. The left refers to the terrain the current selected unit is on. The right refers to the terrain affecting the unit under the mouse, if any.
-Top 2 sections in the side panel display the selected units faction emblem and the faction stat bonuses. Unit factions are hardcoded to Blue-> Graysong Talons and Red->Lowvale Lynxes
-Blue units will have less HP for more range while Red units will have more damage but less range
-the third faction( Coldfield Ursine ) would have more HP but less movement
-Debug window changes the stats of the selected unit in realtime.


*Known Issues
-When switching between units with attack selected, overlay sometimes does not apear.
-When attacking, overlay sometimes stops reverting back to the green movement overlay
-enemy units sometimes look like they are in between grids
-Factions buffs are implemented but only 2 factions are accessible. No way to select the factions at the start
-Wins are only recorded when a game ends, not when the game closed midway.
-particles were planned with base class and sprites already setup but not implemented
-Some errors in reading config files which haven't resulted in gameplay errors

*Possible expansions
-animations for particle effects
-faction selection menu
-sound that plays with particle effects