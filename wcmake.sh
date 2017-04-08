rm -rf Build/Windows
mkdir -p Build/Windows
cd Build/Windows
cmake -G "Visual Studio 15 Win64" ../..
cd ../..