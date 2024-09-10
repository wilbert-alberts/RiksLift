DATE=`date '+%Y-%m-%d %H:%M:%S'`
echo "RiksLift service started at ${DATE}" | systemd-cat -p info

/home/ben/RL.exe mqtt://localhost 2>>/home/ben/RiksLog/RL.log

