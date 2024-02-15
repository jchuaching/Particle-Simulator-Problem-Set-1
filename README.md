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
git clone https://github.com/jchuaching/Particle-Simulator-Problem-Set-1
```

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
8. After the build is successful, make sure that the program is in Release Mode, and then go to the Debug menu.
   ![Step 8](https://github.com/jchuaching/Particle-Simulator-Problem-Set-1/assets/75210705/93937f2c-b137-45ef-a4d9-9a6e41013dee)
9. Click Start Without Debugging (or press Ctrl+F5) to run the project.
  ![Step 9](https://github.com/jchuaching/Particle-Simulator-Problem-Set-1/assets/75210705/3c467aea-7b4c-46ca-b915-a798bbf36e79)

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
![GUI Interface](https://github.com/jchuaching/Particle-Simulator-Problem-Set-1/assets/75210705/9ac02abe-96c9-4e89-9e30-cbd95961551a)

In order to **add a particle**, enter valid inputs for the x, y, angle, and velocity. <br>
X - represents the x coordinate of the particle (x >= 0 and x is within the width of the window 1280). <br>
Y - represents the y coordinate of the particle (y >= 0 and y is within the height of the window 720). <br>
Angle - represents the angle where the particle faces (angle >= 0).  <br>
Velocity - represents the velocity of the particle in pixel per second (velocity >= 0). <br>

In order to **add walls**, enter valid inputs for the x1, y1, x2, and y2. <br>
X1 - represents the x1 coordinate of the wall (x1 >= 0 and x1 is within the width of the window 1280). <br>
Y1 - represents the y1 coordinate of the wall (y1 >= 0 and y1 is within the height of the window 720). <br>
X2 - represents the x2 coordinate of the wall (x2 >= 0 and x2 is within the width of the window 1280). <br>
Y2 - represents the y2 coordinate of wall (y2 >= 0 and y2 is within the height of the window 720). <br>

Batches can be added when a radio button is selected.<be>
![Batch Form 1](https://github.com/jchuaching/Particle-Simulator-Problem-Set-1/assets/75210705/02010942-b724-4544-a1a2-99f1e8374f4a)

The photo above displays the first form that batches may be added in.

N - number of particles (N > 0). <br>
Start X - represents the x coordinate of the first particle (x >= 0 and x is within the width of the window 1280). <br>
Start Y - represents the y coordinate of the first particle (y >= 0 and y is within the height of the window 720). <br>
End X - represents the x coordinate of the last particle (x >= 0 and x is within the width of the window 1280). <br>
End Y - represents the y coordinate of the last particle (y >= 0 and y is within the height of the window 720). <br>
Angle - represents the angle where the particle faces (angle >= 0).  <br>
Velocity - represents the velocity of the particle in pixel per second (velocity >= 0). <br>

![Batch Form 2](https://github.com/jchuaching/Particle-Simulator-Problem-Set-1/assets/75210705/61bc8773-73c6-4b60-87d1-83c94d3aea63)

The photo above displays the second form that batches may be added in.

N - number of particles (N > 0). <br>
X - represents the x coordinate of the particles (x >= 0 and x is within the width of the window 1280). <br>
Y - represents the y coordinate of the particles (y >= 0 and y is within the height of the window 720). <br>
Start Angle - represents the angle where the first particle faces (angle >= 0).  <br>
End Angle - represents the angle where the last particle faces (angle >= 0).  <br>
Velocity - represents the velocity of the particle in pixel per second (velocity >= 0). <br>

![Batch Form 3](https://github.com/jchuaching/Particle-Simulator-Problem-Set-1/assets/75210705/950dfc95-9031-465f-9516-82b2ec6ca589)

The photo above displays the third form that batches may be added in.

N - number of particles (N > 0). <br>
X - represents the x coordinate of the particles (x >= 0 and x is within the width of the window 1280). <br>
Y - represents the y coordinate of the particles (y >= 0 and y is within the height of the window 720). <br>
Angle - represents the angle where the particle faces (angle >= 0).  <br>
Start Velocity - represents the velocity of the first particle in pixel per second (velocity >= 0). <br>
End Velocity - represents the velocity of the last particle in pixel per second (velocity >= 0). <br>

The inputs of the user should only be numbers.  <br>

Once you have entered valid inputs, **click the "Add" button** to add balls/particles or walls.
