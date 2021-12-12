buildGlad=("
-c ${PWD}/include/glad/glad.c 
-o ${PWD}/bintmp/glad.o"
)

buildFileio=("
-c ${PWD}/include/other/fileio.cpp 
-o ${PWD}/bintmp/fileio.o"
)

/usr/bin/g++ ${buildGlad[*]} && echo "Glad Built Sucessfully" || echo "Glad Build Failed"
/usr/bin/g++ ${buildFileio[*]} && echo "Filio Built Sucessfully" || echo "Filio Build Failed"