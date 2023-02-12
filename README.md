# FTXUI_PV

- documentation of the code is made inline with comments
- to start the game run the `arkanoid-ftxui` cmake configuration

## How to setup development environment (in Visual Studio Code): 

- install the latest version of [vs code](https://code.visualstudio.com/download) 
- install the latest version of [docker](https://docs.docker.com/get-docker/) (desktop version is recommended for beginners)
- for docker beginners: pull the already built image (arm/amd64) from my [docker hub](https://hub.docker.com/repository/docker/megaminxcoding/cpp-image) with the command `docker pull megaminxcoding/cpp-image` and start it from the **docker desktop application** (starting from cli is troublesome)
- for docker pros: build the Docker image yourself -> [.devcontainer/Dockerfile](.devcontainer/Dockerfile)

----- (assuming you image is running successful now) -----
- in vs code install the extension [Remote - Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) (reload vs code if required)
- in the bottom left is now a blue status symbol visible which indicates the state of the container connection (looking like this '> <')
- click it and choose `Attach to Running Container` -> select the cpp-image (your vs code instance is now virtually inside the docker container)
- once connected (status symbol bottom left changes) install **INSIDE THE CONTAINER** the vs code extension [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)
- after installation there are going to appear new buttons on the bottom status bar to manage/debug/run your cmake projects as soon as you open a folder with a `CMakeList.txt` (btw: you can select folders in the container by simply using the built-in "Open Folder" functionality in vs code)
- **Final sanity check**: if your bottom bar looks sth. like this in an opened CMake project - youre fine
<img width="933" alt="image" src="https://media.github.fh-zwickau.de/user/189/files/58c70970-baf3-43f9-9685-4b3776b81cd4">

  

## The game itself

![image](https://media.github.fh-zwickau.de/user/189/files/d6144600-6a5f-446b-9e5a-8f988521b509)




## Known Issues:
> sometimes the ball disappears to the right (unexplainable bug) - to solve this, reduce the ball_direction_delta (velocity) --> recommended 1 to 1.5f

---
Questions to [Felix Schober](mailto@felix.schober.jc3@fh-zwickau.de).

