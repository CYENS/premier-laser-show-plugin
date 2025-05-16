```bash
git submodule add https://github.com/CYENS/premier-laser-show-plugin.git Plugins/PremierLaserShow
```

# OSC Command Reference

A quick reference for Open Sound Control (OSC) messages driving the performance.

  * [Scene 1: Light Painting](#scene-1--light-painting)
    + [Phase 1: Setup](#phase-1--setup)
  * [Scene 2: Monster](#scene-2--monster)
    + [Phase 1: Monster Cue](#phase-1--monster-cue)
    + [Phase 2: Monster Appearance](#phase-2--monster-appearance)
  * [Scene 3: Light Solo](#scene-3--light-solo)
  * [Scene 4: Approximation](#scene-4--approximation)
  * [Scene 5: Light Forest](#scene-5--light-forest)
  * [Scene 6: The Mound](#scene-6--the-mound)
  * [Scene 7: Qualities](#scene-7--qualities)
  * [Scene 8: Puppet](#scene-8--puppet)
  * [Scene 9: Labyrinth](#scene-9--labyrinth)
  * [Scene 10: Laser Swarm](#scene-10--laser-swarm)

---

## Scene 1 Light Painting

Lights follow motion-capture joints, then enter wobbling behavior.

### Phase 1: Setup

- **Start Scene**
    ```bash
    /scene/activate 1 1
    ```
    
    A cue that signifies Scene 1, phase 1
    
- **Associate Lights to Joints**
    ```bash
    /light/joint/associate/position <j0> <j1> … <j15>
    ```
    
	Each light follows a joint or -1 for not following a joint.
	Send every frame.
	
    _Example 1:_ `/light/joint/associate/position 5 12 -1 22 7 0 19 8 14 1 17 2 11 6 20 4
    1st light follows joint 5, 2nd light  follows joint 12, 3rd light doesn't follow any joint ...
    
    _Example 2:_ `/light/joint/associate/position 5 -1 -1 -1 -1 ...
	Only first light follows joint.
    
- **Set Light Intensity**
    ```bash
    /light/intensity <i0> <i1> … <i15>
    ```
    light intensity every frame
    
    _Example:_ `/light/intensity 0.2 0.8 0.4 0.9 0.1 0.7 0.3 0.6 0.5 1.0 0.2 0.85 0.15 0.75 0.35 0.65`
    

- **Apply Light Offset**
    ```bash
    /light/offset <x0> <y0> <z0> … <x15> <y15> <z15>
    ```
	Apply an offset for each joint ( so if  light 0 looks at joint 0, then add x0, y0 ). 
	This is for the wobbling behavior where lights don't strictly follow the joints but get some autonomy. In the first phase offset is 0 for all lights.
	Send every frame.
	
    _Example:_ `/light/offset 0.05 -0.12 0.20 0.07 0.15 0.03 -0.22 -0.10 0.18 … 0.11 0.13 0.16`
    
- **End Scene**
    ```bash
    /scene/activate 1 0
    ```
    
    Cue for ending scene 1. Phase 0 means end of scene.

---

## Scene 2: Monster

Laser color syncs with lights; pan/tilt then laser lines draw.

### Phase 1: Monster Cue

- **Start Phase**
    
    ```bash
    /scene/activate 2 1
    ```
    
    Scene 2 Phase 1
    
- **Pan Lights**
    ```bash
    /light/pan <x0> <y0> <x1> <y1> … <x15> <y15>
    ```
    
    _Example:_ `/light/pan 15.0 3.0 2.0 65.0 ... 75.0 2.0`
    
- **Tilt Lights**
    ```bash
    /light/tilt <x0> <y0> <x1> <y1> … <x15> <y15>
    ```
    
    _Example:_ `/light/tilt 15.0 3.0 2.0 65.0 ... 75.0 2.0

### Phase 2: Monster Appearance

- **Show Appearance**
    ```bash
    /scene/activate 2 2
    ```
    
    Scene 2, phase 2. Monster appears. In this phase lines should appear with positions angles and lengths
    
- **Draw Laser Lines**
    ```bash
    /laser/line <x0> <y0> <a0> <l0> … <x15> <y15> <a15> <l15>
    ```
	
	Each 4 values describe the position of the light, it's angle, and it's length.
	
    _Example:_ `/laser/line 100.0 200.0 45.1 150.5 ....
    The first line is at 100.0 200.0 with angle of 45.1 and of length 150.5
- **End Scene**
    ```bash
    /scene/activate 2 0
    ```   

---

## Scene 3: Light Solo

DMX control only.

- **Start Solo**
    ```bash
    /scene/activate 3 1
    ```
    _Example:_ `/scene/activate 3 1`
- **End Solo**
    ```bash
    /scene/activate 3 0
    ```
   

---

## Scene 4: Approximation

Replays joint-driven intensity and pan/tilt data.

May just use DMX.

- **Start**
    ```bash
    /scene/activate 4 1
    ```    
    
- **Pan**
    ```bash
    /light/pan <x0> <y0> <x1> <y1> … <x15> <y15>
    ```
    
    _Example:_ `/light/pan 25 26 24 27 23 28 22 29 21 30 20 31 19 32 18 33`
    
- **Tilt**
    ```bash
    /light/tilt <x0> <y0> <x1> <y1> … <x15> <y15>
    ```
    
    _Example:_ `/light/tilt 5 6 4 7 3 8 2 9 1 10 0 11 12 13 14 15`
    
- **Intensity**
    ```bash
    /light/intensity <i0> … <i15>
    ```
    
    _Example:_ `/light/intensity 0.4 0.9 0.3 1.0 0.2 0.8 0.6 0.7 0.5 0.95 0.15 0.85 0.25 0.75 0.35 0.65`

---

## Scene 5: Light Forest

Rotation mapping per joint.

- **Start Forest**
    ```bash
    /scene/activate 5 1
    ```
    _Example:_ `/scene/activate 5 1`
    
- **Associate Rotation**
    ```bash
    /light/joint/associate/rotation <j0> … <j15>
    ```
    
    _Example:_ `/light/joint/associate/rotation 4 19 0 7 22 3 15 8 11 2 20 6 14 1 21 9`
    

---

## Scene 6: The Mound

Ellipsoids’ size and angle controlled by joint distances/velocities.

Or simulate the behavior for each ellipsoid: each size is control by the distance between pair of joints and the angle based on the velocity.

For this phase we might just use a cue for custom prescripted behavior.

- **Start Mound**
    ```bash
    /scene/activate 6 1
    ```
- **Horizontal Size**
    
    ```bash
    /ellipsoid/horizontal_size <s0> … <s5>
    ```
    
    _Example:_ `/ellipsoid/horizontal_size 1.0 1.2 0.9 1.1 1.3 0.8`
    
- **Vertical Size**
    
    ```bash
    /ellipsoid/vertical_size <s0> … <s5>
    ```
    
    _Example:_ `/ellipsoid/vertical_size 0.9 1.1 0.8 1.0 1.2 0.7`
    
- **Angle**
    
    ```bash
    /ellipsoid/angle <a0> … <a5>
    ```
    
    _Example:_ `/ellipsoid/angle 45.0 60.5 50.05 70.34 55.2 65.0`
    


---

## Scene 7: Qualities

No precise movement is required

Pan / Tilt / light intensity.

May go with DMX.

- **Start Qualities**
    ```bash
    /scene/activate 7 1
    ```
- **Pan**
    ```bash
    /light/pan <x0> <y0> <x1> <y1> … <x15> <y15>
    ```
    
    _Example:_ `/light/pan 25 26 24 27 23 28 22 29 21 30 20 31 19 32 18 33`
    
- **Tilt**
    ```bash
    /light/tilt <x0> <y0> <x1> <y1> … <x15> <y15>
    ```
    
    _Example:_ `/light/tilt 5 6 4 7 3 8 2 9 1 10 0 11 12 13 14 15`
    
- **Intensity**
    ```bash
    /light/intensity <i0> … <i15>
    ```
    
    _Example:_ `/light/intensity 0.4 0.9 0.3 1.0 0.2 0.8 0.6 0.7 0.5 0.95 0.15 0.85 0.25 0.75 0.35 0.65`
    
- **End Scene**
    
    ```bash
    /scene/activate 7 0
    ```
---
## Scene 8: Puppet

Follow recorded shoulder, elbow & hand joints.

- **Start Puppet**
    ```bash
    /scene/activate 8 1
    ```
    
    lights follow shoulder elbow and hand, also lights fade in.
    
- **Follow Recorder Motion**
    ```bash
    /scene/activate 8 2
    ```
    
    Motion capture no longer live, the lights still follow shoulder elbow and hand, but not of the live motion captured. 
    
    The lights instead follow shoulder elbow and hand of a prerecorded motion capture that is not shown.
    
    Here we can use what Ismail already made, in the end the lights should end in two parallel lines because the recording is the actor with their two hands extended in the front.
    
- **End Puppet**
    ```bash
    /scene/activate 8 0
    ```

---

## Scene 9: Labyrinth

Corridor fade, then full sequence, then spread out.

- **Corridor**
    ```bash
    /scene/activate 9 1
    ```
    
    lights fade in
    
- **Sequence**
    
    ```bash
    /scene/activate 9 2
    ```
    Already Implemented by Ismail.
- **End Scene**
    
    ```bash
    /scene/activate 9 0
    ```
    
    Spread out. Already implemented by Ismail
    

---

## Scene 10: Laser Swarm

Connected line swarm with dynamic length.

- **Start Swarm**
    ```bash
    /scene/activate 10 1
    ```

- **Draw Connected Lines**
    ```bash
    /laser/line <x0> <y0> <a0> <l0> … <x15> <y15> <a15> <l15>
    ```
- **End Swarm**
    ```bash
    /scene/activate 10 0
    ```
