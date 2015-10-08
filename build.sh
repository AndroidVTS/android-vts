./gradlew assembleRelease && cp app/build/outputs/apk/app-release-unsigned.apk bin/androidVTS.apk && signapk bin/androidVTS.apk

adb install -r bin/AndroidVTS.apk
