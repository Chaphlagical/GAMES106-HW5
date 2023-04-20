# GAMES106-HW5
GAMES 106 Homework5: Mesh Reduction

## Build

Run following commands:

```shell
mkdir build
cd build
cmake ..
cmake --build ./
```

Run `GAMES106-HW5.exe` in Debug/Release folder and load a scene, you may see:

![image-20230411213906286](img/screenshot.png)

## Viewer Usage

```
[drag]  Rotate scene
A,a     Toggle animation (tight draw loop)
D,d     Toggle double sided lighting
F,f     Toggle face based
I,i     Toggle invert normals
L,l     Toggle wireframe
O,o     Toggle orthographic/perspective projection
T,t     Toggle filled faces
Z       Snap to canonical view
[,]     Toggle between rotation control types (trackball, two-axis valuator 		with fixed up, 2D mode with no rotation))
<,>     Toggle between models
;       Toggle vertex labels
:       Toggle face labels
```

