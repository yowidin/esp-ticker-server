# ESP platformio ticker server

This application requires installed SOCI library:
```
git clone git clone  https://github.com/SOCI/soci
cmake -B_builds -H. -DWITH_MYSQL=ON -DCMAKE_CXX_FLAGS:LIST="-frtti -fexceptions" -DSOCI_TESTS=OFF -DSOCI_CXX_C11=ON
cd _builds
sudo checkinstall -y
```
