|==============================================================================|
|==============================================================================|
|==============================================================================|
|                             My Flappy Bird                                   |
|==============================================================================|
|                           Smeu Adina - 335CA                                 |
|==============================================================================|
|An implementation of the game "Flappy Bird".                                  |
|==============================================================================|
|The bird                                                                      |
|                                                                              |
|The bird consists of the following meshes:                                    |
|- the body:                                                                   |
|  - a circle that has a black center and all the other vertices are blue;     |
|  - its center is considered the centre of gravity of the bird;               |
|- the head:                                                                   |
|  - a circle that has a black center and all the other vertices are blue;     |
|- the eye:                                                                    |
|  - a white circle;                                                           |
|- the iris:                                                                   |
|  - a brown circle;                                                           |
|- the beak:                                                                   |
|  - a black triangle;                                                         |
|- the wing:                                                                   |
|  - a green triangle;                                                         |
|  - its flapping is mimicked by scaling the wing on the OY axis with a scale  |
|    factor that ranges between a positive and a negative value;               |
| - the tail:                                                                  |
|   - three triangles and three ellipsis;                                      |
|   - it rotates around the body.                                              |
|                                                                              |
|The bird's OY coordinates are modified either by the gravitational            |
|gravitational acceleration or by the jump acceleration. The bird also rotates |
|around its gravitational centre with an angle that is influenced by these     |
|accelerations.                                                                |
|==============================================================================|
|The obstacles                                                                 |
|                                                                              |
|A single mesh is created and the maximum number of pairs of obstacles that    |
|can appear on the screen is determined based on the resolution and the        |
distance between the obstacles.                                                |
|The position of the leftmost obstacle is stored, whereas the others are       |
|determined by applying translations and rotations on it.                      |
|The obstacles have various scales and textures and they are stored in vectors |
|that have the length of the maximum number of obstacles that can be rendered. |
|Each time the leftmost obstacle can no longer be rendered, the position is    |
|updated.                                                                      |
|The scales are randomly generated and the textures are randomly chosen from   |
|the existing ones.                                                            |
|==============================================================================|
|The logic                                                                     |
|                                                                              |
|The bird can fly up when the spacebar key is pressed. The obstacles move to   |
|the left side of the screen with a speed that keeps increasing. Only obstacles|
|that can be seen on the screen are rendered.                                  |
|The score increases every time a pair of obstacles disappears from the screen.|
|When the score becomes greater than 30, another scaling operation is applied  |
|on the obstacles. When the stalagmites grow, the stalactites shrink and the   |
|other way around.                                                             |
|The collisions are checked for the head and the body of the bird (it doesn't  |
|really hurt when it hits an obstacle with its tail, right?) and only with     |
|the obstacles that are near the bird (when the interval between the bird      |
|component's minimum and maximum x overlaps the interval between the obstacle's|
|minimum and maximum x.                                                        |
|The score is displayed at the end of the game using meshes generated for each |
|digit.                                                                        |
|==============================================================================|
|PS: No transformations depending on deltaTimeSeconds are applied on the scene |
|components on the first frame.                                                |
|==============================================================================|
|==============================================================================|
|==============================================================================|