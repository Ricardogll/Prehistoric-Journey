# **Our Project**

Game for the Videogame Development subject at UPC CITM Terrassa. Our goal is to make a data driven platformer with two levels made in Tiled with the player as a character moving and climbing on the levels, also being able to save and load the game. With all of this there is enemies pathfinding implemented along with animations and logic normalized based on the fps, which can be capped at 30 or uncapped. For the entities and UI we have a structured system documented in UML.

## **The game**
A platformer where you are a caveman armed with a mace and you have to catch as much food as possible while batting against bats and T-Rexs running through the levels.

## **Team**

![](team_photo.png)

_Gutiérrez LLenas, Ricardo’s work and Github_
* Pathfinding, normalized logic, entity system, camera culling and UI functionality.
* [Github account](https://github.com/Ricardogll)

_Gómez Roldán, Sergio’s work and Github_
* Sprites, audio, entities wander, UI interaction and some modules of code.
* [Github account](https://github.com/Sersius)

## **Main Core subsystems**
The game code is structured in modules. The module j1App.cpp manages all the other modules calling in loop its respective awake, start, preupdate, update, postupdate, cleanup that they share through a base class j1Module.cpp.

For entities and UI we have implemented a respective factory system that manages everything that happens in their modules.

All animations, paths and levels are read through a xml file since this is a data driven project, to improve the readability of the code and at the same time to do the changes while testing more fast.

### **Innovations**
* Camera culling, only printing what the camera sees at the moment.

* Entities wander, if they are not following you they start moving around.

* Main menu with animation

## **Gameplay**
___
<iframe width="560" height="315" src="https://www.youtube.com/embed/L2X9tgzint4" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


## **How to play**
You have to catch as much food as possible for more score, you get score by beating enemies and collecting chicken. You have 3 lifes, if you run out of lifes you will return to the main menu, for the game controls check down down below. Hope you enjoy!

### **Controls**
~~~~~~~~~~~~~~~
Player controls:
- Left  (A)
- Right (D)
- Jump  (Espace)
- Attack (C)
- Climb (W,A,S,D). Press W when you are on top of a climbable part of the map to make the player grab onto it.


- Esc to pause the game and open the respective menu while in-game.

Debug Controls:
- F1:  Start at the very first level
- F2:  Start at the second map
- F5:  Save the current state
- F6:  Load the previous state
- F8:  UI debug mode
- F9:  Draw colliders and enemies pathfinding
- F10: God Mode
- F11: Cap or uncap FPS. (Capped fps equals 30fps)
- F3: To see camera culling in work with 1 less tile

*Note: You won't be able to save or load while the game is changing scenes or jumping to avoid errors.*
~~~~~~~~~~~~~~~

## **Important links**

> ### Check the Github repository [here.](https://github.com/Ricardogll/Prehistoric-Journey)
> ### Download the latest version of the game [here.](https://github.com/Ricardogll/Prehistoric-Journey/releases/tag/v1.0.2)

### Disclosure

We do not own any of the art, music or fx. Authors credited below

- Art and music: by PixelBoy https://www.gamedevmarket.net/member/Pixelboy

- Fx:  https://freesound.org

## License

MIT License

Copyright (c) 2018 Ricardo Gutiérrez Llenas and Sergio Gómez Roldán

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
