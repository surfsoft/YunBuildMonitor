ROOT=/mnt/sd/job-$1
URL=`cat $ROOT/url.txt`
BUILD=$URL/api/json

STATUSLINE=`curl $BUILD | tr "," "\n" | grep color | tr ":" "\n" | tail -1`

RUNNING=1
if test -z `echo $STATUSLINE | grep anime`
then
  RUNNING=0
fi
  
STATUS=8
if test ! -z `echo $STATUSLINE | grep yellow`
then
  STATUS=4
fi
if test ! -z `echo $STATUSLINE | grep blue`
then
  STATUS=2
fi
      
echo $STATUS > $ROOT/status.txt
echo $RUNNING > $ROOT/running.txt
