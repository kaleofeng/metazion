rm -rf Build/Windows
mkdir Build/Windows
cd Build/Windows
cmake -G "Visual Studio 14 Win64" ../..
cd ../..