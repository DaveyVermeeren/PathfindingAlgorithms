# PathfindingAlgorithms
 This project is intended for me to explore a number of pathfinding algorithms within a variety of maps, either randomly generated or loaded in, with visual presentation.
 
 # Loading in maps
 Step 1. Use OgmoEditor to create a map on a Grid_layer stored as a 1D array  
 Step 1.1 The listed values represent the following:
 * 0 - Floor
 * 1 - Wall
 * 2 - Target
 * 3 - Start
<a/>
 Step 2. Copy the saved .json to PathfindingAlgorithms/Resource<br/>
 Step 3. Insert the filename when running the project, directory and filetype are added automatically
 
 # Used third-party tools
 While most of the actual project is written in C++, it makes use of some libraries and tools:
 * [Ogmo Editor](https://ogmo-editor-3.github.io/) to create maps manually
 * [simple2D](https://github.com/simple2d/simple2d) for visuals
 * [nlohmann json](https://github.com/nlohmann/json) for map saving and loading
