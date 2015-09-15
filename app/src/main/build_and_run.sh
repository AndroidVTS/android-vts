ndk-build

NAME=pingpongCheck-pie

#enable kysms
adb push libs/armeabi/$NAME /data/local/tmp/ && \
adb shell chmod 755 /data/local/tmp/$NAME && \
adb shell /data/local/tmp/$NAME $1
#print the failed memory access when from last_kmsg when the phone comes back up
