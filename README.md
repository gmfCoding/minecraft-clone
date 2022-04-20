# About
**sample-glad-imgui-cmake** is my example project for getting **imgui**, **glad**, **glfw** with **cmake** to generate the buildfiles.

**Note**: I'm very unexperienced in this area so something could be improved but you are welcome to use this project at your own volition.

This project was mainly made so I could learn about **cmake**, **glad** and **imgui**.


# Building

I don't quite entirely understand how cmake works yet, I've been working on this with Windows 10 with **MSYS2 MinGW** installed so I've been using the **MinGW Makefiles** generators

1. Create a build directory (for cleanliness/organisation and easy cleaning)

    ```mkdir build;```

    ```cd build;```

2. Generate the MinGW Makefiles makefiles

    ```cmake -G "MinGW Makefiles" ../;```

3. Run the buildsystem to compile the program

    ```cmake --build .;```

# Building with G++

Just for the sake of the argument how would we go about building this with g++?

1. Create some folders

	`mkdir gpptmp;`

	`mkdir gppbin;`

	`mkdir lib;`

2. Download glfw3 binaries (or build them)
[glfw.org/download](https://www.glfw.org/download)

3. Grab the appropriate binaries and place them in our newly created lib directory (such as `\lib\glfw3dll.lib`)

4. Separatly compile glad

	`g++ -c app\vendor\glad\src\glad.c -o gpptmp\glad.o -Iapp\vendor\glad\include;`

5. Compile the main program
	`g++ app\src\*.cpp app\vendor\imgui\*.cpp app\vendor\imgui\backends\imgui_impl_opengl3.cpp app\vendor\imgui\backends\imgui_impl_glfw.cpp -o gppbin\main.exe gpptmp\glad.o app\srcinc -Iapp\vendor\include -Iapp\vendor\glad\include -Iapp\vendor\imgui -Iapp\vendor\imgui\backends -Iapp\vendor\glfw -Llib\ -DGLFW_INCLUDE_NONE -lglfw3 -std=c++17;`
6. Now we just have to place the `glfw3.dll` inside our `gppout` directory and the executable should be able to be executed!

What a mess! That's why we use a buildsystem so we don't have to deal with this mess everytime.
Adding new source files is a pain this way, but can be some what cleaned up (see [gpp_build.bat](/gpp_build.bat))

