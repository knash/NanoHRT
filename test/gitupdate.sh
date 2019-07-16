echo Updating Git and compiling...
echo All changes will be stashed and overwritten
git stash
git fetch
git pull
cd ../../../ ; scram b -j 12 ; cd -
echo done!
