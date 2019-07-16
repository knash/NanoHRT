echo Updating Git and compiling...
git stash
git fetch
git pull
cd ../../../ ; scram b -j 12 ; cd -
echo done!
