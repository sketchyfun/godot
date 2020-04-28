#!/bin/bash
scons p=windows -j 6 tools=no target=release
scons p=x11 -j 6 tools=no target=release builtin_libpng=yes builtin_openssl=yes builtin_zlib=yes use_static_cpp=yes use_lto=yes
scons p=osx -j 6 target=release tools=no osxcross_sdk=darwin15

strip bin/*

mv bin/godot.x11.opt.64 bin/templates
mv bin/godot.windows.opt.64.exe bin/templates

cp -r misc/dist/osx_template.app .  
mkdir osx_template.app/Contents/MacOS  
mv bin/godot.osx.opt.64 osx_template.app/Contents/MacOS/godot_osx_release.64
chmod +x osx_template.app/Contents/MacOS/godot_osx*  
zip -q -9 -r osx.zip osx_template.app
rm -rf osx_template.app
mv osx.zip bin/templates

