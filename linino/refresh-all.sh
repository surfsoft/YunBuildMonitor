ROOT=/mnt/sd
jobCount=$((`wc -l $ROOT/urls.txt | tr " " "\n" | head -1`))
jobsState=""
for i in $(seq 1 $jobCount)
do
    jobUrl="`head -$i $ROOT/urls.txt | tail -1`/api/json"
    statusLine=`curl $jobUrl | tr "," "\n" | grep color | tr ":" "\n" | tail -1`
    jobState=0
    if test ! -z `echo $statusLine | grep yellow`
    then
        jobState=1
    else
        if test ! -z `echo $statusLine | grep blue`
        then
            jobState=2
        fi
    fi
    jobRunning=0
    if test -z `echo $statusLine | grep anime`
    then
        jobRunning=1
    fi
    jobsState="`echo $jobsState`$jobState$jobRunning"    
done
echo $jobsState > $ROOT/status.txt
