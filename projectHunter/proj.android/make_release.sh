ANDROID_ROOT=/tools/adt-bundle-mac-x86_64-20140702/sdk/tools
bash build_native.sh
rm -r ./bin
$ANDROID_ROOT/android update project --name hunter -t 1 -p `pwd` --subprojects
ant release