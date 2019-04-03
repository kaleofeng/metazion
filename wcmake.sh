rm -rf Build/Windows
mkdir -p Build/Windows
cd Build/Windows
cmake -G "Visual Studio 16 2019" ../..
cd ../..