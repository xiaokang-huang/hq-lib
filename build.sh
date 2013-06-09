mkdir out

cd mylib
scons -c
scons -Q
cp ./libmylib.so ../out/
cd ..

cd test_mylib
scons -c
scons -Q
cp ./testmylib ../out/
cd ..

