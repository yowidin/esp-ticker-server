# ESP platformio ticker server

## Build

This application requires installed SOCI library:
```
git clone git clone  https://github.com/SOCI/soci
cmake -B_builds -H. -DWITH_MYSQL=ON -DCMAKE_CXX_FLAGS:LIST="-frtti -fexceptions" -DSOCI_TESTS=OFF -DSOCI_CXX_C11=ON
cd _builds
sudo checkinstall -y
```

Build the server application:
```
cmake -B_builds -H. -DETS_DB_NAME=$DB_USERNAME -DETS_DB_NAME=$DB_NAME -DETS_PASSWORD=$DB_PASSWORD -DETS_SOCI_PATH=$SOCI_CMAKE_PATH
cmake --build _builds
```


Install all the components
   - Copy `index.php` into your web directory (adjust username and password if required)
   - Copy `esp-ticker-server.service` into `/lib/systemd/system/`
   - Enable service: `sudo systemctl enable esp-ticker-server.service`
   - Start service: `sudo systemctl start esp-ticker-server.service`
