# PARTICLE SIMULATOR

## Getting Started  with the Particle Simulator C++ Project:
This guide will walk you through the process of downloading, setting up, and running the project using Microsoft Visual Studio on a Windows machine.

## Prerequisites
Make sure that you have the following installed on your system:

- Git: To clone the project repository. 
- Microsoft Visual Studio: With C++ development tools. During installation, make sure to install the "Desktop development with C++" workload.

## Step 1: Clone the Git Repository
1. Open a command prompt or PowerShell window.
2. Navigate to the directory where you want to store the project.
3. Clone the repository using the following command:
```
git clone https://github.com/your-username/bouncyball.git
```
Alternatively, you could also do the following:
1. Open Visual Studio.
2. Select Clone or check out code from the opening screen.
3. Enter the URL of the GitHub repository in the repository location box.
4. Click Clone.

## Step 2: Open the Project in Visual Studio
If you cloned the repository from within Visual Studio, it should be ready to open. 
Otherwise:

1. Launch Microsoft Visual Studio.
2. Select 'Open a project or solution' from the 'Get started' section.
3. Navigate to the directory where you cloned the repository.
4. Find and select the bouncyball.sln file.
5. Click 'Open' to load the project in Visual Studio.

## Step 3: Set Up the Project
1. Wait for Visual Studio to finish loading the project and any dependencies. This may take a moment.
2. Check if Visual Studio requires any additional components and install them if prompted.

## Step 4: Configure the Project Settings
1. Right-click on the project name in the 'Solution Explorer' pane.
2. Select 'Properties' from the context menu.
3. Navigate to 'Configuration Properties' > 'General'.
4. Set the 'Output Directory' and 'Intermediate Directory' to the desired paths if necessary.
5. Go to 'C/C++' > 'General', and ensure that the 'Additional Include Directories' field includes the path to the 'include' directory if your project requires it.
6. Check 'Linker' > 'General' and 'Input' for any additional library dependencies required by your project.

## Step 5: Build the Project
1. Press 'Ctrl+Shift+B' to build the solution, or go to the 'Build' menu and select 'Build Solution'.
2. Review the 'Output' pane at the bottom of the Visual Studio window to ensure there are no compile errors.

## Step 6: Run the Program
1. Press 'F5' to start debugging and run the application, or go to the 'Debug' menu and select 'Start Debugging'.
2. Look at the 'Output' pane or the new window that opens to see the running application.
