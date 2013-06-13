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

cd engine
scons -c
scons -Q
cp ./libhqengine.so ../
cd ..

cd test_engine
scons -c
scons -Q
cp ./testengine ../
cd ..

