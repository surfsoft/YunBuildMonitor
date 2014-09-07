# TODO needs re-writing for the new structure
echo "This setup script has not, yet, been updated for the NeoPixel version of the build monitor."
exit 1

SDCARD=/mnt/sd

if test ! -d $SDCARD
then
    echo
    echo "ERROR: /mnt/sd not found"
    echo
    echo "Have you inserted a microSD card, with a directory called 'arduino' in the root, into the Yun's card slot?"
    echo
    exit 1
fi

echo
echo "Yun Build Monitor Setup"
echo "======================="
echo
echo "WARNING: This will remove your existing Yun Build Monitor setup (if present)."
echo -n "Are you sure you wish to continue? (answer 'yes' to continue setup): "
read ANSWER

if [ "$ANSWER" != "yes" ]
then
    exit 1
fi

echo
echo -n "Creating job configurations... "

cd $SDCARD
rm -rf job-0
mkdir job-0
cd job-0
echo "0" > running.txt
echo "2" > status.txt
echo "http://jenkins.cyanogenmod.com/job/android" > url.txt
cd ..

rm -rf job-1
mkdir job-1
cd job-1
echo "0" > running.txt
echo "2" > status.txt
echo "http://jenkins.cyanogenmod.com/job/cm-build-all" > url.txt
cd ..

rm -rf job-2
mkdir job-2
cd job-2
echo "0" > running.txt
echo "2" > status.txt
echo "http://jenkins.cyanogenmod.com/job/cm_daily_build_cycle" > url.txt
cd ..

rm -rf job-3
mkdir job-3
cd job-3
echo "0" > running.txt
echo "2" > status.txt
echo "http://jenkins.cyanogenmod.com/job/recovery" > url.txt
cd ..

echo "done."

echo -n "Enabling cron daemon... "

/etc/init.d/cron start
/etc/init.d/cron enable

echo "done."

echo
echo "Job directories set up. Don't forget to edit url.txt in each to change the job being montored."
echo "You will also need to add the following crontab entries (use crontab -e):"
echo
echo "    * * * * * $SDCARD/refresh-all.sh"
echo
