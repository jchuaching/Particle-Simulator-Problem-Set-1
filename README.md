# PARTICLE SIMULATOR
Chua Ching, Dytoc <br>
Problem Set #1 STDISCM

---

## Getting Started  with the Particle Simulator C++ Project:
This guide will walk you through the process of downloading, setting up, and running the project using Microsoft Visual Studio on a Windows machine.

## Prerequisites
Make sure that you have the following installed on your system:

- Git: To clone the project repository. 
- Microsoft Visual Studio: With C++ development tools. During installation, make sure to install the "Desktop development with C++" workload.

## Step 1: Clone the Repository
1. Open Visual Studio.
2. Select Clone or check out code from the opening screen.
3. Enter the URL of the GitHub repository in the repository location box.
4. Click Clone.
  
Alternatively, you can clone the repository using the command line:
1. Open a command prompt or Git Bash.
2. Navigate to the directory where you want to clone the project, e.g., cd Documents.
3. Use the command
```
git clone https://github.com/your-username/bouncyball.git
```
, replacing 
```
https://github.com/your-username/bouncyball.git
```
with the actual URL of the repository.

## Step 2: Open the Project
If you cloned the repository from within Visual Studio, it should be ready to open. Otherwise:

1. Launch Visual Studio.
2. Go to File > Open > Project/Solution or click Open a project or solution on the start page.
3. Navigate to the directory where you cloned the repository.
4. Select the bouncyball.sln file and click Open.

## Step 3: Build the Solution
5. Once the project is open, go to the Build menu at the top.
6. Select Build Solution (or press Ctrl+Shift+B).
7. Visual Studio will now compile the project. Make sure there are no errors in the Output window at the bottom.

## Step 4: Run the Application
8. After the build is successful, go to the Debug menu.
9. Click Start Debugging (or press F5) to run the project.

If everything is set up correctly, your application should now start.

## Additional Configuration (If Needed)
If you encounter any issues related to missing dependencies or configuration, you may need to adjust the project settings:
1. Right-click on the project in the Solution Explorer.
2. Choose Properties.
3. Under C/C++, ensure that all include directories are correctly referenced.
4. Under Linker, ensure that additional library directories and dependencies are correctly referenced.

---

## How to use the Particle Simulator C++ Program:
The program should like this when it is running:
[Screenshot 2024-02-12 191838](https://github.com/jchuaching/bouncyball/assets/75210705/219ff33e-2814-4355-a770-314bc148f7a3)

In order to **add balls**, enter valid inputs for the x, y, angle, and velocity. <br>
x - represents the x coordinate of the ball/particle (x >= 0 and x is within the width of the window). <br>
y - represents the y coordinate of the ball/particle (y >= 0 and y is within the height of the window). <br>
angle - represents the angle where the ball faces (angle >= 0).  <br>
velocity - represents the velocity of the ball in pixel per second (velocity >= 9). <br>!

In order to **add walls**, enter valid inputs for the x1, y1, x2, and y2. <br>
x1 - represents the x1 coordinate of the wall (x1 >= 0 and x1 is within the width of the window). <br>
y1 - represents the y1 coordinate of the wall (y1 >= 0 and y1 is within the height of the window). <br>
x2 - represents the x2 coordinate of the wall (x2 >= 0 and x2 is within the width of the window). <br>
y2 - represents the y2 coordinate of wall (y2 >= 0 and y2 is within the height of the window). <br>

N - number of particles (N > 0). <br>

The inputs of the user should only be numbers.  <br>

Once you have entered valid inputs, click the add button to add balls/particles or walls.
