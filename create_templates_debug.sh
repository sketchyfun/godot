template_dir=~/.local/share/godot/templates/3.1.alpha

export OSXCROSS_ROOT=/home/ben/osxcross
scons p=x11 target=release_debug tools=no builtin_libpng=yes builtin_openssl=yes builtin_zlib=yes use_static_cpp=yes use_lto=yes -j 6
scons p=windows target=release_debug tools=no -j 6
scons p=osx osxcross_sdk=darwin15 target=release_debug tools=no -j 6
strip bin/*
cp -r misc/dist/osx_template.app .
mkdir osx_template.app/Contents/MacOS
#cp bin/godot.osx.opt.32 osx_template.app/Contents/MacOS/godot_osx_release.32
#cp bin/godot.osx.opt.debug.32 osx_template.app/Contents/MacOS/godot_osx_debug.32
cp bin/godot.osx.opt.64 osx_template.app/Contents/MacOS/godot_osx_release.64
#cp bin/godot.osx.opt.debug.64 osx_template.app/Contents/MacOS/godot_osx_debug.64
chmod +x osx_template.app/Contents/MacOS/godot_osx*
echo "Creating osx.zip..."
zip -q -9 -r bin/osx.zip osx_template.app
rm -rf osx_template.app
echo "osx.zip created!"
echo "Copying executables to godot template directory:" $template_dir
cp bin/godot.x11.opt.64 $template_dir/linux_x11_64_debug
cp bin/godot.windows.opt.64.exe $template_dir/windows_64_debug.exe
cp bin/osx.zip $template_dir/osx.zip
echo "Moving executables to bin/templates"
mv bin/godot.x11.opt.64 bin/templates
mv bin/godot.windows.opt.64.exe bin/templates
mv bin/osx.zip bin/templates
echo "All done!"