|==============================================================================|
|==============================================================================|
|==============================================================================|
|                               "Worms 3D"                                     |
|==============================================================================|
|                           Smeu Adina - 335CA                                 |
|==============================================================================|
|A version of the game Worms 3D with deformable ground.                        |
|There are 2 players, each having the chance of launching one projectile per   |
|round. In case of a collision of the projectile with the ground, the later one|
|is deformed. If the other player is hit, then the current one wins the game.  |
|At the beginning of every round, the camera is placed behind the current worm.|
|When the projectile is launched, the camera moves with it and no user input is|
|permitted excrpt the extras.                                                  |
|==============================================================================|
|The worm                                                                      |
|                                                                              |
|The worm can be rotated around its OY axis using the left and right arrow. Its|
|gun can be rotated around its OX axis using the up and down arrows. In order  |
|to launch the projectile, the player has to adjust the worm's and its gun's   |
|positions and then press the left button of the mouse.                        |
|Each worm has a spotlight above it.                                           |
|==============================================================================|
|The projectile                                                                |
|                                                                              |
|The projectile consists of a textured sphere and can be seen at the front of  |
|the gun of the current player. When it is launched, it experiences a          |
|projectile motion (the air repellance is ignored, only the gravitational      |
|acceleration is used). When it hits the ground, it creates a crater.          |
|In order to detect if a collision with the player has occured, it is checked  |
|whether the 2 spheres intersect (the projectile and the bounding box of the   |
|player). For the ones with the ground, for each vertex of the ground situated |
|underneath the projectile it is checked whether the vertex is inside the      |
|sphere. If it is, then a crater is created. The depths variate based on the   |
|distance between the ground vertex and the collision point.                   |
|==============================================================================|
|The ground                                                                    |
|                                                                              |
|The ground consist of a square mesh made up of multiple vertices. Each vertex |
|has its x and z coordinates predetermined, whereas the y coordinate is        |
|extracted from a heightmap texture using the x and z coordinates as indices   |
|for the column and the line. The extracted height is multiplied with a        |
|constant factor in order to make the ground more appealing.                   |
|Another texture is used for the color of the mesh.                            |
|==============================================================================|
|The background                                                                |
|                                                                              |
|The whole action takes place inside a sphere that has a texture similar to the|
|sky and rotates around its OY axis.                                           |
|The sun consists of a textured sphere, rotates around the OY axis of the sky  |
|and illuminates the whole scene.                                              |
|==============================================================================|
|Extras                                                                        |
|The bounding box of the worm can be seen if the "B" key is pressed.           |
|The ground's texture can be changed by pressing the "G" key.                  | 
|==============================================================================|
|PS: You cannot kill yourself.                                                 |
|==============================================================================|
|==============================================================================|
|==============================================================================|