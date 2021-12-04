# /usr/bin/g++ \
# -fdiagnostics-color=always \
# ./src/*.cpp \
# -g \
# -o ./bin/main \
# ./bintmp/glad.o \
# ./bintmp/fileio.o \
# -I./include/fileio \
# -I./include \
# -I./include/glad \
# -L./lib \
# -std=c++17 \
# -lpng \
# -lGL \
# -lX11 \
# -lglfw \
# -ldl \
# -pthread \

argArray=("
-fdiagnostics-color=always
-g

${PWD}/src/*.cpp
-o ${PWD}/bin/main

${PWD}/bintmp/glad.o
${PWD}/bintmp/fileio.o

-I${PWD}/include/other
-I${PWD}/include
-I${PWD}/include/glad
-L${PWD}/lib

-std=c++17
-lpng
-lGL
-lX11
-lglfw
-ldl
-pthread
-DGLM_FORCE_XYZW_ONLY
"
)

/usr/bin/g++ ${argArray[*]} && echo "Build Successful" || echo "Build Failed"