ShapeSync - A Unity3D Mobile Game

The goal of the game: keep two fingers contacting the white border of a random shape as it moves
around the screen, without touching the "danger zone" of the shape itself or outside of the border.
If you are in the danger zone for too long, you get a game over and you must start over. Stay in the
safe zone for the given period of time and you'll advance a level, where the movement patterns will
get faster and more erratic! Your score depends solely on how many levels you can get through.

This is a mobile game I built to experiment with Unity3D's functionality in building
2D games with multitouch functionality and an arcade-like accelerating difficulty curve.

The inspiration for this was my experience going to carnivals and playing a game where you
are given a metal rod with a ring at the end, and there's a rotating metal spiral that you
need to pass the ring through, but without the ring touching the spiral at all. Building on
this idea, the goal of the game is to maintain contact with the border of a given shape while
it moves in a random pattern, but you cannot touch the shape itself or outside of the shape.

The game includes 3D geometry generated for a 2D space, multitouch integration, saving high score
states, and a movement pattern and speed that scales based on progress.