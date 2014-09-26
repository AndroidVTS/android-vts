ndk-build

NAME="cve-2014-4943check"
adb push libs/armeabi/$NAME /data/local/tmp/ && \
adb shell chmod 755 /data/local/tmp/$NAME && \
adb shell /data/local/tmp/$NAME
