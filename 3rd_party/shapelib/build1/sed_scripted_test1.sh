#!/bin/sh

EG_DATA=/u/www/projects/shapelib/eg_data

testdir="$(dirname "$(readlink -f $0)")"

(
cd "$top_builddir"
echo -------------------------------------------------------------------------
echo Test 1: dump anno.shp
echo -------------------------------------------------------------------------
/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/build1/shpdump $EG_DATA/anno.shp | head -250

echo -------------------------------------------------------------------------
echo Test 2: dump brklinz.shp
echo -------------------------------------------------------------------------
/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/build1/shpdump $EG_DATA/brklinz.shp | head -500

echo -------------------------------------------------------------------------
echo Test 3: dump polygon.shp
echo -------------------------------------------------------------------------
/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/build1/shpdump $EG_DATA/polygon.shp | head -500

echo -------------------------------------------------------------------------
echo Test 4: dump pline.dbf - uses new F field type
echo -------------------------------------------------------------------------
/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/build1/dbfdump -m -h $EG_DATA/pline.dbf | head -50

echo -------------------------------------------------------------------------
echo Test 5: NULL Shapes.
echo -------------------------------------------------------------------------
/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/build1/shpdump $EG_DATA/csah.dbf | head -150
) > "$testdir/s1.out"

result="$(diff "$testdir/s1.out" "$testdir/stream1.out")"
if [ "$result" == "" ]; then
	echo "******* Stream 1 Succeeded *********"
	rm "$testdir/s1.out"
	exit 0
else
	echo "******* Stream 1 Failed *********"
	echo "$result"
	rm "$testdir/s1.out"
	exit 1
fi
