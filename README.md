# PARTICLE SIMULATOR
Chua Ching, Dytoc
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
