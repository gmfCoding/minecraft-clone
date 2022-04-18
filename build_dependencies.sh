buildGlad=("
-c ${PWD}/include/glad/glad.c 
-o ${PWD}/bintmp/glad.o"
)

buildFileio=("
-c ${PWD}/include/other/fileio.cpp 
-o ${PWD}/bintmp/fileio.o"
)
buildAtulPwd=("
-c ${PWD}/include/other/AtulPwd.cpp 
-o ${PWD}/bintmp/AtulPwd.o"
)

/usr/bin/g++ ${buildGlad[*]} && echo "Glad Built Sucessfully" || echo "Glad Build Failed"
/usr/bin/g++ ${buildFileio[*]} && echo "Filio Built Sucessfully" || echo "Filio Build Failed"
/usr/bin/g++ ${buildAtulPwd[*]} && echo "AtulPwd Built Sucessfully" || echo "AtulPwd Build Failed"