|==============================================================================|
|==============================================================================|
|==============================================================================|
|                            "Avion cu Motor"                                  |
|==============================================================================|
|                           Smeu Adina - 335CA                                 |
|==============================================================================|
|An endless runner with an airplane that moves from the left to the right, has |
|three lives and has to avoid obstacles and collect fuel items.                |
|==============================================================================|
|The plane                                                                     |
|                                                                              |
|The plane consists of the following meshes:                                   |
|- the body:                                                                   |
|  - its center is considered the centre of gravity of the plane;              |
|- the wings;                                                                  |
|- the tail and its components;                                                |
|- the cabin:                                                                  |
|  - a transparent mesh;                                                       |
|  - contains the pilot;                                                       |
|- the wheels;                                                                 |
|- the propeller:                                                              |
|  - it rotates cotinuously on the ox axis;                                    |
|  - it contains a spotlight.                                                  |
|                                                                              |
|The plane's OY coordinates are modified either by the mouse's movement or by  |
|the gravitational acceleration when the plane crashes. The plane also rotates |
|around its gravitational centre with an angle that is influenced by the mouse.|
|The plane's OX coordinates are influenced by the gravitational acceleration   |
|when the plane crashes.                                                       |
|==============================================================================|
|The obstacles                                                                 |
|                                                                              |
|An obstacle is a sphere mesh that has a pinkish texture and has the following |
|attributes:                                                                   |
|- a random scale;                                                             |
|- random OX and OY radiuses that describe the elliptic movement of the        |
|  obstacle around the origin;                                                 |
|- the angle it makes with the OX axis.                                        |
|                                                                              |
|The obstacles have an elliptic movement and each time the plane collides with |
|one, he loses a life.                                                         |
|==============================================================================|
|The fuel items                                                                |
|                                                                              |
|A fuel item is a sphere mesh that has a fuel texture and has the following    |
|attributes:                                                                   |
|- a random scale;                                                             |
|- random OX and OY radiuses that describe the elliptic movement of the        |
|  item around the origin;                                                     |
|- the angle it makes with the OX axis;                                        |
|- the number of spheres from the cluster that have to be rendered.            |
|                                                                              |
|The fuel items consist of clusters of 5 spheres and have an elliptic movement.|
|Each time the plane collides with one, he receives fuel points.               |
|==============================================================================|
|The clouds                                                                    |
|                                                                              |
|A cloud consist of 5 to 9 transparent boxes or spheres with random scales and |
|have an elliptic movement. They are deformed in the vertex shader.            |
|==============================================================================|
|The sea                                                                       |
|                                                                              |
|The sea consists of a cylinder that is deformed in the fragment shader in     |
|order to simulate waves.                                                      |
|==============================================================================|
|The user interface                                                            |
|                                                                              |
|The fuel amount and the lives are displayed in the left corner of the screen. |
|==============================================================================|
|The logic                                                                     |
|                                                                              |
|The plane moves on the OY axis and rotates around its OZ axes based on the    |
|movement of the mouse. The clouds, obstacles and fuel items rotate around the |
|origin on an elliptic curve and rotate around their OY axis.                  |
|The sea consists of a cylinder that rotates and is deformed in order to       |
|simulate waves.                                                               |
|The whole scene is illuminated by the sun, while the plane also contains a    |
|spotlight in its propeller.                                                   |
|When the plane collides with an obstacle, its lives decrease by one, whereas  |
|when it collides with a fuel item, its fuel amount is increased by 10 units.  |
|When the player has either no more lives or no more fuel, the plane crashes   |
|in the sea.                                                                   |
|The bounding box of the plane can be seen if the "B" key is pressed.          |
|The camera type can be changed using the "C" key.                             | 
|==============================================================================|
|PS: No transformations depending on deltaTimeSeconds are applied on the scene |
|components on the first frame.                                                |
|==============================================================================|
|==============================================================================|
|==============================================================================|