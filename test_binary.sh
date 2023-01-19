cat /dev/urandom | head -c 1000000 > binaryfile # generate a 1MB file
curl -o downloadfile localhost:15635/binaryfile # request the file
diff downloadfile binaryfile # check if the downloaded file is intact
