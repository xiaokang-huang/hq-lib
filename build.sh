cd mylib
scons -c
scons -Q
cp ./libmylib.so ../
cd ..

cd test_mylib
scons -c
scons -Q
cp ./testmylib ../
cd ..

