rm -rf Build/Macos
mkdir -p Build/Macos
cd Build/Macos
cmake -G "Xcode" ../..
cd ../..