# PulsarEngine
Game Engine developed as a project for CITM-UPC. It has 3D rendering, engine and gameobjects configuration windows, 
resource managment system and a physics engine based on Bullet. 

* Latest release [here](https://github.com/Avilgor/PulsarEngine/releases)

## Done by

All of it was done by myself, you can find my Github here: [Avilgor](https://github.com/Avilgor)

![foto](https://github.com/Avilgor/PulsarEngine/blob/master/docs/Foto.jpg?raw=true)

## Features

* Inspector window with multiple interactable and editable options.
* Hierarchy window with scene objects.
* Menu bar with multiple options:
  * Create/delete gameobjects 
  * Render options
  * Edit
  * About 
  * Open editor windows
* Config panel to adjust window settings,see hardware specs, loaded resources and physics engine options.
* In editor log console.
* Project window with all items in assets.
* Camera frustum culling.
* Physics engine with colliders and constraints.
* Physic car sample.

The rendering is done using OpenGL with Glew library. The engine window is created with SDL and the whole interface depends on ImGui library. Going deeper in the engine there is
Assimp to load all FBX data files and Devil lib is used to load png textures. Physfs and JSON parser are the ones who manage all engine files. For all the maths I'm using MathGeoLib and for the Guizmo, the ImGuizmo library. Finally, the physics engine is using Bullet library.



## Physics engine
It has a total of three types of colliders (box, sphere and capsule) and 4 constraints (point, slider, hinge and cone). 
All of them are configurable from the editor. Bullet works woth rigidbodies in colliders, for this reason, each gameobject can only have one collider at a time and one type of each constraint. To made it to work with the gameobject movement, rigidbodies have to be updated before the world simulation steps, 
if not there will be weird physics behaviours. Looking in optimization, this update is only done if the gameobject transform has to be updated. After the simulation,
gameobjects positions are updated acording to the rigidbody.

![gif1](https://github.com/Avilgor/PulsarEngine/blob/master/docs/PhysicsEditor.gif?raw=true)

Colliders can be setted as static or dynamic and trigger or not. That will change how they behave with the world and it's collisions. They also have options to change it's size, and local offset. All this options are locked while the simulation is running to avoid bad behaviours.

![gif2](https://github.com/Avilgor/PulsarEngine/blob/master/docs/PhysicsWorking.gif?raw=true)

As seen above, there is also a car in the scene which can be controlled. Camera can't go inside physics gameobjects in runtime because it has a sphere collider.


## License

[GNU General Public License v3.0](https://github.com/Avilgor/PulsarEngine/blob/master/LICENSE)
