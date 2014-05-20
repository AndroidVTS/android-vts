#! /bin/sh

TMP_DIR=`mktemp -d`

CLASS_NAME=ZipBugChecker
javac -Xlint $CLASS_NAME.java -d $TMP_DIR && \
dx --dex --no-strict --output=$TMP_DIR/classes.dex  $TMP_DIR/ && \
zip -j $TMP_DIR/$CLASS_NAME.jar $TMP_DIR/classes.dex && \
adb push $TMP_DIR/$CLASS_NAME.jar /data/local/tmp/ && \
adb shell mkdir /data/local/tmp/dalvik-cache && \
adb shell ANDROID_DATA=/data/local/tmp dalvikvm -cp /data/local/tmp/$CLASS_NAME.jar $CLASS_NAME

rm -r ${TMP_DIR}
