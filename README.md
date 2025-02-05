# 2D Graphics Renderer with OpenGL & FPS Logger

## Overview

This project is a simple 2D graphics renderer implemented in C++ using OpenGL. It demonstrates fundamental OpenGL concepts such as window creation, rendering 2D shapes (rectangles and circles), shader programming, transformations, and performance measurement (FPS).  The project also includes a Python script to analyze and visualize the logged FPS data.

## Features

- **OpenGL Rendering:** Renders basic 2D shapes:
    - Rectangle (using two triangles)
    - Circle (approximated using a triangle fan)
- **Keyboard-Controlled Transformations:** Allows real-time manipulation of rendered shapes using keyboard input:
    - Translation (arrow keys)
    - Rotation (Q/E keys)
    - Scaling (Z/X keys)
- **Shader Programming:** Utilizes simple Vertex and Fragment shaders for rendering.
- **Transformation Matrices:** Implements model-view-projection transformations using matrix operations.
- **FPS Calculation and Logging:** Measures frames per second (FPS) and logs the data to a CSV file (`fps_log.csv`) with timestamps.
- **FPS Analysis (Python):** Provides a Python script (`fps_analyzer.py`) to:
    - Read and parse the FPS log data.
    - Calculate average, minimum, and maximum FPS.
    - Generate a plot of FPS over time (`fps_plot.png`).

## Concepts Demonstrated

- **OpenGL Fundamentals:**
    - Window and context creation using GLFW.
    - Modern OpenGL function loading using GLEW.
    - Vertex Buffer Objects (VBOs) and Vertex Array Objects (VAOs).
    - Vertex and Fragment Shaders.
    - Uniforms for passing data to shaders.
    - Drawing primitives (triangles, triangle fans).
    - Transformation matrices (model, view, projection).
- **C++ Programming:**
    - Basic C++ project structure.
    - Use of external libraries (GLFW, GLEW, GLM).
    - Shader compilation and program linking.
    - Input handling with GLFW callbacks.
    - Time measurement for FPS calculation.
    - File I/O for FPS logging.
- **Python Scripting:**
    - CSV file reading and parsing.
    - Basic statistical calculations (average, min, max).
    - Data visualization using matplotlib.

## Getting Started

### Prerequisites

- **C++ Dependencies:**
    - **GLFW:**  For window creation and input handling. Install via Homebrew on macOS: `brew install glfw`
    - **GLEW:** For OpenGL extension loading. Install via Homebrew on macOS: `brew install glew`
    - **GLM (optional):** For matrix operations. Included directly in the code, no separate installation needed.
- **Python Dependencies:**
    - **Python 3.x:**  Make sure Python 3 is installed on your system.
    - **matplotlib:** For plotting. Install via pip: `pip3 install matplotlib`

### Compilation (C++)

1.  Navigate to the project directory in your terminal:
    ```bash
    cd /Users/masoom/Developer/open gl/2D-Graphics-Renderer-OpenGL
    ```
2.  Compile the `main.cpp` file using g++:
    ```bash
    g++ main.cpp -o main -lglfw -I/opt/homebrew/include -L/opt/homebrew/lib -lGLEW -framework OpenGL
    ```
    - **`-o main`**:  Specifies the output executable file name as `main`.
    - **`-lglfw`**:  Links the GLFW library.
    - **`-I/opt/homebrew/include`**:  Specifies the include directory for GLFW and GLEW (adjust if you installed them in a different location).
    - **`-L/opt/homebrew/lib`**:  Specifies the library directory for GLFW and GLEW (adjust if you installed them in a different location).
    - **`-lGLEW`**:  Links the GLEW library.
    - **`-framework OpenGL`**: Links the OpenGL framework (macOS specific). For other platforms (like Linux or Windows), you might need to adjust this flag (e.g., `-lGL`).

### Execution (C++)

1.  Run the compiled executable:
    ```bash
    ./main
    ```
    - This will open an OpenGL window displaying a rotating, scaling, and translating rectangle and circle. The window title will show the real-time FPS.
    - FPS data will be logged to `fps_log.csv` in the project directory.
    - To close the application, press ESC or close the window.

