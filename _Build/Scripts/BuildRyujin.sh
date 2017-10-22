#!/bin/bash

if [ "$#" -ne 1 ] 
then
    echo "Usage: sh BuildRyujin.sh <configuration>"
    exit 1
fi

CONFIGURATION=$1

echo "Building RyujinCore"
cd ../../RyujinCore
xcodebuild -project RyujinCore.xcodeproj -configuration $CONFIGURATION clean
xcodebuild -project RyujinCore.xcodeproj -configuration $CONFIGURATION
err=$?
if [ $err -ne 0 ] 
then
    echo "\nRyujinCore build failed... stopping\n"
    exit 1
fi


echo "Building RyujinRenderer"
cd ../RyujinRenderer
xcodebuild -project RyujinRenderer.xcodeproj -configuration $CONFIGURATION clean
xcodebuild -project RyujinRenderer.xcodeproj -configuration $CONFIGURATION
err=$?
if [ $err -ne 0 ] 
then
    echo "\nRyujinRenderer build failed... stopping\n"
    exit 1
fi


echo "Building Ryujin"
RYUJIN_TARGET=Ryujin
cd ../Ryujin
if [ "$CONFIGURATION" = "Debug" ]
then
    RYUJIN_TARGET=RyujinDebug
fi
xcodebuild -project Ryujin.xcodeproj -target $RYUJIN_TARGET -configuration $CONFIGURATION clean
xcodebuild -project Ryujin.xcodeproj -target $RYUJIN_TARGET -configuration $CONFIGURATION
err=$?
if [ $err -ne 0 ] 
then
    echo "\nRyujin build failed... stopping\n"
    exit 1
fi