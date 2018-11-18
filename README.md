# Prehistoric Journey

A little 2D Platformer made by two students on second year of CITM UPC of Terrassa.

For the first version we had two levels where the player could move around, jump and climb lianas on the first level and rocks on the second one. 

Now 2 types of enemies have been added that, if close to you, will chase you down! Don't worry because now you can use your mighty weapon to defend yourself. All the game logic has been normalized so playing in faster or slower machines will result in the same gameplay.

For this game we used C++, using SDL and pugi libraries. The maps have been made using Tiled.

### Controls

- Left  (A)

- Right (D)

- Jump  (Espace)

- Attack (C)

- Climb (W,A,S,D). Press W when you are on top of a climbable part of the map to make the player grab onto it.

- Plus and minus in the keypad to change the volume

- Esc to close the game

### Debug Controls

- F1:  Start at the very first level

- F2:  Start at the second map

- F5:  Save the current state

- F6:  Load the previous state

- F9:  Draw colliders and enemies pathfinding

- F10: God Mode

- F11: Cap or uncap FPS. (Capped fps equals 30fps)

- F3: To see camera culling in work with 1 less tile

*Note: You won't be able to save or load while the game is changing scenes to avoid errors.*

## Innovation

- Camera culling, only printing what the camera sees at the moment.

- Volume changing mid-game with keypad plus and minus.

- Entities wander, if they are not following you, start moving around.

### Authors and accomplishments on this assignment

- [Ricardo Gutiérrez Llenas:](https://github.com/Ricardogll) Pathfinding. Normalized logic. Entity system. 



- [Sergio Gómez Roldán:](https://github.com/Sersius) Audio and animation. Entities wander. UML. Brofiler. Diverse tweaks.

- [Project](https://github.com/Ricardogll/Prehistoric-Journey)



### Disclosure

We do not own any of the art, music or fx. Authors credited below

- Art and music: by PixelBoy https://www.gamedevmarket.net/member/Pixelboy

- Fx:  https://freesound.org

## License

MIT License

Copyright (c) 2018 

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
