# SisterRay
Sister Ray is a modularized engine re-implementation for FFVII. It is a rewrite of large sections of the engine designed to make source-code modding of the original game feasible and allow much more in-depth mods than have been possible in the past.

# Key Features: 
* A central event system, to which mods will be able to register callbacks using the sister-ray API
* A widget system to facilitate the create and manipulate of GUI elements and menus
* Modularization of much of the games battle engine to facilitate adding new commands, mechanics, damage formulas, etc

Further down the road:
A public API against which cod mods can be written and loaded. Data driven approaches to most game resources.

# Launch Mods

Sister-Ray includes default modifications to the game using the re-implemented engine. These include:
* In Battle PHS (Swapping characters a la FFX)
* Limit Break rework (Limit no longer replaces the attack command, and all known limits are usable instead of just the "current" level)
* Stagger Mechanics/Cast Times.
* Elemental and Status Resistances/Affinites for greater stat depth

These mods will be selectable/configurable in the eventual release

SisterRay is powered by Cid and Mog
