# PulsarEngine
Game Engine developed as a project for CITM-UPC. It has 3D rendering, engine and gameobjects configuration windows, 
resource managment system and a physics engine based on Bullet. 

* Latest release [here](https://github.com/Avilgor/PulsarEngine/releases)

## Done by

All of it was done by myself, you can find my Github here: [Avilgor](https://github.com/Avilgor)

![](https://github.com/Avilgor/PulsarEngine/blob/master/docs/Foto.jpg)

## Features

* Inspector window with multiple interactable and editable options.
* Hierarchy window with sacene objects.
* Menu bar with multiple options
  * Create/delete gameobjects 
  * Render options
  * Edit
  * About 
  * Open editor windows
* Config panel to adjust window settings,see hardware specs and loaded resources
* In editor log console
* Project window with all items in assets
* Camera frustum culling
* Physics engine with colliders and constraints
* Physic car sample



## Physics engine
Physics were made using Bullet. It has a total of three types of colliders (box, sphere and capsule) and 4 constraints (point, slider, hinge and cone). 
All of them are configurable from the editor. Bullet works woth rigidbodies in colliders, for this reason, easch gameobject can only have one collider at a time and one type of each constraint. To made it to work with the gameobject movement, rigidbodies have to be updated before the world simulation steps, 
if not there will be weird physics behaviours. Looking in optimization, this update is only done if the gameobject transform has to be updated. After the simulation,
gameobjects positions are updated acording to the rigidbody.

![](https://github.com/Avilgor/PulsarEngine/blob/master/docs/PhysicsEditor.gif)

Colliders can be setted as static or dynamic and trigger or not. That will change how they behave with the world and it's collisions.


## License

[GNU General Public License v3.0](https://github.com/Avilgor/PulsarEngine/blob/master/LICENSE)







Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

