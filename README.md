# 3D Scene Editor

## Operations

### 1.1 Shadow Mapping
1. Insert Mode
    - Press 'i' to enter insert mode
    - Load prepared object by pressing '1','2','3'

2. Move Mode
    - Press 'o' to enter move mode
    - Use 'w' 'a' 's' 'd' 'q' 'e' to Translate the object via xyz axis
    - Use 'f' 'g' 'h' 'j' 'k' 'l' to Rotate the object through xyz axis
    - Use ',' '.' to scale the object
   ![Insert and Move](image/A4-1-1.png)

3. Light Mode
   - Press 'y' to enter light mode
   - Press 's' to make the shadow red
   ![Red Shadow](image/A4-1-2.png)
   - Press 'a' and 'd' to move the light in a circle
   ![Move Light](image/A4-1-3.png)

### 1.2 Environment Mapping
   - In move mode, press 'v' to make the object mirror shading, press 'c' to resume phong shading
   - Skybox: completed
   ![Mirror Shading](image/A4-2-1.png)

### 1.3 Camera Control
1. Camera Mode
    - Press 'u' to enter camera mode
    - Use 'w' 's' 'a' 'd' 'q' 'e' to move the camera
        ![Camera control](image/A4-3-1.png)

### Optional
1. Refraction
   - In the move mode, press 'b' to make the object refraction shading
   ![Refraction Shading](image/A4-optional-1.png)
2. Dynamic environment mapping
   - In the move mode, press 'b' to make the object dynamic mirror shading
   ![Dynamic environment Shading](image/A4-optional-2.png)
   


```
Report ends here, texts below are report for Assignment3
```




### Open the application editor
![Initial](image/Initial.png)

### 1.1 Scene Editor
1. Insert Mode
    - Press 'i' to enter insert mode
    - Load prepared object by clicking
        - Draw cube
            ![Draw cube](image/1.1.1.png)
        - Draw bumpy cube
            ![Draw bumpy cube](image/1.1.2.png)
        - Draw bunny
            ![Draw bunny](image/1.1.3.png)

2. Delete Mode
    - Press 'p' to enter delete mode
        ![Delete Mode](image/1.1.4.png)

### 1.2 Object Control
1. Move Mode
    - Press 'o' to enter move mode
    - Use 'w' 'a' 's' 'd' 'q' 'e' to Translate the object via xyz axis
    - Use 'f' 'g' 'h' 'j' 'k' 'l' to Rotate the object through xyz axis
    - Use 'n' 'm' to scale the object
        ![Move mode](image/1.2.1.png)


2. Shading Model
    - In the move mode, we can change shading model and color of the object
    - Use '1' to '9' to change the object color
    - Use 'z' 'x' 'c' to change the object to wireframe, flat, phong, shading model
        ![Shading and color](image/1.2.2.png)

### 1.3 Camera Control
1. Camera Mode
    - Press 'u' to enter camera mode
    - Use 'w' 's' 'a' 'd' 'q' 'e' to translate the camera
        ![Camera control](image/1.3.1.png)

2. Projection
    - Press 'n' and 'm' to swithc between orthographics projection and perspective projection in camera mode
        ![Orthographics projection](image/1.3.1.png)
        ![Perspective projection](image/1.3.2.png)


### 1.5 Trackball
1. Trackball
    - Use 'z' 'x' to swtich the camera between trackball mode and normal mode

### 1.6 Blobby Man
See blobby_man branch