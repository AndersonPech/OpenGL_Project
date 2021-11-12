# Assignment 2 Justification

If you think any of the questions within the sections are not applicable, please write "N/A".

## Section 1: Creation and Use of 3D Objects

### 1a: Show that you have created or imported at least one 3D object that is visible in your application. Include references to where this is implemented in your code.

I created a volume mesh and imported 2 objects (the evangelion and the angel(pyramid)). In scene.cpp I called the functions to create the meshes and to upload the object. From line 72 in the same file i created another cube but this time creating the positions in an array. In model line 79 was where the model was uploaded. 

### 1b: Describe your scene graph and the hierarchy within the scene. Include references to where this is implemented in your code.

In scene.cpp there is three levels of hierarchy. The ground, the angel and the cubes. This is in the make_scene function in scene.cpp from line 20. In scene.hpp line 26 i added a new parameter (in hindsight i should've refactor it into a class) to hold the cubes since the cubes must also hold its positions. 

### 1c: Describe how your objects are moving or animating in realtime. Include code references (line numbers and function names) to use of delta time and also how this affects the scene graph.

There are two different points where there is animation. In animator.cpp from line 100 animates the angel and the cubes. The angel will bop up and down. The cubes are animated in aniamteCircle using a spline and updates the light position in line 43. 

### 1d: Describe how you have used parametric splines in your application. Include code references to the implementation of control point based curves in your project.

animateCircle function in animate.cpp at line 35 to 41 uses paremetric curves to animate the cubes spinning in an oval like direction. The curves are determined by the time and positions are updated in the struct in model.hpp one line 38.

## Section 2: Camera

### 2a: Describe what controls the player should use to move the camera in your scene. Include code references to the implementation of delta time control of speed and use of user input in your project.

In camera.cpp the camera uses delta time to turn left right and directions. Update_cam_pos is where this is done. Press F to change to ground camera, P to go to cutscene camera and Q to exit either camera modes.

### 2b: Describe how your camera interacts with the scene. Is it able to "walk" on the Z-X plane or follow terrain? Include code references.

A boolean called fps on line 12 in camera.cpp determines whether or not to lock the camera. 

### 2c: Describe how the view of your scene can change from player controlled to application controlled (specify user input that makes this change happen). Include code references to use of parametric curves and delta time in the application controlled camera.

At P key, the camera turns into a cutscene. The aniamte function on line 27 holds the values for the parametric curve. The curve will repeat after finishing its turn.

## Section 3: Lighting

### 3a: Describe where Diffuse Lighting is clear to see in your application. Include code references to your implemented lighting (including where this is implemented in your shaders).

In shaders.frag in the calcdirlight and calcpointlight functions diffuse lighting was used for both of the lights on line 50 for direction and line 74 for the point light. The cubes do not have any lighting on them since they are meant to emit lights. Hence they are stored in shaderscube.frag who have a single colour (red).

### 3b: Describe where Specular Lighting is clear to see in your application, including how the camera position affects the lighting. Include code references to your implemented lighting (including where this is implemented in your shaders).

Specular Lighting is done on the floor and the container. It should be apparent went approaching close to the light. The light should also be reflected off the angel (although the behaviour is random due to the polygons of it). In shaders.frag this is done in calcpointlight where specular lighting is calculated on line 80. 

### 3c: Describe which of your light(s) in your scene are point light(s) and how you have placed objects in the scene to clearly show the point lights functioning. Include code references to point light implementation.

In shaders.frag there is an array that holds a pointlight object. There are 30 point lights. This creates a loop of red light on the floor in the application. The point light should also reflect of the angel. The point lights are added to the color in the main function in shaders.frag as well as the directional light.

### 3d: Describe all the lights in your scene and what type they are. Include code references to implementation of the capability to work with an arbitrary number of lights.

Two sets of lights. direct which lights the scene and point which is red. In shaders.frag the array for pointlights holds multiple position of point lights. There are 30 in this case. 

### 3e: Explain what maps you have used and which objects they are on. Your application should clearly show the effect these maps are having on lighting. Include code references to maps on objects and vertices as well as lighting code that uses the maps.

There is one specular map involved. This can be seen on the floor where it is red (warning for flashing lights). It's more obvious on the container where the red light touches. The texture is called pat.jpg and is applied in default.frag. In calcpointlight in the specular section. The object is loaded in renderer.cpp in make renderer in a GLuint called specular on line 31. 

## Section 4: Subjective Mark

### How have you fulfilled this requirement?

(Attempted to) recreate a scene frome neon genesis evangelion in which one of the evas attacks the angel (the pyramid). The cubes spinning around it are reflective of the scene. 

### What is your vision for the scene you are showing?

https://www.youtube.com/watch?v=rnMJRMEq5s4 at 1:14 i attempted to create this spinning cubes. 

### Are you referencing anything in this scene? Is this a quote or homage to any other art? Please describe it.

Homage to neon genesis evangelion.

### Please give instructions for how a viewer/player should interact with this scene to see all the content you have created.

Fly around and use cutscene camera. 

## Section 5 (optional): Code Style Feedback

### There are no marks assigned to code style in this assignment. However, if you would like to receive some feedback on your coding style, please point out a section of your code (up to 100 lines) and your tutor will give you some written feedback on the structure and readability of that code.

Write your answer here (lines numbers or function names that you'd like reviewed)

## Section 6 (optional): Use of External Libraries

### What does the library do?

Write your answer here...

### Why did you decide to use it?

Write your answer here...

### How does this integrate with the assignment project build system? Please include any instructions and requirements if a marker needs to integrate an external library.

Write your answer here...
