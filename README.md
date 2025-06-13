# SpectrumRetreat

<h2>Description</h2>
This is a simple project using a custom engine made with Modern OpenGL and C++. This simple game is similar to The Spectrum Retreat released in 2018 where you have a "phone" on your bottom left screen that you can use to swap colors with interactables objects to pass through them. 
<br /> 


<h2>Languages and Utilities Used</h2>

- <b>C++</b>
- <b>Custom Math Library</b> 

<h2>Engine Used </h2>

- <b>Simple Graphic Custom Engine made with Modern OpenGL</b>

<h2>Features and mechanics that I worked on : </h2>

- <b><h3>Custom Graphics Engine:</h3></b> Built with Modern OpenGL for rendering.

- <b><h3>Player Interaction:</h3></b> Implemented player movement, input handling, and camera control.

- <b><h3>Physics System:</h3></b>
- Collision Detection: Utilized Collider objects (Box, Sphere, Capsule) for broad-phase and narrow-phase collision checks.
- Raycasting: Implemented raycasting functionality to detect interactions with objects.
- Gravity: Applied gravity to the player, including jump mechanics and fall speed limits.

- <b><h3>Game Object Management:</h3></b>
- Dynamic Object Creation: Created various GameObject types (Doors, Buttons, Moving Objects, Obstacles, End Points, Death Zones) from mesh data.
- Color-Swapping Mechanic: Implemented the core "phone" feature to swap ColorState with interactable objects (Doors, Buttons, Moving Objects).
- Animated Objects: Handled MOVING_OBJECT interpolation for their transformations.

- <b><h3>Resource Management:</h3></b> Implemented a ResourceManager to handle loading and managing textures, shaders, and meshes.
- <b><h3>Lighting System:</h3></b> Incorporated directional, point, and spot lights into the scene.
- <b><h3>User Interface (UI):</h3></b> Developed UI elements such as a phone display, a cursor, and an in-game menu including restart functionality.
- <b><h3>Audio System:</h3></b> Managed sound playback and stopping using the Audio_Manager (singleton pattern).
- <b><h3>Game Flow:</h3></b> Included game state management for winning and resetting the game.

<h2>Gameplay</h2>


