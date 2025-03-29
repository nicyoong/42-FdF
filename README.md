# FDF - 42 Project

## Project Overview

FDF (Field of View) is a graphical project that allows you to visualize 3D models in 2D. The goal of this project is to render 3D objects using projection, focusing on creating a wireframe representation of a map. The main objective was to implement Bresenham's Line Algorithm to draw straight lines in a 2D plane, which was used to render the wireframe of the 3D object.

## What I Learned

### 1. **Understanding Bresenham's Line Algorithm**
   - One of the main concepts I had to learn for this project was the implementation of **Bresenham's Line Algorithm**. This algorithm is a fundamental method for drawing lines in computer graphics. It uses an efficient way of calculating the points of a line between two endpoints on a grid, without needing to perform floating-point arithmetic. 
   - I learned how to use integer-based calculations to determine the pixel positions along the line, which results in faster rendering, especially when dealing with large numbers of lines.

### 2. **2D to 3D Projection**
   - The project required me to implement a 3D to 2D projection, simulating a 3D object in a 2D space. I became familiar with the principles of geometric transformations, including translation, rotation, and scaling. These transformations allowed me to move, scale, and rotate the 3D map to fit into the 2D window properly.
   - I learned how to work with a basic 3D coordinate system and apply perspective transformations to produce a visually accurate 2D representation of the 3D model.

### 3. **Working with Arrays and Maps**
   - A significant part of the project involved representing the map in a structured way. I used **2D arrays** to store the height data and other relevant information of each point in the map. These arrays allowed for efficient access to the data while drawing the map.
   - By manipulating these arrays, I was able to simulate a top-down view of the 3D space and map each point to its corresponding location in the 2D window.

### 4. **Handling User Input and Window Management**
   - I worked with **minilibx** (a basic graphics library used in 42) to create the window and manage user input. This involved responding to keypresses and mouse events, allowing the user to rotate and zoom in/out of the 3D map, providing interactivity.
   - Managing the graphical output with real-time changes based on user input was a crucial skill I developed during this project.

### 5. **Optimizing for Performance**
   - As the complexity of the map increased, I had to optimize my code for performance. This included reducing unnecessary redraws and efficiently handling the calculations for line rendering.
   - The challenge of rendering hundreds or thousands of lines in real-time taught me how to balance between visual accuracy and performance.

### 6. **Debugging and Testing**
   - Debugging graphical programs was an important part of this project. I had to ensure that each line was being drawn correctly and that the map was accurately displayed. I spent time carefully checking the algorithm implementation and adjusting it to fix any graphical artifacts or errors.
   - Testing also involved validating the user interface and ensuring that interactive features like rotation and zooming worked as expected.

## Key Features
- **3D to 2D Projection**: Visualize a 3D map on a 2D plane.
- **Bresenham's Line Algorithm**: Efficiently render straight lines between points.
- **Interactive Controls**: Rotate and zoom the map with keyboard input.
- **Real-time Rendering**: Handle dynamic updates to the map as the user interacts.