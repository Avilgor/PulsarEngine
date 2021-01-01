# PulsarEngine
Game Engine developed as a project for CITM-UPC. It has 3D rendering, engine and objects configuration windows, 
resource managment system and a physics engine based on Bullet. 

[Pulsar engine](https://github.com/Avilgor/PulsarEngine)

Latest release [here](https://github.com/Avilgor/PulsarEngine/releases)

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

## Controls

### Selection
* While pressing control you can select multiple objects in hierarchy
* Delete key deletes all selected gameobjects

### Camera
* Mouse wheel: Zoom
* RightClick: Rotate
* Left shift: Fast movement
* Mouse wheel button: Move
* F: Focus the current or first selected gameobject
* Space: Throw physic ball in looking direction (only if scene is running)

### Project
* Pressing delete key deletes selected asset
* Drag and drop on scene to load selected asset

### Hierarchy
* Drag gameobjects to reparent
* Pressing delete key deletes selected gameobjects
* In Edit you can unparent selected gameobjects

### Scene
* W: Toggle gizmo translation mode
* E: Toggle gizmo rotation mode
* R: Toggle gizmo scale mode
* Left click: Select hovered gameobject in scene

### Vehicle
* Arrow keys: Car movement (Forward, left, right and backwards)
* X: Card Brake

## Tips: 
* By default camera rotation orbits the current point, so it's not needed to press alt key continously.
* Physic simulation only working when scene is running or toggled in the physics options window

## Done by
* Adrià Avila: [Avilgor](https://github.com/Avilgor)

