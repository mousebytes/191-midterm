=========================================
CSCI 191T - Advanced Game Dev - Midterm Exam
=========================================

-----------------------------------------
Overview
-----------------------------------------
This project is a 3D first-person shooter game created for the Midterm Exam. It fulfills all the requirements specified in the exam PDF, including a landing page, main menu, help screen, pause menu, and a complete gameplay scene.

-----------------------------------------
Summary of Custom Systems
-----------------------------------------
The provided engine was heavily extended with the following custom systems to create the game:

* **_Time.h / .cpp:**
    * Replaces the _timer.h
    * A static class that provides framerate independent deltaTime and totalTime using chrono::high_resolution_clock for smooth physics and animation.

* **_ObjModel.h / .cpp:**
    * A new, modern .obj model loader. This replaces the _3dmodelloader.h (MD2)
    * It parses .obj files, loading vertices, normals, texture coordinates, and faces.
    * It also normalizes model coordinates to fit a -1 to +1 space, making them easier to scale.

* **_StaticModel.h / .cpp & _StaticModelInstance.h / .cpp:**
    * Uses _ObjModel to load and render single mesh 3D models (like the terrain and bullet).
    * The Instance class allows for multiple copies of a model (e.g., many bullets) to exist in the world, each with its own position, scale, rotation, and collider set.

* **_AnimatedModel.h / .cpp & _AnimatedModelInstance.h / .cpp:**
    * A custom animation system that loads a sequence of .obj files (e.g., walk_00.obj, walk_01.obj...) as a single animation.
    * It performs vertex linear interpolation (lerp) between frames based on _Time::deltaTime to produce smooth animations.
    * The Instance class manages the current animation state (playing "idle", "walk", etc.), physics (gravity, velocity), and colliders.

* **_Collider.h (Abstract Base Class):**
    * A new abstract collider system to replace the _collisioncheck.h.
    * Defines a common interface for all collider types and includes a ColliderType enum (e.g., `COLLIDER_FLOOR`, `COLLIDER_WALL`, `COLLIDER_TARGET`) for physics interactions.

* **_CubeHitbox.h / .cpp & _SphereHitbox.h / .cpp:**
    * Concrete implementations of the _Collider class.
    * They support world space transformation and perform cubic and spherical collision checks.
    * They can be drawn in debug mode (toggled with '1' and '2' keys).

* **_Player.h / .cpp:**
    * A major class that encapsulates all player logic.
    * It holds an _AnimatedModelInstance for its body.
    * It handles all mouse and keyboard input for movement and looking.
    * It runs physics checks [UpdatePhysics()] against static colliders for wall sliding and floor detection.
    * It directly controls the _camera object to create a first person view (UpdateCamera).

* **_Bullets.h / .cpp:**
    * A bullet manager that handles the firing, updating, and drawing of all projectiles.
    * Bullets are [_StaticModelInstance]s.
    * It manages bullet lifetime and performs collision detection against registered "target" colliders.

* **_TargetManager.h / .cpp:**
    * Spawns and manages the falling targets (which are [_AnimatedModelInstance]s).
    * Targets are assigned COLLIDER_TARGET and fall with gravity.
    * The manager removes targets when they are hit by a bullet (isHit) or hit the floor (isGrounded).

* **_Button.h / .cpp:**
    * A new UI class for creating and drawing 2D menu buttons.
    * It handles texture loading and provides a simple isClicked(x, y) function for menu logic.

-----------------------------------------
Summary of Modified Core Files
-----------------------------------------
* **_scene.h / .cpp:**
    * The SceneState enum (LandingPage, MainMenu, Help, Playing, Paused) controls the entire application flow.
    * It initializes all custom systems (_Player, _TargetManager, [_Button]s, etc.).
    * It routes input, update, and draw calls to the correct functions based on the current m_sceneState.

* **_camera.h / .cpp:**
    * Modified to use _Time::deltaTime for smooth freecam movement.
    * In game, its position and rotation are completely controlled by the _Player::UpdateCamera function.

* **_common.h:**
    * Modified to include new, Vector3 and Vector2 structs with operator overloading, which are used by all new custom systems.
    * Includes _Time.h for global access to delta time.
    * Defines the global isDebug and colliderDrawFace flags.

-----------------------------------------
How Requirements Were Met
-----------------------------------------
1.  **Landing Page:** Implemented via SceneState::LandingPage in _scene.cpp.
2.  **Menu Page (Play, Help, Exit):** Implemented via SceneState::MainMenu, using the _Button class for all three options.
3.  **Menu Interaction:** handleMainMenuInput in _scene.cpp uses _Button::isClicked to transition states.
4.  **Help Page:** Implemented via SceneState::Help. VK_ESCAPE and a Back button return to the main menu.
5.  **Pause Menu:**
    * Pressing VK_ESCAPE in-game switches to SceneState::Paused and sets _Player::isFrozen = true.
    * A full pause menu is drawn with "Resume", "Help", and "Main Menu" buttons
    * Pressing VK_ESCAPE again or clicking "Resume" returns to SceneState::Playing.
6.  **Gameplay Features:**
    * **Camera as Eyes:** _Player::UpdateCamera sets the camera position to the player model's origin + an eyeHeight offset.
    * **Mouse/Key Rotate:** _Player::HandleMouse handles all camera/player rotation.
    * **AWSD Movement:** _Player::HandleKeys handles all player movement.
    * **Shoot Falling Objects:** The _TargetManager spawns falling _AnimatedModelInstance targets, and the _Bullets manager fires projectiles that collide with them.
    * **Objects Disappear on Collision:** _TargetManager removes targets when isHit (from _Bullets) or isGrounded (hit floor) is true.
    * **Walk Around Skybox:** The terrainInstance has COLLIDER_FLOOR and COLLIDER_WALL hitboxes that the _Player::UpdatePhysics function collides with, allowing free movement within the scene.
    * **Realism:** Achieved via _Time based physics, smooth interpolated animations (_AnimatedModel), a 3D gun model overlay, and a cohesive collision system.

-----------------------------------------
How To Play
-----------------------------------------
* **Movement:** W, A, S, D
* **Look:** Mouse
* **Fire:** Left-Click
* **Pause:** Escape
* **Debug Colliders:** '1' (toggle wireframe) and '2' (toggle solid faces)