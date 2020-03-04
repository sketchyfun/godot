template_dir=~/.local/share/godot/templates/3.1.alpha

export OSXCROSS_ROOT=/home/ben/osxcross
#scons p=x11 target=release tools=no builtin_libpng=yes builtin_openssl=yes builtin_zlib=yes use_static_cpp=yes use_lto=yes -j 6
#scons p=windows target=release tools=no -j 6
scons p=osx osxcross_sdk=darwin15 target=release tools=no -j 6

#scons p=osx osxcross_sdk=darwin15 target=release_debug tools=no -j 6
strip bin/*
cp -r misc/dist/osx_template.app .
cp -r misc/dist/osx_template.app ./osx_template_debug.app
mkdir osx_template.app/Contents/MacOS
mkdir osx_template_debug.app/Contents/MacOS
#cp bin/godot.osx.opt.32 osx_template.app/Contents/MacOS/godot_osx_release.32
#cp bin/godot.osx.opt.debug.32 osx_template.app/Contents/MacOS/godot_osx_debug.32
cp bin/godot.osx.opt.64 osx_template.app/Contents/MacOS/godot_osx_release.64
cp bin/godot.osx.opt.debug.64 osx_template_debug.app/Contents/MacOS/godot_osx_debug.64
chmod +x osx_template.app/Contents/MacOS/godot_osx*
chmod +x osx_template_debug.app/Contents/MacOS/godot_osx*
echo "Creating osx zips..."
zip -q -9 -r bin/osx.zip osx_template.app
zip -q -9 -r bin/osx_debug.zip osx_template_debug.app
rm -rf osx_template.app
rm -rf osx_template_debug.app
echo "osx zips created!"
echo "Copying executables to godot template directory:" $template_dir
cp bin/godot.x11.opt.64 $template_dir/linux_x11_64_release
cp bin/godot.windows.opt.64.exe $template_dir/windows_64_release.exe
cp bin/osx.zip $template_dir/osx.zip
cp bin/osx_debug.zip $template_dir/osx_debug.zip
echo "Moving executables to bin/templates"
mv bin/godot.x11.opt.64 bin/templates
mv bin/godot.windows.opt.64.exe bin/templates
mv bin/osx.zip bin/templates
echo "All done!"