### Execution (Python FPS Analyzer)

1.  Ensure that you have run the C++ renderer for some time to generate `fps_log.csv`.
2.  Run the Python script:
    ```bash
    python3 fps_analyzer.py
    ```
    - This script will:
        - Print FPS statistics (Average, Min, Max) to the console.
        - Save an FPS plot as `fps_plot.png` in the project directory.

### Verification

1.  **Run C++ Renderer:** Compile and run `./main`. Verify that an OpenGL window appears with animated shapes and an updating FPS counter in the title.
2.  **Check `fps_log.csv`:** After running the renderer, check for the creation of `fps_log.csv` and verify it contains timestamped FPS data.
3.  **Run Python Analyzer:** Execute `python3 fps_analyzer.py`. Verify that FPS statistics are printed in the console and `fps_plot.png` is created.
4.  **Open `fps_plot.png`:** Open the image file and verify that it displays a plot of FPS over time.

## Project Structure

```
2D-Graphics-Renderer-OpenGL/
├── README.md             // This README file
├── main.cpp              // C++ source code for OpenGL renderer
├── shader.vs             // Vertex shader source code
├── shader.fs             // Fragment shader source code
├── fps_log.csv           // CSV file to store FPS logs (generated after running ./main)
├── fps_analyzer.py       // Python script to analyze FPS logs
└── fps_plot.png          // PNG image of FPS plot (generated after running python3 fps_analyzer.py)
```

## Keyboard Controls

When the OpenGL renderer window is active:

- **Arrow Keys (Left, Right, Up, Down):** Translate the shapes along the X and Y axes.
- **Q/E Keys:** Rotate the shapes counter-clockwise (Q) and clockwise (E).
- **Z/X Keys:** Scale the shapes down (Z) and up (X).
- **ESC Key:** Close the application.

## Code Explanation (Brief)

- **C++ (main.cpp):**
    - Initializes GLFW and GLEW.
    - Creates an OpenGL window and sets up the viewport.
    - Compiles and links vertex and fragment shaders from `shader.vs` and `shader.fs`.
    - Defines vertex data for a rectangle and a circle.
    - Creates VBOs and VAOs to store and manage vertex data.
    - Implements the render loop:
        - Clears the screen.
        - Updates the transformation matrix based on keyboard input.
        - Sets the transformation matrix and color as uniforms in the shader.
        - Draws the rectangle and circle.
        - Calculates and logs FPS.
        - Updates the window title with FPS.
        - Polls and processes events (keyboard input, window close).
        - Swaps buffers to display the rendered frame.
    - Cleans up resources (shaders, buffers, window) upon exit.

- **GLSL (shader.vs, shader.fs):**
    - **Vertex Shader (`shader.vs`):**
        - Takes vertex positions as input.
        - Applies the transformation matrix (uniform `transform`) to the vertex positions.
        - Outputs the transformed vertex positions as `gl_Position`.
    - **Fragment Shader (`shader.fs`):**
        - Takes a uniform color (`ourColor`) as input.
        - Outputs the uniform color for each fragment.

- **Python (fps_analyzer.py):**
    - Reads `fps_log.csv` using the `csv` module.
    - Calculates average, minimum, and maximum FPS values.
    - Uses `matplotlib.pyplot` to generate a line plot of FPS over time and saves it as `fps_plot.png`.

## Potential Improvements

- **More Shapes:** Add more 2D shapes (triangles, ellipses, polygons, etc.).
- **Color Control:** Implement controls to change the color of the shapes.
- **Texture Mapping:** Add texture mapping to the shapes.
- **More Transformations:** Implement more complex transformations (shearing, reflection).
- **UI Elements:** Add basic UI elements (using a UI library) to control rendering parameters.
- **Real-time Plotting:** Update the FPS plot in real-time as the renderer runs.
- **Error Handling:** Implement more robust error handling in both C++ and Python code.

This project provides a basic yet functional foundation for understanding 2D graphics rendering with OpenGL and performance measurement. It can be further expanded and improved to explore more advanced graphics concepts.
